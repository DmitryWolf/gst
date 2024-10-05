#include "generator.h"
#include <random>
#include <omp.h>
#include <fstream>
#include <string>

#define TYPE_FOR_RAND int

std::vector<long long> generate_vector(std::size_t K) {
    std::vector<long long> data(K);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<TYPE_FOR_RAND> dist(
        std::numeric_limits<TYPE_FOR_RAND>::min(), 
        std::numeric_limits<TYPE_FOR_RAND>::max()
    );
    
    #pragma omp parallel for num_threads(THREADNUM)
    for (size_t i = 0; i < K; ++i) {
        data[i] = dist(gen);
    }
    return data;
}

long long generate_value(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<TYPE_FOR_RAND> dist(std::numeric_limits<TYPE_FOR_RAND>::min(), std::numeric_limits<TYPE_FOR_RAND>::max());
    return dist(gen);
}

std::vector<long long> prefix_sum(const std::vector<long long>& vec) {
    std::vector<long long> result(vec.size());
    if (vec.size() == 0) {
        return result;
    }
    result[0] = vec[0];
    long long sum = vec[0];
    #pragma omp parallel for reduction(+:sum) num_threads(THREADNUM)
    for (size_t i = 1; i < vec.size(); ++i) {
        sum += vec[i];
        result[i] = sum;
        // result[i] = result[i - 1] + vec[i];
    }
    return result;
}

template<typename T>
void SaveData(const T& value, const std::string& road) {
    std::ofstream outFile(road, std::ios::app);
    if (!outFile) {
        std::cerr << "Ошибка при открытии файла: " << road << std::endl;
        return;
    }
    
    outFile << value << std::endl;
    outFile.close();
}

template<typename T>
void SaveData(const std::vector<std::vector<T>>& vec, const std::string& road) {
    std::ofstream file(road, std::ios::app);

    if (!file) {
        std::cerr << "Ошибка при открытии файла для записи: " << road << std::endl;
        return;
    }

    for (const auto& row : vec) {
        for (const auto& value : row) {
            file << value << " ";
        }
        file << std::endl;
    }
    file.close();
}

template<typename T>
void SaveDataWithErase(const T& value, const std::string& road) {
    std::ofstream outFile(road, std::ios::trunc);
    if (!outFile) {
        std::cerr << "Ошибка при открытии файла: " << road << std::endl;
        return;
    }
    
    outFile << value << std::endl;
    outFile.close();
}

template<typename T>
void SaveDataWithErase(const std::vector<std::vector<T>>& vec, const std::string& road) {
    std::ofstream file(road, std::ios::trunc);

    if (!file) {
        std::cerr << "Ошибка при открытии файла для записи: " << road << std::endl;
        return;
    }

    for (const auto& row : vec) {
        for (const auto& value : row) {
            file << value << " ";
        }
        file << std::endl;
    }
    file.close();
}

template void SaveData(const std::vector<std::vector<long long>>& vec, const std::string& road);
template void SaveDataWithErase(const std::vector<std::vector<long long>>& vec, const std::string& road);

template void SaveData(const long long& value, const std::string& road);
template void SaveData(const std::string& value, const std::string& road);
template void SaveDataWithErase(const long long& value, const std::string& road);
template void SaveDataWithErase(const std::string& value, const std::string& road);
template void SaveData(const unsigned long& value, const std::string& road);
template void SaveDataWithErase(const unsigned long& value, const std::string& road);
template void SaveData(const long& value, const std::string& road);
template void SaveDataWithErase(const long& value, const std::string& road);