#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <boost/asio.hpp>
#include <thread>
#include <random>
#include <functional>
#include "generator.h"

using boost::asio::ip::tcp;

class TCPRequests{
protected:
    TCPRequests();

    std::vector<std::vector<long long>> receive_data(tcp::socket& socket);

    void send_data(tcp::socket& socket, const std::vector<std::vector<long long>>& message);

    boost::asio::io_context io_context_;
    tcp::socket socket_;
};

class TCPServer : public TCPRequests {
public:
    TCPServer();

    void RunServer();
    
private:
    tcp::acceptor acceptor_;

    void StartAccept();

    void HandleClient(tcp::socket socket);
};

class TCPClient : public TCPRequests {
public:
    TCPClient() = delete;

    TCPClient(std::string ip, std::string port);

    void Connect();

    void SendData(const std::vector<std::vector<long long>>& data);

    std::vector<std::vector<long long>> GetData();

private:
    tcp::resolver resolver_;
    std::string ip_;
    std::string port_;
    bool isConnected;
};