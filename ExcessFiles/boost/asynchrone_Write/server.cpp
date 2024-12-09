#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/execution_context.hpp>

#include <memory> // Pour std::shared_ptr et std::make_shared

using boost::asio::ip::tcp;

class AsyncServer {
public:
    AsyncServer(boost::asio::io_context& io_context, const std::string& port)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), std::stoi(port))) {
        // Démarre l'acceptation de connexions
        start_accept();
    }

private:
    void start_accept() {
        // Crée un socket pour un client entrant
        auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());

        // Lance l'acceptation asynchrone
        acceptor_.async_accept(*socket,
           [this, socket](const boost::system::error_code& ec) {
               if (!ec) {
                   std::cout << "Client connecté depuis : " << socket->remote_endpoint() << "\n";
                   start_write(socket); // Envoie un message après connexion
               } else {
                   std::cerr << "Erreur d'acceptation : " << ec.message() << "\n";
               }

               // Continue à accepter d'autres connexions
               start_accept();
           });
    }

    void start_write(std::shared_ptr<tcp::socket> socket) {
        static int message_count = 1;
        std::string message = "Message du serveur #" + std::to_string(message_count++) + "\n";

        boost::asio::async_write(
            *socket, boost::asio::buffer(message),
            [this, socket](const boost::system::error_code& ec, std::size_t /*length*/) {
                if (!ec) {
                    // Relance l'écriture pour envoyer un autre message
                    start_write(socket);
                } else {
                    std::cerr << "Erreur d'envoi : " << ec.message() << "\n";
                    // Fermeture du socket si le client est déconnecté
                    socket->close();
                }
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        // Initialise et démarre le serveur sur le port 1234
        AsyncServer server(io_context, "1234");

        // Démarre la boucle d'événements
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
    }

    return 0;
}
