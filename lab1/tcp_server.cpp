#include "tcp.h"


using boost::asio::ip::tcp;

// Функция для генерации случайного вектора из K элементов
std::vector<int> generate_vector(size_t K) {
    std::vector<int> data(K);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);
    
    for (size_t i = 0; i < K; ++i) {
        data[i] = dist(gen);
    }
    return data;
}

// Обработчик клиента
void handle_client(tcp::socket socket, size_t N, size_t K) {
    try {
        // Время начала работы
        auto start = std::chrono::high_resolution_clock::now();
        
        // Генерация N векторов по K элементов
        {
            std::string message;
            for (size_t i = 0; i < N; ++i) {
                std::vector<int> data = generate_vector(K);
                for (int value : data) {
                    message += std::to_string(value) + " ";
                }
                message += "\n";
            }

            // Отправка данных клиенту
            TCP::send_data(socket, message);
        }
        
        // Ожидание результата от клиента
        std::string result = TCP::receive_data(socket);
        
        // Время конца работы
        auto end = std::chrono::high_resolution_clock::now();

        // Сохранение результата в файл
        {
            std::chrono::duration<double> duration = end - start;

            // std::ofstream output_file("results.txt", std::ios::app); // Открытие файла в режиме добавления
            std::ofstream output_file("results.txt", std::ios::trunc); // Открытие файла в режиме перезаписи
            if (output_file.is_open()) {
                output_file << result << "\nN = " << N << "\nK = " << K << "\nВремя выполнения: " << duration.count() << " секунд" << std::endl; // Запись результата
                output_file.close();
                std::cout << "Результат сохранен в файл results.txt" << std::endl;
            } else {
                std::cerr << "Не удалось открыть файл для записи" << std::endl;
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Ошибка при работе с клиентом: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        // Сервер прослушивает на порту 8080
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        size_t N = 10;  // Количество векторов
        size_t K = 5;  // Количество элементов в каждом векторе

        std::cout << "Сервер запущен и слушает на порту 8080..." << std::endl;

        while (true) {
            // Ожидание подключений
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            // Обработка клиента в отдельном потоке
            std::thread(handle_client, std::move(socket), N, K).detach();
        }

    } catch (std::exception& e) {
        std::cerr << "Ошибка сервера: " << e.what() << std::endl;
    }

    return 0;
}
