
#include "ClientManager.hpp"

// Ajoute un utilisateur (nom et socket) => à changer en ajouter un nom avec une
// instance de clientSession
void ClientManager::add_client(const std::string &name,
                               std::shared_ptr<ClientSession> clientsession) {
  boost::lock_guard<boost::mutex> lock(client_mutex_);
  connectedclients[name] = clientsession;
  std::cout << "Client ajouté : " << name << std::endl;
}

// retourne le socket de l'utilisateur
std::shared_ptr<boost::asio::ip::tcp::socket>
ClientManager::get_client_socket(const std::string &name) {
  boost::lock_guard<boost::mutex> lock(client_mutex_);

  auto pair = connectedclients.find(name); // Chercher le client

  if (pair != connectedclients.end()) { // Vérifier s'il existe
    std::cout << "Client : " << name << " Existe" << std::endl;
    return pair->second->get_socket(); // Retourner la socket associée
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
    pair->second->close_socket();       // ferme la socket
    std::cout << "Socket supprimé de la liste : " << name << std::endl;
    connectedclients.erase(pair); // efface la pair
    std::cout << "Client supprimé de la liste : " << name << std::endl;
  }
}

// Imprime la liste des clients
void ClientManager::print_client_liste() const {
  std::cout << "Liste des clients:" << std::endl;
  std::cout << "---------------------" << std::endl;
  for (const auto &[name, clientsession] : connectedclients) {
    std::cout << "Client name " << name << std::endl;
    std::cout << "Socket :" << clientsession->get_socket() << std::endl;
    std::cout << "---------------------" << std::endl;
  }
}