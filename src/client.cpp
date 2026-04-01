#include "client.hpp"
#include "thread_safe_iostream.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

void Client::connect(const std::string& address, const size_t& port)
{
    if (_connected)
    {
        threadSafeCout << "Already connected, disconnecting first\n";
        disconnect();
    }

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        threadSafeCout << "error: Failed to create socket" << std::endl;
        _connected = false;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<uint16_t>(port));

    if (inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0)
    {
        threadSafeCout << "error: Invalid address: " << address << std::endl;
        close(_socket);
        _connected = false;
        return;
    }

    if (::connect(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        threadSafeCout << "error: Failed to connect to " << address << ":" << port << std::endl;
        close(_socket);
        _connected = false;
        return;
    }

    fcntl(_socket, F_SETFL, O_NONBLOCK);
    _connected = true;
    threadSafeCout << "Connected to " << address << ":" << port << std::endl;
}

void Client::disconnect()
{
    if (_connected)
    {
        close(_socket);
        _connected = false;
    }
}

void Client::defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _actions[messageType] = action;
}

void Client::send(const Message& message) {
    if (!_connected)
    {
        threadSafeCout << "error: Cannot send, client not connected" << std::endl;
        return;
    }

    DataBuffer buffer;
    buffer << message;

    ssize_t sent = ::send(_socket, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);
    if (sent < 0)
        threadSafeCout << "error: Failed to send message" << std::endl;

}

void Client::update()
{
    if (!_connected) return;

}