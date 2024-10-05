#pragma once

#include <iostream>
#include <vector>

#define THREADNUM 8

std::vector<long long> generate_vector(std::size_t K);

long long generate_value();

std::vector<long long> prefix_sum(const std::vector<long long>& vec);

template<typename T>
void SaveData(const std::vector<std::vector<T>>& vec, const std::string& road);

template<typename T>
void SaveDataWithErase(const std::vector<std::vector<T>>& vec, const std::string& road);

template<typename T>
void SaveData(const T& value, const std::string& road);

template<typename T>
void SaveDataWithErase(const T& value, const std::string& road);
