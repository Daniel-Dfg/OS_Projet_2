#include <boost/asio.hpp>
#include <iostream>

#include "Client.hpp"

int main() {
  try {
    boost::asio::io_context io_context;

    // Initialise et démarre le client
    Client client(io_context, "127.0.0.1", "1234");

    // Démarre la boucle d'événements
    io_context.run();
  } catch (const std::exception& e) {
    std::cerr << "Erreur : " << e.what() << "\n";
  }

  return 0;
}
