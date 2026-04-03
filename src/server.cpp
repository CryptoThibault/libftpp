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
    _actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID)
{
    int type = message.type();
    uint32_t dataSize = static_cast<uint32_t>(message.data().size());
    std::vector<std::byte> sendBuffer(sizeof(type) + sizeof(dataSize) + dataSize);

    std::memcpy(sendBuffer.data(), &type, sizeof(type));
    std::memcpy(sendBuffer.data() + sizeof(type), &dataSize, sizeof(dataSize));

    if (dataSize > 0)
        std::memcpy(sendBuffer.data() + sizeof(type) + sizeof(dataSize), message.data().data(), dataSize);

    auto it = _clients.find(clientID);
    if (it == _clients.end()) return;

    ssize_t sent = ::send(it->second, sendBuffer.data(), sendBuffer.size(), MSG_DONTWAIT);
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
    for (auto& [clientID, sock] : _clients)
        sendTo(message, clientID);
}

void Server::update()
{
    if (!_started) return;

    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int clientSocket = accept(_socket, (sockaddr*)&clientAddr, &addrLen);
    if (clientSocket >= 0)
    {
        long long clientID = _nextClientID++;
        _clients[clientID] = clientSocket;
        _recvBuffers[clientID] = {};
        threadSafeCout << "New client connected: " << clientID << "\n";
    }

    std::vector<long long> disconnectedClients;

    for (auto& [clientID, sock] : _clients)
    {
        char temp[4096];
        ssize_t bytes = recv(sock, temp, sizeof(temp), MSG_DONTWAIT);

        if (bytes == 0)
        {
            threadSafeCout << "Client disconnected: " << clientID << "\n";
            disconnectedClients.push_back(clientID);
            continue;
        }
        else if (bytes < 0)
        {
            continue;
        }

        auto& buffer = _recvBuffers[clientID];
        buffer.insert(buffer.end(), reinterpret_cast<std::byte*>(temp), reinterpret_cast<std::byte*>(temp + bytes));

        while (true)
        {
            if (buffer.size() < sizeof(int) + sizeof(uint32_t))
                break;

            int type;
            uint32_t size;

            std::memcpy(&type, buffer.data(), sizeof(int));
            std::memcpy(&size, buffer.data() + sizeof(int), sizeof(uint32_t));

            if (buffer.size() < sizeof(int) + sizeof(uint32_t) + size)
                break;

            Message msg(type);
            if (size > 0)
                msg.data().append(buffer.data() + sizeof(int) + sizeof(uint32_t), size);

            auto it = _actions.find(type);
            if (it != _actions.end())
            {
                long long idCopy = clientID;
                it->second(idCopy, msg);
            }

            buffer.erase(buffer.begin(), buffer.begin() + sizeof(int) + sizeof(uint32_t) + size);
        }
    }

    for (long long cid : disconnectedClients)
    {
        close(_clients[cid]);
        _clients.erase(cid);
        _recvBuffers.erase(cid);
    }
}