#include <iostream>
#include <boost/asio.hpp>
#include <functional> // Pour std::bind
#include <boost/asio/io_context.hpp>
#include <boost/asio/execution_context.hpp>


using boost::asio::ip::tcp;

class AsyncClient {
public:
    AsyncClient(boost::asio::io_context& io_context, const std::string& ip, const std::string& port)
            : socket_(io_context) {
        // Crée un endpoint avec l'adresse IP et le port
        tcp::endpoint endpoint(boost::asio::ip::make_address(ip), std::stoi(port));

        // Démarre la connexion asynchrone
        socket_.async_connect(endpoint, std::bind(&AsyncClient::on_connect, this, std::placeholders::_1));
    }

private:
    void on_connect(const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "Connecté au serveur !\n";
            prompt_user(); // Lance la saisie utilisateur
        } else {
            std::cerr << "Erreur de connexion : " << ec.message() << "\n";
        }
    }

    void send_message(const std::string& message) {
        boost::asio::async_write(
            socket_, boost::asio::buffer(message + "\n"),
            [this](const boost::system::error_code& ec, std::size_t /*length*/) {
                if (ec) {
                    std::cerr << "Erreur d'envoi : " << ec.message() << "\n";
                }
            });
    }

    void prompt_user() {
        while (true) {
            std::cout << "Entrez un message : ";
            std::string message;
            std::getline(std::cin, message);

            if (message == "quit") { // Ajoute une condition de sortie si nécessaire
                break;
            }

            send_message(message);
        }
    }

    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        // Initialise et démarre le client
        AsyncClient client(io_context, "127.0.0.1", "1234");

        // Démarre la boucle d'événements
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
    }

    return 0;
}
