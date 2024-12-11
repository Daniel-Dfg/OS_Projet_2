#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class ClientManager {
private:
  boost::mutex client_mutex_; // Mutex pour synchronisation thread-safe
  std::unordered_map<std::string, std::shared_ptr<boost::asio::ip::tcp::socket>>
      connectedclients; // Associe noms et sockets
public:
  ClientManager() = default; // Constructeur par défaut
  // Ajoute un utilisateur (nom et socket)
  void add_client(const std::string &name,
                  std::shared_ptr<boost::asio::ip::tcp::socket> socket);

  // Récupère une socket associée à un nom
  std::shared_ptr<boost::asio::ip::tcp::socket>
  get_client_socket(const std::string &name);

  // Vérifie si un nom d'utilisateur existe
  bool client_exists(const std::string &name);

  // Supprime un utilisateur
  void remove_client(const std::string &name);
  void print_client_liste();
};

#endif // CLIENTMANAGER_HPP
