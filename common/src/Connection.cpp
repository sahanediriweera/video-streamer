//
// Created by Sahan Ediriweera on 2026-02-03.
//

inline int set_nonblocking(int fd) {
    int flags = fcntl(fd,F_GETFL,0);
    if (flags == -1) return -1;
    int result = fcntl(fd,F_SETFL,flags | O_NONBLOCK);
}