#include "tcp.h"
#include <omp.h>

std::string outputClient = "resources/outputClient.txt";
std::string outputServer = "resources/outputServer.txt";

void test(size_t N = 500, size_t K = 100){
    
    std::vector<std::vector<long long>> vec(N);
    #pragma omp parallel for num_threads(THREADNUM)
    for (size_t i = 0; i < N; ++i) {
        vec[i] = generate_vector(K);
    }

    TCPClient client("127.0.0.1", "8080");
    client.Connect();
    
    client.SendData(vec);  

    std::vector<std::vector<long long>> res = client.GetData();
    // SaveDataWithErase(res, outputClient);
}

int main(int argc, char* argv[]){
    if (argc != 1 && argc != 3 && argc != 4) {
        std::cerr << "Использование: " << argv[0] << " <N> <K>" << std::endl;
        return 1;
    }

    size_t N = 500;
    size_t K = 10;
    size_t cnt = 10;
    if (argc >= 3) {
        try {
            N = std::stoul(argv[1]);
            K = std::stoul(argv[2]);
            if (argc == 4) {
                cnt = std::stoul(argv[3]);
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при парсинге аргументов: " << e.what() << std::endl;
            return 1;
        }
    }
    
    SaveDataWithErase("N=" + std::to_string(N), outputServer);
    SaveData("K=" + std::to_string(K), outputServer);
    for (size_t i = 0; i < cnt; i++) {
        test(N, K);
    }
    std::cout << "Всем пока!" << std::endl;
    return 0;
}