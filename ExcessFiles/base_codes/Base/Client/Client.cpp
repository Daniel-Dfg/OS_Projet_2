
// Rien changer partie d'Haluk
#include "Client.hpp"
using boost::asio::ip::tcp;

Client::Client(std::string &name_, boost::asio::io_context &io_context,
               const std::string &ip, const std::string &port)
    : socket_(io_context), io_context_(io_context), name_(name_) {
  tcp::endpoint endpoint(boost::asio::ip::make_address(ip), std::stoi(port));

  socket_.async_connect(endpoint,
                        [this](const boost::system::error_code &errorCode) {
                          tryConnect(errorCode);
                        });
}

void Client::run() {
  std::thread readThread([this]() { io_context_.run(); });
  userInput();
  readThread.join();
}

void Client::tryConnect(const boost::system::error_code &errorCode) {
  if (!errorCode) {
    std::cout << "Connexion réussi " << std::endl;
    readMessages();
  } else {
    std::cerr << "Erreur de connexion " << errorCode.message() << std::endl;
    ;
  }
}

void Client::readMessages() {
  boost::asio::async_read_until(
      socket_, boost::asio::dynamic_buffer(buffer_), '\n',
      [this](const boost::system::error_code &errorReading,
             std::size_t length) {
        if (!errorReading) {
          // Récupérer le message complet du buffer
          std::string message = buffer_.substr(0, length);
          buffer_.erase(0, length); // Supprime le message du buffer

          // Partie ajouter pour tout prendre tout avant le premier mot avant
          // l'espace , je ne pense pas du tout ce que la technique à faire mais
          // c'est pour allée rapidement, le temps de faire les fonctions adapté
          // pour le nettoyage des textes, faire de même avoir les messages
          // qu'on envoie nous
          std::istringstream iss(message);
          std::string sender;
          iss >> sender;

          // Afficher le message
          std::cout << sender << " : " << iss.str().substr(sender.size())
                    << std::endl;

          // Relancer la lecture asynchrone
          readMessages();
        } else {
          std::cerr << "Erreur lecture de message: " << errorReading.message()
                    << std::endl;
        }
      });
}

void Client::sendMessages(const std::string &message) {
  // devras s'occuper de faire le bon format aussi ici
  boost::asio::async_write(
      socket_, boost::asio::buffer(message + "\n"),
      [this, message](const boost::system::error_code &errorSending,
                      std::size_t /*length*/) {
        if (!errorSending) {
          std::cout << message << std::endl;
        }
        if (errorSending) {
          std::cerr << "Erreur envoie message: " << errorSending.message()
                    << std::endl;
        }
      });
}

void Client::userInput() {
  while (running) {
    std::string message;
    std::getline(std::cin, message);

    if (message == "quit") {
      io_context_.stop();
      running = false;
    }
    sendMessages(message);
  }
}