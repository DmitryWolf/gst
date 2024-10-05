# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++20 -Wall
LDFLAGS = -lboost_system -pthread -fopenmp

# Исходные файлы
CLIENT_SRC = client.cpp tcp.cpp generator.cpp
SERVER_SRC = server.cpp tcp.cpp generator.cpp

# Исполняемые файлы
CLIENT_EXEC = client
SERVER_EXEC = server

# Цель по умолчанию
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Компиляция клиента
$(CLIENT_EXEC): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_EXEC) $(LDFLAGS)

# Компиляция сервера
$(SERVER_EXEC): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_EXEC) $(LDFLAGS)

# Очистка
clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)
