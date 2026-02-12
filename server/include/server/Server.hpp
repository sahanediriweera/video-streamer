//
// Created by Sahan Ediriweera on 2026-02-03.
//

#ifndef VIDEO_STREAMER_SERVER_HPP
#define VIDEO_STREAMER_SERVER_HPP
#include <memory>
#include <unordered_map>
#include <poll.h>
#include <vector>

template<typename T>
class Connection; //Should I use the import or declaration here? check in book

class Server {
private:
    int server_fd;
    std::vector<pollfd> poll_fds;
    std::unordered_map<int,std::shared_ptr<Connection<std::vector<char>>>> clients;

    void accept_client();
    void handle_client_data(int fd,size_t pollIndex); //having a poll index is not suitable connect all together if possible
    void broadcast(int sender_fd,const std::vector<char>& data);

public:
    explicit Server(uint16_t port);
    void run();

};

void set_non_blocking(int fd);

#endif //VIDEO_STREAMER_SERVER_HPP