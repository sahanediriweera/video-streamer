//
// Created by Sahan Ediriweera on 2026-02-03.
//

#ifndef VIDEO_STREAMER_CLIENTSESSION_HPP
#define VIDEO_STREAMER_CLIENTSESSION_HPP

#include <memory>
#include <vector>
#include "common/Connection.hpp"



class ClientSession {
private:
    int client_fd;
    std::shared_ptr<Connection<std::vector<char>>> connection;

public:
    explicit ClientSession(int fd);
    ~ClientSession();

    ClientSession(const ClientSession&) = delete;
    ClientSession& operator=(const ClientSession&) = delete;
    ClientSession(ClientSession&&) = default;
    ClientSession& operator=(ClientSession&&) = default;

    int get_fd() const {return client_fd;}

    ssize_t receive(std::vector<char>& data) const;

    bool send(const std::vector<char>& data) const;

    void disconnect();
};

#endif //VIDEO_STREAMER_CLIENTSESSION_HPP