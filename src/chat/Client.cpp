#include "Client.hpp"
#include <sstream>

Client::Client(std::string name, bool modManuel, bool modBot, std::string addressIP, int port) : name_(name),
                                                                                                 modManuel_(modManuel),
                                                                                                 modBot_(modBot),
                                                                                                 addressIP_(addressIP),
                                                                                                 port_(port) {
    check_return_value(socket_ = socket(AF_INET, SOCK_STREAM, 0)); // Socket TCP-IPv4
    signalManager.initSignalHandling(modManuel_);
    Connect();
}


int Client::Connect() {
    serv_addr_.sin_family = AF_INET;
    serv_addr_.sin_port = htons(port_);
    check_return_value(inet_pton(AF_INET, addressIP_.c_str(), &serv_addr_.sin_addr));
    check_return_value(connect(socket_, (struct sockaddr *) &serv_addr_, sizeof(serv_addr_)));

    connected = true;
    signalManager.clientConnected= true;

    readThread_ = std::thread(&Client::ReceiveMessage, this);
    SendMessage();
    return CODE_RETOUR_NORMAL;
}

void Client::SendMessage() {
    while (connected) {
        if (signalManager.showMemory) DisplayMemory();
        std::string message;
        std::cout<<name_<<" : ";
        getline(std::cin, message);
        ssize_t bytesSent = send(socket_,message.c_str(),message.size(),0);
        if (bytesSent<0) {
            std::cerr<<"error";
        }
    }
}

void Client::ReceiveMessage() {
    if (modManuel_) { // Enoncé : si option manuel SIGINT dans le thread principale affiche
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        pthread_sigmask(SIG_BLOCK, &set, nullptr);
    }
    char buffer[1054];
    while (connected) {
        ssize_t bytesReceived = recv(socket_, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived>0) {
            buffer[bytesReceived] ='\0';
            DisplayMessage(buffer);
        } else if (bytesReceived == 0) {
            connected= false;
        } else {
            std::cerr<<"erreur";
        }
    }
}

void Client::DisplayMessage(const char* buffer) {
    std::string name;
    std::string message;

    std::string receivedMessage(buffer);
    std::istringstream stream(receivedMessage);
    stream >> name;
    std::getline(stream,message);
    std::cout<<name<< " : "<<message<<std::endl;
}

void Client::DisplayMemory() {}

void Client::Disconnect() {
    if (connected) {
        connected = false;
        if (readThread_.joinable()) readThread_.join();
        close(socket_);
    }
}

Client::~Client() {
    Disconnect();
}