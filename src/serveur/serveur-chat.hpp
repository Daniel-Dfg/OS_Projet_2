#pragma once
#include <queue>
#include <sys/types.h>
#include <poll.h>
#include <unordered_map>
#include "../commons.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <queue>
#include <poll.h>

using std::unordered_map;

class Server {
private:
    static constexpr int MAX_CLIENTS = 1000,
                        MAX_QUEUE = 5,
                        MAX_PSEUDO_SIZE = 30,
                        MAX_MESSAGE_SIZE = 1024,
                        MAX_BUFFER_SIZE = MAX_PSEUDO_SIZE + MAX_MESSAGE_SIZE + 2; // "+2" pour accepter le \0 et l'espace
                                                                                  // entre pseudo et message

    static int server_fd;
    static std::vector<pollfd> poll_fds;
    static unordered_map<string, int> name_to_fd;
    static unordered_map<int, string> fd_to_name;
    static std::queue<Message> message_queue;
    static bool running;

    static void init(int port = 1234);
    static void handleNewConnection();
    static void handleClientMessage(int client_fd);
    static void handleDisconnection(int client_fd);
public:

    static void start(int port);
    static void sendMessage(const string receiver, const Message& msg);
    static void run();
    static void stop();

};
