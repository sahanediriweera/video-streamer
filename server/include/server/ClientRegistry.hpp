//
// Created by Sahan Ediriweera on 2026-02-03.
//
#pragma once
#include <unordered_map>

#include "ClientSession.hpp"

class ClientRegistry {
private:
    std::unordered_map<int, std::unique_ptr<ClientSession>> clients;
public:
    ClientRegistry() = default;
    ~ClientRegistry() = default;

    ClientRegistry(const ClientRegistry&) = delete;
    ClientRegistry& operator=(const ClientRegistry&) = delete;
    ClientRegistry(ClientRegistry&&) = delete;
    ClientRegistry& operator=(ClientRegistry&&) = default;

    bool add_client(int fd,std::unique_ptr<ClientSession> session);

    bool remove_client(int fd);

    [[nodiscard]] ClientSession* get_client(int fd) const;
    [[nodiscard]] bool has_client(int fd) const;

    [[nodiscard]] size_t client_count() const { return clients.size();}

    void broadcast(int sender_fd,const std::vector<char>& data);

    [[nodiscard]] std::vector<int> get_all_fds() const;

    void clear();
};