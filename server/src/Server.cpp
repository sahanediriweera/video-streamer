//
// Created by Sahan Ediriweera on 2026-02-03.
//


#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common/Connection.hpp"
#include "server/Server.hpp"

void set_non_blocking(const int fd) {
    const int flags = fcntl(fd,F_GETFL,0);
    if (flags == -1) {
        throw std::runtime_error("fcntl F_GETFL failed"); //Todo add an exit clause
    }
    if (fcntl(fd,F_SETFL,flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("fcntl F_SETFL failed");
    }
}

void Server::setup_server_socket(uint16_t port) {
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if (server_fd < 0) {
        throw std::runtime_error("Could not create server socket");// add exit clause
    }

    set_non_blocking(server_fd);

    constexpr int opt = 1; // is this performant benefiting check again
    if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0) {
        close(server_fd);
        throw std::runtime_error("setsocketopt failed");
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd,reinterpret_cast<sockaddr*>(&addr),sizeof(addr))< 0) {
        close(server_fd);
        throw std::runtime_error("bind failed");
    }

    if (listen(server_fd,10)< 0) {
        close(server_fd);
        throw std::runtime_error("Listen failed");
    }

    struct pollfd server_poll{};
    server_poll.fd = server_fd;
    server_poll.events = POLLIN;
    poll_fds.push_back(server_poll);
}

Server::Server(const uint16_t port) {

    setup_server_socket(port);
    std::cout << "Server listening on port: " << port << "\n";

}

Server::~Server() {
    client_registry.clear();
    if (server_fd >= 0) {
        close(server_fd);
    }
}

void Server::accept_client() {
    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    int client_fd;
    while ((client_fd = accept(server_fd,reinterpret_cast<sockaddr*>(&client_addr),&client_addr_len)) == -1) {
        if (errno != EINTR) {
            perror("accept failed");
            return;
        }
    }

    try {
        set_non_blocking(client_fd);
    }catch (const std::exception& e) {
        std::cerr << "Failed to set non-blocking: " << e.what() << "\n";
        close(client_fd);
        return;
    }

    try {
        if (auto session = std::make_unique<ClientSession>(client_fd); !client_registry.add_client(client_fd,std::move(session))) {
            std::cerr<< "Failed to register client " << client_fd << "\n";
            close(client_fd);
            return;
        }

        poll_fds.push_back({client_fd,POLLIN,0});
        std::cout << "New Clinet connected " << client_fd << "\n";
    }catch (const std::exception& e) {
        std::cerr << "Sessopm creation failed: " << e.what() << "\n";
        auto it = std::ranges::find_if(poll_fds.begin(),poll_fds.end(),[client_fd](const pollfd& p) {
            return p.fd == client_fd;
        });

        if (it != poll_fds.end()) {
            poll_fds.erase(it);
            close(client_fd);
        }
    }
}

void Server::handle_client_data(const int fd, const size_t pollIndex) {
    ClientSession* session = client_registry.get_client(fd);
    if (!session) {
        close(fd);
        poll_fds.erase(poll_fds.begin() + pollIndex);
        return;
    }

    std::vector<char> data;

    if (ssize_t bytes = session->receive(data); bytes> 0) {
        std::cout << "Received " << bytes << " bytes from client " << fd << "\n";
        client_registry.broadcast(fd,data);
    }else if (bytes == 0 || (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
        std::cout << "Client disconnected: " << fd << "\n";
        client_registry.remove_client(fd);
        poll_fds.erase(poll_fds.begin() + pollIndex);
    }
}

void Server::run() {
    std::cout << "Server running...\n";

    while (true) {

        if (int poll_count = poll(poll_fds.data(),poll_fds.size(),-1); poll_count < 0) {
            std::cerr << "poll failed" << strerror(errno) << "\n";
            break;
        }

        for (size_t i = 0; i < poll_fds.size(); ++i) { //TODO refactor this
            if (poll_fds[i].revents & POLLIN) {
                if (poll_fds[i].fd == server_fd) {
                    accept_client();
                }else {
                    handle_client_data(poll_fds[i].fd,i);
                }
            }
        }
    }
}
