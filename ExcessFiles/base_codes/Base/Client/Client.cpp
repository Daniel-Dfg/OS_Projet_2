#include "Client.hpp"

#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_context &io_context, const std::string &ip,
               const std::string &port)
    : socket_(io_context) {
  // Crée un endpoint avec l'adresse IP et le port fournis
  tcp::endpoint endpoint(boost::asio::ip::make_address(ip), std::stoi(port));

  // Démarre une connexion asynchrone
  socket_.async_connect(
      endpoint, std::bind(&Client::on_connect, this, std::placeholders::_1));
}

void Client::on_connect(const boost::system::error_code &ec) {
  if (!ec) {
    std::cout << "Connecté au serveur !" << std::endl;

    // Démarre une lecture asynchrone pour recevoir les messages

    // Démarre la saisie utilisateur
    prompt_user();
  } else {
    std::cerr << "Erreur de connexion : " << ec.message() << std::endl;
  }
}

void Client::send_message(const std::string &message) {
  boost::asio::async_write(
      socket_,
      boost::asio::buffer(message +
                          '\n'), // Ajoute un délimiteur '\n' pour les messages
      [this](const boost::system::error_code &ec, std::size_t /*length*/) {
        if (ec) {
          std::cerr << "Erreur d'envoi : " << ec.message() << std::endl;
        } else {
          prompt_user(); // Redemande un message après un envoi réussi
        }
      });
}

void Client::prompt_user() {
  std::cout << "Entrez un message : ";
  std::string message;
  std::getline(std::cin, message); // Lecture de l'entrée utilisateur

  // Si l'utilisateur entre "quit", ferme la connexion
  if (message == "quit") {
    std::cout << "Déconnexion..." << std::endl;
    socket_.close();
    return;
  }

  // Envoie le message
  send_message(message);
}
