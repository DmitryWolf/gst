#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <boost/asio.hpp>
#include <thread>
#include <random>

namespace TCP{
    using boost::asio::ip::tcp;

    std::string receive_data(tcp::socket& socket) {
        try {
            boost::system::error_code error;

            // Чтение size
            uint32_t message_length = 0;
            boost::asio::read(socket, boost::asio::buffer(&message_length, sizeof(message_length)), error);

            if (error) {
                throw boost::system::system_error(error);
            }

            // Чтение std::string
            std::vector<char> data(message_length);
            boost::asio::read(socket, boost::asio::buffer(data), error);

            if (error && error != boost::asio::error::eof) {
                throw boost::system::system_error(error);
            }

            std::string retData = std::string(data.begin(), data.end());
            std::cout << "Полученные данные:\n" << retData << std::endl;

            return retData;
        } catch (std::exception& e) {
            std::cerr << "Ошибка при чтении данных: " << e.what() << std::endl;
        }
        return "";
    }

    // Отправка данных в две фазы: size и std::string размера size
    void send_data(tcp::socket& socket, const std::string& message) {
        uint32_t message_length = message.size();
        boost::asio::write(socket, boost::asio::buffer(&message_length, sizeof(message_length)));
        boost::asio::write(socket, boost::asio::buffer(message));
        
        std::cout << "Данные отправлены:\n" << message << std::endl;
    }
}