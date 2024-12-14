#include "Client.hpp"
#include <sstream>

Client::Client(std::string name, bool modManuel, bool modBot, std::string addressIP, int port, std::string realname)
        : name_(name),
          modManuel_(modManuel),
          modBot_(modBot),
          addressIP_(addressIP),
          port_(port),
          realname_(realname) {
    check_return_value(socket_ = socket(AF_INET, SOCK_STREAM, 0)); // Socket TCP-IPv4
    signalManager.initSignalHandling(modManuel_);
    Connect();
}


int Client::Connect() {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_);
    check_return_value(inet_pton(AF_INET, addressIP_.c_str(), &serv_addr.sin_addr),"inet ");
    std::cout<<"Connexion au server ...\n";
    // TODO revoir les cas d'échecs avec l'énoncé et les valeurs de retour
    while (1) {
        if (connect(socket_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) ==0) {
            std::cout<<"Connexion réussi\n";
            break;
        }
        if (errno == ECONNREFUSED) {
            std::cerr << "Connexion refusée par le serveur. Nouvel essai dans 5 secondes\n";
        } else if (errno == ETIMEDOUT) {
            std::cerr << "Le serveur n'a pas répondu : délai maximum dépassé.\n";
            exit(ETIMEDOUT);
        } else {
            perror("Erreur de connexion\n");
            exit(EXIT_FAILURE);
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    connected = true;
    signalManager.clientConnected= true;

    readThread_ = std::thread(&Client::ReceiveMessage, this);
    // Envoyer le nom au serveur
    ssize_t bytes = write(socket_, realname_.c_str(), realname_.length());
    if (bytes<0) { // TODO
        exit(1);
    }
    SendMessage();
    return CODE_RETOUR_NORMAL; // TODO A REVOIR
}

void Client::SendMessage() {
    while (connected) {
        if (signalManager.showMemory) DisplayMemory();
        std::string message;
        getline(std::cin, message);
        size_t space = message.find(' ');
        std::string onlyText = message.substr(space+1);
        if (size(onlyText) < 1024){
            // Utilisation send plutôt que write pour les options(flags), potentiellement nécessaire ultérieurement
            ssize_t bytesSent = send(socket_,message.c_str(),message.size(),0);
            if (bytesSent<0) {
                std::cerr<<"Erreur d'envoi\n";
                connected= false;
            }
        }
    }
}



void Client::ReceiveMessage() {
    if (modManuel_) { // Enoncé : si option manuel : SIGINT doit être vu par thread d'origine et afficher memory
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        pthread_sigmask(SIG_BLOCK, &set, nullptr);
    }
    char buffer[1024];
    while (connected) {
        // Utilisation recv plutôt que read pour les options(flags), potentiellement nécessaire ultérieurement
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
    std::cout<<name << " : "<<message<<std::endl;
}

void Client::DisplayMemory() {}

void Client::Disconnect() {
    if (connected) {
        connected = false;
    }
    if (readThread_.joinable()) readThread_.join();
    close(socket_);
}

Client::~Client() {
    Disconnect();
}