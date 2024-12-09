#include <iostream>
#include <boost/asio.hpp>

""" Cet exemple est un serveur Boost.Asio qui communique avec des clients via TCP de manière synchrone. """


using boost::asio::ip::tcp; // Permet d'écrire tcp directement

int main() {
    try {
        // io_context est le cerveau de Boost.asio. C'est une boucle d'événements
        // Coordonne l'I/O, planifie les tâches, nécessaire pour les objets de Boost.Asio...
        boost::asio::io_context io_context;
        // Configure un acceptor pour écouter les connexions entrantes
        // 1234 pour le Port
        tcp::endpoint endpoint1(tcp::v4(), 1234); //Adresse IP 0.0.0.0 Toutes les adresses locales (localhost, Ethernet, Wi-Fi, etc.)
        tcp::endpoint endpoint2(boost::asio::ip::make_address("127.0.0.1"), 1234); // Uniquement les connexions depuis localhost
        tcp::acceptor acceptor(io_context, endpoint2); // Attends les connexion entrantes sur l'adresse et port donné

        std::cout << "Serveur en attente de connexions sur le port 1234...\n";

        while (true) {
            // Accepter une connexion
            tcp::socket socket(io_context); // point d'entrée qui permet d'écouter et d'accepter des connexions venant des clients.
            acceptor.accept(socket); // Bloque jusqu'à ce qu'un client se connecte


            // Elle permet d’obtenir l'adresse IP et le port de l'autre côté de la connexion. ex 127.0.0.1:5678
            std::cout << "Client connecté depuis : " << socket.remote_endpoint() << "\n";

            // Envoie un message au client.
            std::string message = "Bonjour, client !\n";
            // Envoie message de manière !!Synchrone!!
            boost::asio::write(socket, boost::asio::buffer(message));
        }
    } catch (const std::exception& e) { // exceptions d'erreur comme connexion echoué, IP ou port invalide etc.
        std::cerr << "Erreur : " << e.what() << "\n";
    }

    return 0;
}