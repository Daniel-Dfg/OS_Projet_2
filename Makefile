CXX = g++
FLAGS = -std=gnu++17 -Wall -Wextra -O2 -Wpedantic

SRC_DIR = src
CLIENT_DIR = $(SRC_DIR)/chat
SERVER_DIR = $(SRC_DIR)/serveur

CLIENT_SRCS = $(wildcard $(CLIENT_DIR)/*.cpp)
SERVER_SRCS = $(wildcard $(SERVER_DIR)/*.cpp)

CLIENT_TARGET = chat
SERVER_TARGET = serveur-chat
all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET):
	$(CXX) $(FLAGS) $(CLIENT_SRCS) -o $(CLIENT_TARGET)

$(SERVER_TARGET):
	$(CXX) $(FLAGS) $(SERVER_SRCS) -o $(SERVER_TARGET)

clean:
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET)

.PHONY: all clean
