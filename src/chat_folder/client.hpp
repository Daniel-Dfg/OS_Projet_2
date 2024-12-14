#include <arpa/inet.h>
#include <string>
#include <sys/types.h>
#include <poll.h>
#include "../commons.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <queue>

using std::string;

class Client {
private:
    using FD = int;
    string name;
    FD sock_fd;
    std::thread sender_thread;
    std::thread receiver_thread;
    bool running;

    void senderLoop();

    void receiverLoop();

    void connectToServer();

public:
    std::queue<Message> outgoingMessages;
    std::queue<Message> incomingMessages;

    Client(const string& name_) : name(name_), running(true) {
        connectToServer();
        sender_thread = std::thread(&Client::senderLoop, this);
        receiver_thread = std::thread(&Client::receiverLoop, this);
    }

    void sendMessage(const string& receiver, const string& text);

    void stop();

    ~Client() { stop(); }
};
