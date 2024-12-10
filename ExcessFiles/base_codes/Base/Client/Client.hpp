#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <string>

class Client {
public:
  // Constructeur
  Client(boost::asio::io_context &io_context, const std::string &ip,
         const std::string &port);

private:
  void
  on_connect(const boost::system::error_code &ec); // Gestion de la connexion

  void send_message(const std::string &message); // Envoi de messages au serveur

  void prompt_user(); // Demande un message à l'utilisateur

  boost::asio::ip::tcp::socket socket_; // Socket pour la communication
};

#endif // CLIENT_HPP
