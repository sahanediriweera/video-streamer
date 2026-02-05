//
// Created by Sahan Ediriweera on 2026-02-03.
//
#include <unistd.h>
#include <type_traits>
#include <vector>
#include <fcntl.h>
#include <sys/socket.h>

inline int set_nonblocking(const int fd) {
    const int flags = fcntl(fd,F_GETFL,0);
    if (flags == -1) return -1;
    const int result = fcntl(fd,F_SETFL,flags | O_NONBLOCK);
    return result;
}

template<typename T>
class Connection {
    static_assert(std::is_same_v<T,std::pmr::vector<char>> || std::is_same_v<T,std::string>,"Connection supports only std::vector<char> or std::string for now");

private:
    const int fd;

public:
    explicit Connection(const int socket_fd):fd(socket_fd){}
    virtual ~Connection() {close(fd);}

    bool sendData(const T& data) {
        const ssize_t sent = ::send(fd,data.data(),data.size(),0);
        return sent == static_cast<ssize_t>(data.size());
    }

    ssize_t receiveData(T& buffer) {
        buffer.resize(4096); //TODO add a macro
        const ssize_t bytesRead = ::recv(fd,buffer.data(),buffer.size(),0);
        if (bytesRead > 0) buffer.resize(bytesRead);
        return bytesRead;
    }

    [[nodiscard]] int getFd() const { return fd;}
};