//
// Created by Sahan Ediriweera on 2026-02-03.
//

#include "server/ClientSession.hpp"
#include <iostream>
#include <fcntl.h>
#include "common/Connection.hpp"


void set_non_blocking(int fd) {
    int flags = fcntl(fd,F_GETFL,0);

    if (flags == -1) {
        throw std::runtime_error("fcntl F_GETFL failed");
    }

    if (fcntl(fd,F_SETFL,flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("fcntl S_SETFL failed");
    }
}

ClientSession::ClientSession(const int fd):client_fd(fd) {
    set_non_blocking(client_fd);
    connection = std::make_shared<Connection<std::vector<char>>>(client_fd);
    std::cout<<"ClientSession created for fd: " << client_fd << "\n";
}

ClientSession::~ClientSession() {
    disconnect();
}

ssize_t ClientSession::receive(std::vector<char>& data) const {
    return connection->receive_data(data);
}

bool ClientSession::send(const std::vector<char>& data) const {
    const ssize_t result = connection->send_data(data);
    return result>0;
}

void ClientSession::disconnect() {
   if (client_fd >=0) {
       std::cout << "ClientSession disconnecting fd: " << client_fd << "\n";
       close(client_fd);
       client_fd = -1;
   }
}