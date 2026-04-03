#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include "message.hpp"
#include "client.hpp"

class Server
{
public:
    void start(const size_t& p_port);
    void defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action);
    void sendTo(const Message& message, long long clientID);
    void sendToArray(const Message& message, std::vector<long long> clientIDs);
    void sendToAll(const Message& message);
    void update();

private:
    int _socket;
    size_t _port;
    bool _started = false;
    std::unordered_map<Message::Type, std::function<void(long long& clientID, const Message& msg)>> _actions;
    std::unordered_map<long long, int> _clients;
    std::unordered_map<long long, std::vector<std::byte>> _recvBuffers;
    long long _nextClientID = 1;
};