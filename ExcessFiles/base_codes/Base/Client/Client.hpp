#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
#include <string>

// Définition de la classe Client
class Client {
public:
  // Constructeur : Initialise la socket et tente de se connecter au serveur
  Client(boost::asio::io_context &io_context, const std::string &ip,
         const std::string &port);

private:
  // Callback exécuté lorsque la connexion est terminée
  void on_connect(const boost::system::error_code &ec);

  // Envoie un message au serveur de manière asynchrone
  void send_message(const std::string &message);

  // Démarre une lecture asynchrone pour recevoir les messages du serveur
  void start_reading();

  // Callback exécuté lorsqu'un message est reçu
  void on_read(const boost::system::error_code &ec, std::size_t length);

  // Demande à l'utilisateur de saisir un message à envoyer
  void prompt_user();

  // Socket utilisée pour la communication avec le serveur
  boost::asio::ip::tcp::socket socket_;

  // Tampon pour stocker les données reçues
  std::string buffer_;
};

#endif // CLIENT_HPP
