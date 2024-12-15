#include "Client.hpp"
#include <sstream>
#include <algorithm>


Client::Client(std::string name, bool modManuel, bool modBot, std::string addressIP, int port)
        : name_(name),
          modManuel_(modManuel),
          modBot_(modBot),
          addressIP_(addressIP),
          port_(port) {
    check_return_value(socket_ = socket(AF_INET, SOCK_STREAM, 0)); // créer socket TCP-IPv4
    signalManager.initSignalHandling(modManuel_); // Mise en place du gestionnaire de signaux
    mainThreadID = pthread_self();
    Connect(); // Connexion au serveur
}


int Client::Connect() {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_);
    check_return_value(inet_pton(AF_INET, addressIP_.c_str(), &serv_addr.sin_addr), "inet ");

    // TODO revoir les cas d'échecs avec l'énoncé et les valeurs de retour
    std::cout << "Connexion au server ...\n";
    while (1) {
        if (connect(socket_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0) {
            std::cout << "Connexion réussi\n";
            break;
        }
        if (errno == ECONNREFUSED) {
            std::cerr << "Connexion refusée par le serveur. Attente en cours.\n";
        } else if (errno == ETIMEDOUT) {
            std::cerr << "Le serveur n'a pas répondu : délai maximum dépassé.\n";
            exit(ETIMEDOUT);
        } else {
            perror("Erreur de connexion\n");
            exit(EXIT_FAILURE);
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    readThread_ = std::thread(&Client::ReceiveMessage, this); // Lancement du 2e thread
    connected = true;
    signalManager.clientConnected = true;

    // Envoyer le nom au serveur
    std::string nametosend = name_;
    std::replace(nametosend.begin(), nametosend.end(), ' ', '-');
    ssize_t bytes = write(socket_, nametosend.c_str(), nametosend.length());

    SetNickName(name_);

    if (bytes < 0) { // TODO
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
        std::string onlyText = message.substr(space + 1);
        // TODO A VERIFIER
        /*
        if (std::cin.eof()) {
            connected = false;
            break;
        }*/
        if (std::cin.fail()) { // Ctrl+C
            std::cin.clear();
        }
        if (size(onlyText) < 1024) { // Si le message ne dépasse pas les 1024 octets
            // Utilisation send plutôt que write pour les options(flags), potentiellement nécessaire ultérieurement
            if (!modBot_) std::cout << name_ << " : " << onlyText << "\n";
            ssize_t bytesSent = send(socket_, message.c_str(), message.size(), 0);
            if (bytesSent < 0) {
                std::cerr << "Erreur d'envoi\n";
                connected = false;
            }
        }
    }
}


void Client::ReceiveMessage() {
    char buffer[1024];
    // Bloque le signal SIGINT dans le readThread_
    if (modManuel_) { // Enoncé : si option manuel : SIGINT doit être vu par thread d'origine et afficher memory
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        pthread_sigmask(SIG_BLOCK, &set, nullptr);
    }

    // Utilisation recv plutôt que read pour les options(flags), potentiellement nécessaire ultérieurement
    while (connected) {
        ssize_t bytesReceived;
        bytesReceived = recv(socket_, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived > 0) { // Message bien reçu
            buffer[bytesReceived] = '\0';
            if (modManuel_) {
                std::cout << "\a" << std::endl; //std::cout<<(strlen(buffer)+size(memory_))<<std::endl;

                if ((strlen(buffer) + size(memory_)) < MAX_MEMOIRE) { // Ajouter à la mémoire
                    memory_ += buffer;
                    std::memset(buffer, 0, strlen(buffer)); // Nettoyer le buffer
                } else { // Mémoire remplie := Afficher
                    DisplayMemory();
                    DisplayMessage(buffer);
                }

            } else { // Cas classique sans mode manuel
                DisplayMessage(buffer);
            }
        } else if (bytesReceived == 0) { // Déconnecté du serveur
            connected = false;
            pthread_kill(mainThreadID, SIGINT);
        } else { // Erreur
            std::cerr << "erreur"; // TODO
        }
    }
}


void Client::DisplayMessage(const char *buffer) {
    std::string name;
    std::string message;

    std::string receivedMessage(buffer);
    std::istringstream stream(receivedMessage);
    stream >> name;
    std::replace(name.begin(), name.end(), '-', ' ');
    SetNickName(name);
    std::getline(stream, message);
    std::cout << name << " : " << message << std::endl;
}


void Client::DisplayMemory() {
    std::istringstream stream(memory_); // Crée un flux pour lire la mémoire
    std::string singleMessage;

    while (std::getline(stream, singleMessage)) {
        DisplayMessage(singleMessage.c_str());
    }
    memory_ = "";
    signalManager.showMemory = false;
}


void Client::SetNickName(std::string &nick) {
    if (modBot_) nick = "[" + std::string(nick) + "]";
    else nick = "\x1B[4m[" + std::string(nick) + "]\x1B[0m";
}


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