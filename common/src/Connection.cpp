//
// Created by Sahan Ediriweera on 2026-02-03.
//
#include "common/Connection.hpp"
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
    bool Connection<T>::send_data(const T& data) {
        const ssize_t sent = ::send(fd,data.data(),data.size(),0);
        return sent == static_cast<ssize_t>(data.size());
    }

template<typename T>
    ssize_t Connection<T>::receive_data(T& buffer) {
        buffer.resize(4096); //TODO add a macro
        const ssize_t bytesRead = ::recv(fd,buffer.data(),buffer.size(),0);
        if (bytesRead > 0) buffer.resize(static_cast<size_t>(bytesRead));
        return bytesRead;
    }