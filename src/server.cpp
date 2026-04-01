#include "server.hpp"
#include "thread_safe_iostream.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

void Server::start(const size_t& p_port)
{
    if (_started)
    {
        threadSafeCout << "Server already started on port " << _port << "\n";
        return;
    }

    _port = p_port;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        threadSafeCout << "error: Failed to create socket" << std::endl;
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_port);

    if (bind(_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        close(_socket);
        threadSafeCout << "error: Bind failed" << std::endl;
        return;
    }

    if (listen(_socket, SOMAXCONN) < 0)
    {
        close(_socket);
        threadSafeCout << "error: Listen failed" << std::endl;
        return;
    }

    fcntl(_socket, F_SETFL, O_NONBLOCK);
    _started = true;
    threadSafeCout << "Server started on port " << _port << std::endl;
}

void Server::defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID)
{
    auto it = _clients.find(clientID);
    if (it == _clients.end()) return;

    int sock = it->second;

    DataBuffer buffer;
    buffer << message;

    ssize_t sent = ::send(sock, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);
    if (sent < 0)
        threadSafeCout << "error: Failed to send to client " << clientID << std::endl;
}

void Server::sendToArray(const Message& message, std::vector<long long> clientIDs)
{
    for (auto id : clientIDs)
        sendTo(message, id);
}

void Server::sendToAll(const Message& message)
{
    for (const auto& [id, client] : _clients)
        sendTo(message, id);
}

void Server::update()
{
    if (!_started) return;

}