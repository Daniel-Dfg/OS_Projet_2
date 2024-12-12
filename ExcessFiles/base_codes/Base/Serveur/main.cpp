#include <boost/asio/signal_set.hpp>

#include "Serveur.hpp"

int main() {
  try {
    std::string sender = "Serveur";
    std::string receiver = "Client"; // Dois retirer sender receiver et
                                     // uniquement garder nombre de threads

    Serveur serveur(sender, receiver, 4);

    // Gestion des signaux pour arrêter le serveur avec Ctrl+C (ça ne marche
    // pas)
    boost::asio::signal_set signals(serveur.getIoContext(), SIGINT, SIGTERM);
    signals.async_wait([&serveur](boost::system::error_code /*ec*/,
                                  int /*signal*/) { serveur.stop(); });

    serveur.start(false, false); // supprimé à mettre dans les ClientSession

  } catch (const std::exception &e) {
    std::cerr << "Erreur : " << e.what() << std::endl;
  }

  return 0;
}
