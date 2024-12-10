#include "Client.hpp"

#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <string>

// Constructeur : initialise le client et tente de se connecter au serveur
Client::Client(boost::asio::io_context &io_context, const std::string &ip,
               const std::string &port)
    : socket_(io_context) {
  // Crée un endpoint avec l'adresse IP et le port fournis
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ip),
                                          std::stoi(port));

  // Démarre la connexion asynchrone au serveur
  socket_.async_connect(endpoint, [this](const boost::system::error_code &ec) {
    on_connect(ec); // Callback pour gérer l'état de la connexion
  });
}

// Callback exécuté lorsque la tentative de connexion est terminée
void Client::on_connect(const boost::system::error_code &ec) {
  if (!ec) {
    std::cout << "Connecté au serveur !" << std::endl;
    prompt_user(); // Démarre la saisie de messages utilisateur
  } else {
    std::cerr << "Erreur de connexion : " << ec.message() << std::endl;
  }
}

// Envoie un message au serveur de manière asynchrone
void Client::send_message(const std::string &message) {
  boost::asio::async_write(
      socket_,
      boost::asio::buffer(message), // Prépare le message pour l'envoi
      [this](const boost::system::error_code &ec, std::size_t /*length*/) {
        if (ec) {
          std::cerr << "Erreur d'envoi : " << ec.message() << std::endl;
        } else {
          // Après un envoi réussi, redemande un nouveau message à l'utilisateur
          prompt_user();
        }
      });
}

// Demande à l'utilisateur de saisir un message à envoyer
void Client::prompt_user() {
  std::cout << "Entrez un message : " << std::endl;
  std::string message;
  std::getline(std::cin, message); // Lecture de l'entrée utilisateur

  // Si l'utilisateur entre "quit", ferme la connexion
  if (message == "quit") {
    std::cout << "Déconnexion..." << std::endl;
    socket_.close(); // Ferme la socket proprement
    return;
  }

  send_message(message); // Envoie le message au serveur
}
