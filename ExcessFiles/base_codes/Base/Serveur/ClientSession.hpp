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
  boost::asio::ip::tcp::socket socket_;
  MessageQueue incomingMessages; // File de messages entrants
  MessageQueue outgoingMessages; // File de messages sortants
  std::string name_;

  void read_message(); // Lire les messages du client
  void send_message(); // Envoyer un message au client
  void handle_received_message(const Message &msg); // Gérer un message reçu

public:
  ClientSession(boost::asio::ip::tcp::socket socket, const std::string &name)
      : socket_(std::move(socket)), name_(name) {}

  void start();
  // Ajouter un message à la file sortante
  void queueMessage(const Message &msg);
};

#endif // CLIENTSESSION_HPP
