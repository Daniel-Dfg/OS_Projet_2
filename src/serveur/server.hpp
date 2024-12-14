// Ne pas oublier de brancher le client au bon port, et de faire ./Client USERNAME, et que le client envoie directement son username
// juste après la connection
#pragma once
#include <queue>
#include <string>
#include <sys/types.h>
#include <poll.h>
#include <unordered_map>
#include "../commons.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring> // Pour strlen

//#include <signal.h>

using std::string, std::unordered_map;

class Server {
private:
    static constexpr int MAX_CLIENTS = 1000;

    static int server_fd;
    static std::vector<pollfd> poll_fds;
    static unordered_map<const char *, int> name_to_fd;
    static unordered_map<int, const char *> fd_to_name;
    static std::queue<Message> message_queue;
    static bool running;

    static void init(int port = 8080);
    static void handleNewConnection();
    static void handleClientMessage(int client_fd);
    static void handleDisconnection(int client_fd);
public:

    static void start(int port);
    static void sendMessage(const char* receiver, const Message& msg);
    static void run();
    static void stop();
};
