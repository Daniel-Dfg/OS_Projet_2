# Compiler et linker
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lboost_system

# Fichiers source et exécutables
CLIENT_SRC = client.cpp
SERVER_SRC = server.cpp
CLIENT_EXEC = client
SERVER_EXEC = server

# Règles par défaut
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Compilation du client
$(CLIENT_EXEC): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_EXEC) $(LDFLAGS)

# Compilation du serveur
$(SERVER_EXEC): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_EXEC) $(LDFLAGS)

# Nettoyage des fichiers générés
clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)
