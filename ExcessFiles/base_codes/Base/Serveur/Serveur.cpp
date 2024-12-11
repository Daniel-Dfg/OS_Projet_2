#include "Serveur.hpp"

// Soumet une tâche au thread_pool pour l'exécuter via io_context
void Serveur::submitToPool(boost::asio::ip::tcp::socket socket) {
  boost::asio::post(io_context, [socket = std::move(socket)]() {
    try {
      std::cout << "Tâche initiale exécutée dans io_context" << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Erreur dans submitToPool : " << e.what() << std::endl;
    }
  });
}

// Configure la socket principale et initialise l'acceptation des connexions
void Serveur::createSocket(int port) {
  try {
    // Création de l'endpoint pour écouter sur l'adresse locale (127.0.0.1) et
    // le port spécifié
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::make_address("127.0.0.1"), port);

    // Création de l'acceptor pour accepter les connexions entrantes
    acceptor_ =
        std::make_shared<boost::asio::ip::tcp::acceptor>(io_context, endpoint);

    std::cout << "Serveur prêt sur le port 127.0.0.1:" << port << std::endl;

    // Prépare une socket pour accepter une première connexion
    std::shared_ptr<boost::asio::ip::tcp::socket> socket =
        std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    acceptConnections(*acceptor_, socket);
  } catch (const std::exception &e) {
    std::cerr << "Erreur dans createSocket : " << e.what() << std::endl;
  }
}

// Accepte les connexions entrantes de manière asynchrone
void Serveur::acceptConnections(
    boost::asio::ip::tcp::acceptor &acceptor,
    std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
  if (!running) {
    std::cerr << "Serveur arrêté. Fin des acceptations." << std::endl;
    return;
  }

  if (!acceptor.is_open()) {
    std::cerr << "Acceptor fermé. Fin des acceptations." << std::endl;
    return;
  }

  // Accepte une connexion entrante de manière asynchrone
  acceptor.async_accept(*socket, [this, socket, acceptor_ptr = acceptor_](
                                     boost::system::error_code ec) mutable {
    if (!ec) {
      // Simplement pour simulé les connexions de différent utilisateur
      std::cout << "Connexion acceptée avec un client." << std::endl;
      static int nbuser = 0;
      std::string username = "user" + std::to_string(nbuser);
      nbuser++;
      // ajoute le client à la base de donnée de client connecté
      clientmanager.add_client(username, socket);
      clientmanager.print_client_liste();

      boost::asio::post(io_context, [this, socket]() mutable {
        handleClient(std::move(*socket));
      });

      // Prépare une nouvelle socket pour la prochaine connexion
      std::shared_ptr<boost::asio::ip::tcp::socket> newSocket =
          std::make_shared<boost::asio::ip::tcp::socket>(io_context);

      acceptConnections(*acceptor_ptr, newSocket);
    } else {
      std::cerr << "Erreur lors de l'acceptation : " << ec.message()
                << std::endl;

      // Si l'acceptor est toujours ouvert, réessaye l'acceptation (on peux
      // retirer ça)
      if (acceptor_ptr->is_open() && running) {
        std::cout << "Relance de l'acceptation après une erreur." << std::endl;
        acceptConnections(
            *acceptor_ptr,
            std::make_shared<boost::asio::ip::tcp::socket>(io_context));
      }
    }
  });
}

// Lance la gestion des tâches dans io_context avec un pool de threads
void Serveur::startIoContextWithThreadPool() {
  try {
    io_context
        .restart(); // Prépare la file d'evenements pour les nouvelles tâches
    for (size_t i = 0; i < nbdethreads; ++i) {
      boost::asio::post(threadPool, [this]() {
        try {
          io_context.run(); // Exécute les tâches dans la file
        } catch (const std::exception &e) {
          std::cerr << "Exception dans io_context.run : " << e.what()
                    << std::endl;
        }
      });
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception dans startIoContextWithThreadPool : " << e.what()
              << std::endl;
  }
}
// Gère la communication avec un client connecté
void Serveur::handleClient(boost::asio::ip::tcp::socket socket) {
  try {
    char buffer[1024]; // Buffer pour lire les messages
    while (true) {
      boost::system::error_code error;

      // Lecture d'un message depuis le client
      size_t length = socket.read_some(boost::asio::buffer(buffer), error);
      if (error == boost::asio::error::eof ||
          error == boost::asio::error::connection_reset) {
        std::cout << "Connexion fermée par le client." << std::endl;
        break;
      } else if (error) {
        std::cerr << "Erreur lors de la lecture : " << error.message()
                  << std::endl;
        break;
      }

      // Affiche le message reçu
      std::string message(buffer, length);
      std::cout << "Reçu : " << message << std::endl;

      // Réponse au client
      std::string response = "Message reçu par le serveur : " + message;
      boost::asio::write(socket, boost::asio::buffer(response), error);

      if (error) {
        std::cerr << "Erreur lors de l'envoi : " << error.message()
                  << std::endl;
        break;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception dans handleClient : " << e.what() << std::endl;
  }
}

// Arrête proprement le serveur
void Serveur::stop() {
  try {
    running = false;
    if (!io_context.stopped()) {
      std::cout << "Arrêt du serveur en cours..." << std::endl;
      io_context.stop();
    }
    threadPool.join(); // Attend que toutes les tâches terminent
    std::cout << "Serveur arrêté proprement." << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Erreur lors de l'arrêt du serveur : " << e.what()
              << std::endl;
  }
}

// Point d'entrée pour démarrer le serveur
int Serveur::start(bool modBot, bool modManuel) {
  createSocket(1234); // Configure la socket d'écoute

  std::cout << "Serveur prêt. En attente de connexions..." << std::endl;

  startIoContextWithThreadPool(); // Démarre le pool de threads et io_context

  // Bloque jusqu'à ce que toutes les tâches dans le thread_pool soient
  // terminées
  threadPool.join();

  return 0;
}
