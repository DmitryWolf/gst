#include "tcp.h"
#include <omp.h>
#include <chrono>

/*
==================================================
===================TCPRequests====================
==================================================
*/

TCPRequests::TCPRequests()
    : io_context_(), socket_(io_context_) {}


std::vector<std::vector<long long>> TCPRequests::receive_data(tcp::socket& socket) {
    try {
        boost::system::error_code error;

        size_t N = 0;
        size_t K = 0;

        boost::asio::read(socket, boost::asio::buffer(&N, sizeof(N)), error);
        boost::asio::read(socket, boost::asio::buffer(&K, sizeof(K)), error);
        // boost::asio::read(socket, boost::asio::buffer(&message_length, sizeof(message_length)), error);

        if (error) {
            throw boost::system::system_error(error);
        }

        // Чтение данных на основе длины
        std::vector<std::vector<long long>> data(N, std::vector<long long>(K));
        for (size_t i = 0; i < N; ++i) {
            boost::asio::read(socket, boost::asio::buffer(data[i].data(), data[i].size() * sizeof(long long)), error);
        }
        
        if (error && error != boost::asio::error::eof) {
            throw boost::system::system_error(error);
        }

        return data;
    } catch (std::exception& e) {
        std::cerr << "Ошибка при чтении данных: " << e.what() << std::endl;
    }
    return {};
}

void TCPRequests::send_data(tcp::socket& socket, const std::vector<std::vector<long long>>& message) {
    size_t N = message.size();
    size_t K = N == 0 ? 0 : message[0].size();
    boost::asio::write(socket, boost::asio::buffer(&N, sizeof(N)));
    boost::asio::write(socket, boost::asio::buffer(&K, sizeof(K)));
    for (size_t i = 0; i < N; ++i) {
        boost::asio::write(socket, boost::asio::buffer(message[i]));
    }
}

/*
==================================================
====================TCPServer=====================
==================================================
*/

TCPServer::TCPServer() : 
    acceptor_(io_context_, tcp::endpoint(tcp::v4(), 8080))
{}

void TCPServer::RunServer(){
    std::cout << "Сервер запущен и слушает на порту 8080" << std::endl;
    StartAccept();
}


void TCPServer::StartAccept(){
    while (true) {
        socket_ = tcp::socket(io_context_);
        acceptor_.accept(socket_);
        std::thread([this, socket = std::move(socket_)]() mutable { 
            HandleClient(std::move(socket));
        }).detach();
    }
}



void TCPServer::HandleClient(tcp::socket socket){
    std::vector<std::vector<long long>> data = receive_data(socket);
    std::vector<std::vector<long long>> prefsum(data.size());

    auto start = std::chrono::high_resolution_clock::now();
    
    #pragma omp parallel for num_threads(THREADNUM)
    for (size_t i = 0; i < data.size(); ++i) {
        prefsum[i] = prefix_sum(data[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    send_data(socket, prefsum);
    // std::cout << "Отослал данные ^_^" << std::endl << "Время выполнения: " << duration << " мс\n" << std::endl;
    SaveData(duration, "resources/outputServer.txt");
}



/*
==================================================
====================TCPClient=====================
==================================================
*/

TCPClient::TCPClient(std::string ip, std::string port) :
    resolver_(io_context_), ip_(std::move(ip)), port_(std::move(port)), isConnected(false)
{}

void TCPClient::Connect(){
    tcp::resolver::results_type endpoints = resolver_.resolve(ip_, port_);
    boost::asio::connect(socket_, endpoints);    
}

std::vector<std::vector<long long>> TCPClient::GetData(){
    return receive_data(socket_);
}

void TCPClient::SendData(const std::vector<std::vector<long long>>& data){
    send_data(socket_, std::move(data));
}
