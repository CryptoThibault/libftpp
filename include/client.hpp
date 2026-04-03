#pragma once

#include <unordered_map>
#include "message.hpp"
#include "server.hpp"

class Client {
public:
    void connect(const std::string& address, const size_t& port);
    void disconnect();
    void defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action);
    void send(const Message& message);
    void update();

private:
    int _socket;
    bool _connected = false;
    std::unordered_map<Message::Type, std::function<void(const Message&)>> _actions;
    std::vector<std::byte> _recvBuffer;
};