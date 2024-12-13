#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP
#include "ClientSession.hpp"
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// Gère la liste des clients connecté
class ClientManager {
private:
  boost::mutex client_mutex_; // Mutex
  std::unordered_map<std::string,
                     std::shared_ptr<ClientSession>>
      connectedclients; // Associe noms et sockets
public:
  // Ajoute un utilisateur (nom et socket)

  void add_client(const std::string &name,
                  std::shared_ptr<ClientSession> clientsession);
  // Récupère une socket associée à un nom
  std::shared_ptr<boost::asio::ip::tcp::socket>
  get_client_socket(const std::string &name);

  // Supprime un utilisateur
  void remove_client(const std::string &name);
  void print_client_liste() const;
  std::unordered_map<std::string, std::shared_ptr<ClientSession>> &
  get_clients_list() {
    return connectedclients;
  }
};

#endif // CLIENTMANAGER_HPP
