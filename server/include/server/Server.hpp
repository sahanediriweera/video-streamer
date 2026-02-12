//
// Created by Sahan Ediriweera on 2026-02-03.
//

#ifndef VIDEO_STREAMER_SERVER_HPP
#define VIDEO_STREAMER_SERVER_HPP
#include <memory>
#include <unordered_map>
#include <poll.h>
#include <vector>

#include "ClientRegistry.hpp"

template<typename T>
class Connection; //Should I use the import or declaration here? check in book

class Server {
private:
    int server_fd{};
    std::vector<pollfd> poll_fds;
    ClientRegistry client_registry;

    void accept_client();
    void handle_client_data(int fd,size_t pollIndex); //having a poll index is not suitable connect all together if possible
    void setup_server_socket(uint16_t port);

public:
    explicit Server(uint16_t port);
    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    void run();

    const ClientRegistry& get_registry() const {return client_registry;}

};

void set_non_blocking(int fd);

#endif //VIDEO_STREAMER_SERVER_HPP