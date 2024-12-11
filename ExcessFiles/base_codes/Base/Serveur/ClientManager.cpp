#include "ClientManager.hpp"

// Ajoute un utilisateur (nom et socket)
void ClientManager::add_client(
    const std::string &name,
    std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
  boost::lock_guard<boost::mutex> lock(client_mutex_);
  connectedclients[name] = socket;
  std::cout << "Client ajouté : " << name << std::endl;
}

void ClientManager::print_client_liste() {
  std::cout << "Liste des clients:" << std::endl;
  for (const auto &[name, socket] : connectedclients) {
    std::cout << "Client name " << name << std::endl;
    std::cout << "Socket :" << socket << std::endl;
    std::cout << "---------------------" << std::endl;
  }
}