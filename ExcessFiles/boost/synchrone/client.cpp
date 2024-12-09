#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
    try {
        // Création de l'objet io_context, nécessaire pour gérer les opérations I/O
        boost::asio::io_context io_context;

        // Résolution de l'adresse IP et du port du serveur.
        // Resolver permet de convertir le nom de domaine "www.example.com" en adresse IP
        // tcp::resolver resolver(io_context);
        // auto endpoints = resolver.resolve("127.0.0.1", "1234"); // Serveur à localhost:1234
        tcp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 1234)


        // Création du socket pour se connecter au serveur
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints); // Se connecte au serveur

        std::cout << "Connecté au serveur.\n";

        // Lecture du message envoyé par le serveur
        char buffer[1024] = {0}; // Tampon pour recevoir les données
        size_t length = socket.read_some(boost::asio::buffer(buffer)); // Lecture synchrone
        std::cout << "Message reçu : " << std::string(buffer, length) << "\n";

    } catch (const std::exception& e) {
        // Gestion des erreurs (ex. : serveur inaccessible, connexion échouée)
        std::cerr << "Erreur : " << e.what() << "\n";
    }

    return 0;
}
