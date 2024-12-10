#ifndef SERVEUR_HPP
#define SERVEUR_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <string>

// La classe SERVEUR gère la communication entre deux utilisateurs via des pipes
class Serveur {
private:
  std::string sender, receiver; // Les noms des utilisateurs
  boost::asio::io_context
      io_context; // Contexte I/O pour les opérations asynchrones
  boost::asio::thread_pool
      threadPool; // Pool de threads pour gérer les connexions
  std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
  const unsigned short int nbdethreads; // Nombre de threads dans le pool

  // Méthodes privées
  void createSocket(int port); // Crée une socket principale
  void acceptConnections(boost::asio::ip::tcp::acceptor &acceptor,
                         std::shared_ptr<boost::asio::ip::tcp::socket>
                             socket); // Gère les connexions entrantes
public:
  bool running = true;

  // Constructeur
  Serveur(std::string sender, std::string receiver, int nbdethreads)
      : sender(std::move(sender)), receiver(std::move(receiver)), io_context(),
        threadPool(nbdethreads), nbdethreads(nbdethreads), running(true) {}
  boost::asio::io_context &getIoContext() { return io_context; }
  // Méthodes publiques
  void
  submitToPool(boost::asio::ip::tcp::socket
                   socket); // Envoie la gestion d'un client dans le thread pool
  int start(bool modBot, bool modManuel); // Démarre le serveur
  void
  handleClient(boost::asio::ip::tcp::socket socket); // Gère un client connecté
  void startIoContextWithThreadPool();               // Lance le contexte I/O
  void stop(); // Arrête le serveur proprement
};

#endif // SERVEUR_HPP
