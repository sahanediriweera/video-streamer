//
// Created by Sahan Ediriweera on 2026-02-03.
//

#ifndef VIDEO_STREAMER_CONNECTION_HPP
#define VIDEO_STREAMER_CONNECTION_HPP
#include <string>
#include <type_traits>
#include <unistd.h>
#include <vector>

inline int set_nonblocking(const int fd);

template<typename T>
class Connection {
    static_assert(
        std::is_same_v<T,std::pmr::vector<char>> || std::is_same_v<T,std::string>,
        "Connection supports only std::pmr::vector<char> and std::string"
    );

private:
    const int fd;
public:
    explicit Connection(const int socket_fd) :fd(socket_fd) {}
    virtual ~Connection() {close(fd); }

    bool send_data(const T& data);
    ssize_t receive_data(T& buffer);
    [[nodiscard]] int get_fd() const { return fd; }
};

#endif //VIDEO_STREAMER_CONNECTION_HPP