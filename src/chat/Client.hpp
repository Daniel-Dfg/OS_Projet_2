#ifndef PROJETOS_CLIENT_HPP
#define PROJETOS_CLIENT_HPP
#include "commons.hpp"
#include <thread>
#include <atomic>
#include "SignalManager.hpp"

class Client {
public:
    Client(std::string name_,bool modManuel,bool modBot,std::string addressIp, int port);
    int Connect();
    void SendMessage();
    void ReceiveMessage();
    void DisplayMessage(const char* message);
    void DisplayMemory();
    void Disconnect();
    ~Client();
private:
    std::string name_;
    bool modManuel_;
    bool modBot_;

    std::string addressIP_;
    int port_;
    int socket_; // Socket TCP-IPv4
    struct sockaddr_in serv_addr_;

    std::thread reader_;
    std::thread readThread_;
    std::atomic<bool> connected{false};
    SignalManager signalManager;

};


#endif //PROJETOS_CLIENT_HPP
