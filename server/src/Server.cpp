//
// Created by Sahan Ediriweera on 2026-02-03.
//


#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
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

Server::Server(uint16_t port) {
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

    std::cout << "Server listening on port: " << port << "\n";

}

void Server::accept_client() {
    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    if (int client_fd = accept(server_fd,reinterpret_cast<sockaddr*>(&client_addr),&client_addr_len); client_fd >= 0) {
        try {
            set_non_blocking(client_fd);
        }catch(std::exception& e) {
            std::cerr << "Failed to set nnon-blocking client" << e.what() << "\n";
            close(client_fd);
            return;
        }

        struct pollfd client_poll{};
        client_poll.fd = client_fd;
        client_poll.events = POLLIN;
        poll_fds.push_back(client_poll);

        clients[client_fd] = std::make_shared<Connection<std::vector<char>>>(client_fd);
        std::cout << "New client connected" << client_fd << "\n";
    }
}

void Server::handle_client_data(int fd,size_t poll_index) {
    auto it = clients.find(fd);
    if (it == clients.end()) {
        close(fd);
        poll_fds.erase(poll_fds.begin()+ poll_index);
        return;
    }

    auto& conn = it->second;
    std::vector<char> data;
    ssize_t bytes = conn->receive_data(data);
    if (bytes > 0) {
        std::cout<< "Received " << bytes << " bytes from client " << fd << "\n";
        broadcast(fd,data);
    }else if (bytes == 0 || (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
        std::cout << "Client disconnected: " << fd << "\n";
        close(fd);
        clients.erase(fd);
        poll_fds.erase(poll_fds.begin()+poll_index);
    }
}

void Server::broadcast(const int sender_fd,const std::vector<char>& data) {
    for (auto& [fd,client] : clients) {
        if (fd != sender_fd) {
            client->send_data(data);
        }
    }
}