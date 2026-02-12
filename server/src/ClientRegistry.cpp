//
// Created by Sahan Ediriweera on 2026-02-12.
//

#ifndef VIDEO_STREAMER_CLIENTREGISTRY_HPP
#define VIDEO_STREAMER_CLIENTREGISTRY_HPP
#include "server/ClientRegistry.hpp"
#include "server/ClientSession.hpp"
#include <iostream>
#include <ranges>

bool ClientRegistry::add_client(int fd,std::unique_ptr<ClientSession> session) {
    if (has_client(fd)) {
        std::cerr << "Client with fd " << fd << " already exists\n";
    }

    clients[fd] = std::move(session);
    std::cout << "Client registed: " << fd << " (Total clients: " << clients.size() << ") \n";
    return true;
}

bool ClientRegistry::remove_client(int fd) {
    const auto it = clients.find(fd);
    if (it == clients.end()) {
        return false;
    }

    std::cout << "Client unregistered: " << fd << " (Remaining clients: " << (clients.size() -1) << ")\n";
    clients.erase(it);
    return true;
}

ClientSession* ClientRegistry::get_client(int fd) const {
    auto const it = clients.find(fd);

    if (it == clients.end()) {
        return nullptr;
    }

    return it->second.get();
}

bool ClientRegistry::has_client(const int fd) const {
    return clients.contains(fd);
}

void ClientRegistry::broadcast(int sender_fd,const std::vector<char>& data) {
    size_t sent_count = 0;
    for (auto const& [fd,session]:clients) {
        if (fd != sender_fd) {
            if (session->send(data)) {
                ++sent_count;
            }
        }
    }

    std::cout << "Broadcast from client " << sender_fd << " to " <<sent_count << " clients\n";
}

std::vector<int> ClientRegistry::get_all_fds() const {
    std::vector<int> fds;
    fds.reserve(clients.size());
    for (const auto &fd: clients | std::views::keys) {
        fds.push_back(fd);
    }
    return fds;
}

void ClientRegistry::clear() {
    std::cout << "Clearing all clients (" << clients.size() << ")\n";
    clients.clear();
}


#endif
