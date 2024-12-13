#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP
#include "MessageQueue.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

// Instance d'une session client, s'occupe des envois / récepetion message au
// socket client
class ClientSession : public std::enable_shared_from_this<ClientSession> {
private:
  std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
  MessageQueue incomingMessages; // File de messages entrants
  MessageQueue outgoingMessages; // File de messages sortants
  std::string name_;

  void send_message(); // Envoyer un message au client
  void handle_received_message(const Message &msg);

  // Gérer un message reçu
  void
  read_message(std::unordered_map<std::string, std::shared_ptr<ClientSession>>
                   &clientconnected);

public:
  std::array<char, 1024> buffer_; // Taille fixe pour le buffer de lecture

  ClientSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                const std::string &name)
      : socket_(std::move(socket)), name_(name) {}

  // Aucun traitement supplémentaire n'est nécessaire ici
  // on dois move une deuxième fois pour que ça intègre dans la classe, sinon
  // ce n'est pas compilable
  std::shared_ptr<boost::asio::ip::tcp::socket> &get_socket() {
    return socket_;
  }
  void start(std::unordered_map<std::string, std::shared_ptr<ClientSession>>
                 &connectedclient);
  // Ajouter un message à la file sortante
  void queueMessage(const Message &msg);
  void close_socket() {
    try {
      if (socket_ &&
          socket_
              ->is_open()) { // Vérifie si le pointeur et la socket sont valides
        socket_->close();    // Ferme la socket encapsulée
        std::cout << "Socket fermée pour le client : " << name_ << std::endl;
      }
    } catch (const std::exception &e) {
      std::cerr << "Erreur lors de la fermeture de la socket : " << e.what()
                << std::endl;
    }
  }
};

#endif // CLIENTSESSION_HPP
