//
// Created by Sahan Ediriweera on 2026-02-03.
//
#include <vector>
#include <common/Connection.hpp>

class Client {
private:
    std::shared_ptr<Connection<std::vector<char>>> conn;
};