#include <iostream>
#include <boost/asio.hpp>
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
        std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(acceptor_.get_executor());

        // Lance l'acceptation asynchrone
        acceptor_.async_accept(*socket,
           [this, socket](const boost::system::error_code& ec) {
               if (!ec) {
                   std::cout << "Client connecté depuis : " << socket->remote_endpoint() << "\n";
                   start_read(socket);
               } else {
                   std::cerr << "Erreur d'acceptation : " << ec.message() << "\n";
               }

               // Continue à accepter d'autres connexions
               start_accept();
           });
    }

    void start_read(std::shared_ptr<tcp::socket> socket) {
        // Démarre une lecture asynchrone
        auto buffer = std::make_shared<std::string>();
        boost::asio::async_read_until(
            *socket, boost::asio::dynamic_buffer(*buffer), '\n',
            [this, socket, buffer](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    std::cout << "Message reçu : " << buffer->substr(0, length);
                    buffer->erase(0, length); // Nettoie le tampon après lecture

                    // Lire à nouveau du même client
                    start_read(socket);
                } else {
                    std::cerr << "Erreur de lecture : " << ec.message() << "\n";
                }
            });
    }

    tcp::acceptor acceptor_; // équivalent socket() + bind() + listen()
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
