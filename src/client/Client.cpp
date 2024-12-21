#include "Client.hpp"
#include <algorithm>
#include <sstream>


Client::Client(std::string name, bool modManuel, bool modBot,
               std::string addressIP, int port)
    : name_(name), modManuel_(modManuel), modBot_(modBot),
      addressIP_(addressIP), port_(port) {

  check_return_value(socket_ = socket(AF_INET, SOCK_STREAM, 0), "socket"); // créer socket TCP-IPv4
  signalManager.initSignalHandling(modManuel_); // Mise en place du gestionnaire de signaux
  mainThreadID = pthread_self();
  Connect(); // Connexion au serveur
}

void Client::Connect() {
    /**
     * @brief Configure et établit une connexion avec le serveur
     */
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port_);
  check_return_value(
      inet_pton(AF_INET, addressIP_.c_str(), &serv_addr.sin_addr), "inet ");

  std::cout << "Connexion au serveur ...\n";
  while (true) {
    if (connect(socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==
        0) {
      std::cout << "Connexion réussie\n";
      break;
    }
    if (errno == ECONNREFUSED) {
      std::cerr << "Connexion refusée par le serveur. Nouvelle tentative...\n";
    } else if (errno == ETIMEDOUT) {
      std::cerr << "Le serveur n'a pas répondu : délai maximum dépassé.\n";
      exit(ETIMEDOUT);
    } else {
      perror("Erreur de connexion\n");
      exit(EXIT_FAILURE);
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
  readThread_ = std::thread(&Client::ReceiveMessage, this); // Lancement du 2e thread (de réception des messages)
  connected = true;
  signalManager.clientConnected = true;

  // Envoyer le nom au serveur
  nameClassic_ = name_;
  std::replace(nameClassic_.begin(), nameClassic_.end(), ' ', '-');
  check_return_value(
      write(socket_, nameClassic_.c_str(), nameClassic_.length()), "write");
  FormatAndSetNickName(name_);

  SendMessage();
}


void Client::SendMessage() {
  size_t space;
  std::string message;
  while (connected) {
    if (signalManager.showMemory)
      DisplayMemory();
    message.clear();
    getline(std::cin, message);

    if (std::cin.fail()) {
      if (signalManager.showMemory)
        std::cin.clear(); // Ctrl+C declenché
      else {
        Disconnect();
        break;
      }
    }
    space = message.find(' ');
    std::string receiver = message.substr(0, space);
    std::string onlyText = message.substr(space + 1);

    if (receiver.empty() || onlyText.empty()) {
      continue;
    }

    // Si le message ne dépasse pas les 1024 octets et contient au minimum un
    // espace
    if (size(onlyText) < 1024 && space != std::string::npos) {
      if (receiver == nameClassic_) {
        std::cerr << "Vous ne pouvez pas envoyer un message à vous même\n";
        continue;
      }
      if (!modBot_) {
        std::lock_guard<std::mutex> lock(displayMutex);
        std::cout << name_ << " : " << onlyText << "\n";
      }
      DisplayMemory();
      // Utilisation send plutôt que write pour les options(flags),
      // potentiellement nécessaire ultérieurement
      ssize_t bytesSent = send(socket_, message.c_str(), message.size(), 0);
      if (bytesSent < 0) {
        std::cerr << "Erreur d'envoi\n";
        exit(ERREUR_AUTRE);
      }
    }
  }
}

// Réception des messages du serveur
void Client::ReceiveMessage() {
  char buffer[1024] = {0};
  // Bloque le signal SIGINT dans le readThread_
  if (modManuel_) { // Enoncé : si option manuel : SIGINT doit être vu par
                    // thread d'origine et afficher memory
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, nullptr);
  }
  // Utilisation recv plutôt que read pour les options(flags), potentiellement
  // nécessaire ultérieurement
  while (connected) {
    ssize_t bytesReceived;
    bytesReceived = recv(socket_, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) { // Message bien reçu
      if (std::strchr(buffer, ' ') ==
          nullptr) { // Un seul mot = Utilisateur non connecté
        DisplayMessage(buffer);
        continue;
      }

      buffer[bytesReceived] = '\n';
      buffer[bytesReceived + 1] = '\0';
      if (modManuel_) {
        std::cout << "\a";
        std::flush(std::cout);

        if ((strlen(buffer) + size(memory_)) <
            MAX_MEMOIRE) { // Ajouter à la mémoire
          std::lock_guard<std::mutex> lock(memoryMutex);
          memory_ += buffer;
          std::memset(buffer, 0, bytesReceived); // Nettoyer le buffer
        } else {                                 // Mémoire remplie := Afficher
          DisplayMemory();
          DisplayMessage(buffer);
        }
      } else { //!modManuel
        DisplayMessage(buffer);
      }
    } else if (bytesReceived == 0) { // Déconnecter du serveur
      connected = false;
      pthread_kill(mainThreadID, SIGINT);
    } else { // Erreur
      std::cerr << "Erreur réception de message";
      exit(ERREUR_AUTRE);
    }
  }
}

void Client::DisplayMessage(const char *buffer) {
    /**
     * @brief Affiche un message en terminal (stdout/stderr) et clear le buffer
     */
  std::lock_guard<std::mutex> lock(displayMutex);
  if (std::strchr(buffer, ' ') == nullptr) {
    std::cerr << "Cette personne (" << buffer << ") n'est pas connectée.\n";
    return;
  }

  std::string name;
  std::string message;

  std::string receivedMessage(buffer);
  std::istringstream stream(receivedMessage);
  stream >> name;
  std::replace(name.begin(), name.end(), '-', ' ');
  FormatAndSetNickName(name);
  std::getline(stream, message);
  std::cout << name << " :" << message << std::endl;
  memset(&buffer, 0, sizeof(buffer));
}

void Client::DisplayMemory() {
    /**
    * @brief Affiche la mémoire en mode manuel
    */
  std::lock_guard<std::mutex> lock(memoryMutex);
  std::istringstream stream(memory_); // Crée un flux pour lire la mémoire
  std::string singleMessage;

  while (std::getline(stream, singleMessage)) {
    DisplayMessage(singleMessage.c_str());
  }
  memory_.clear();
  signalManager.showMemory = false;
}


void Client::FormatAndSetNickName(std::string &nick) {
  if (modBot_)
    nick = "[" + std::string(nick) + "]";
  else
    nick = "[\x1B[4m" + std::string(nick) + "\x1B[0m]";
}


void Client::Disconnect() {
  if (connected) {
    connected = false;
    if (shutdown(socket_, SHUT_RDWR) < 0) {
      std::cerr << "Erreur shutdown\n";
    }
    if (close(socket_) < 0) {
      std::cerr << "Erreur lors de la fermeture de la socket\n";
    }
  }
  if (readThread_.joinable())
    readThread_.join();
}

Client::~Client() { Disconnect(); }
