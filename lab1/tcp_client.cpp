#include "tcp.h"

using boost::asio::ip::tcp;

// Функция для вычисления префиксной суммы для одного вектора
std::vector<int> prefix_sum(const std::vector<int>& vec) {
    std::vector<int> result(vec.size());
    result[0] = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        result[i] = result[i - 1] + vec[i];
    }
    return result;
}

// Функция для разбора строки в несколько векторов
std::vector<std::vector<int>> parse_vectors(const std::string& data) {
    std::istringstream iss(data);
    size_t n = 0;
    for (const char& c : data) {
        if (c == '\n') {
            n++;
        }
    }

    int value;
    std::vector<int> tempVector;
    while (iss >> value) {
        tempVector.push_back(value);
    }
    size_t k = tempVector.size() / n;

    std::vector<std::vector<int>> vectors(n, std::vector<int>(k));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < k; ++j){
            vectors[i][j] = tempVector[i * k + j];
        }
    }
    return vectors;
}

int main() {
    try {
        boost::asio::io_context io_context;

        // Подключение клиента к серверу
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "8080");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::string data = TCP::receive_data(socket);

        // Разбор полученной строки в векторы
        std::vector<std::vector<int>> vectors = parse_vectors(data);

        size_t n = vectors.size();
        size_t k = n > 0 ? vectors[0].size() : 0;

        // Вычисление префиксной суммы для каждого вектора
        std::vector<std::vector<int>> result_vectors(n);
        for (size_t i = 0; i < n; ++i) {
            result_vectors[i] = prefix_sum(vectors[i]);
        }

        // Подготовка результата для отправки на сервер
        std::string result_message;
        for (const auto& vec : result_vectors) {
            for (int v : vec) {
                result_message += std::to_string(v) + " ";
            }
            result_message += "\n";
        }

        // Отправка результата на сервер
        TCP::send_data(socket, result_message);
    } catch (std::exception& e) {
        std::cerr << "Ошибка клиента: " << e.what() << std::endl;
    }

    return 0;
}
