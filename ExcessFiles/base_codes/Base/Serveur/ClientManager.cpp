
#include "ClientManager.hpp"

// Ajoute un utilisateur (nom et socket)
void ClientManager::add_client(
    const std::string &name,
    std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
  boost::lock_guard<boost::mutex> lock(client_mutex_);
  connectedclients[name] = socket;
  std::cout << "Client ajouté : " << name << std::endl;
}

// retourne le socket de l'utilisateur
std::shared_ptr<boost::asio::ip::tcp::socket>
ClientManager::get_client_socket(const std::string &name) {
  boost::lock_guard<boost::mutex> lock(client_mutex_);

  auto pair = connectedclients.find(name); // Chercher le client

  if (pair != connectedclients.end()) { // Vérifier s'il existe
    std::cout << "Client : " << name << " Existe" << std::endl;
    return pair->second; // Retourner la socket associée
  } else {
    std::cout << "Client : " << name << " N'existe pas" << std::endl;
    return nullptr; // Client introuvable
  }
}

// Supprime un utilisateur
void ClientManager::remove_client(const std::string &name) {
  boost::lock_guard<boost::mutex> lock(client_mutex_);
  auto pair = connectedclients.find(name);
  if (pair != connectedclients.end()) { // si la paire existe
    connectedclients.erase(pair);
    std::cout << "Client supprimé de la liste : " << name << std::endl;
  }
}

// Imprime la liste des clients
void ClientManager::print_client_liste() {
  std::cout << "Liste des clients:" << std::endl;
  std::cout << "---------------------" << std::endl;
  for (const auto &[name, socket] : connectedclients) {
    std::cout << "Client name " << name << std::endl;
    std::cout << "Socket :" << socket << std::endl;
    std::cout << "---------------------" << std::endl;
  }
}