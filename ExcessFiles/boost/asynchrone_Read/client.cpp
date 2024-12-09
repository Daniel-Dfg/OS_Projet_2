#include <iostream>
#include <boost/asio.hpp>
#include <functional> // Pour std::bind

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

            // Démarre une lecture asynchrone après la connexion
            boost::asio::async_read_until(
                    socket_, boost::asio::dynamic_buffer(buffer_), '\n',
                    std::bind(&AsyncClient::on_read, this, std::placeholders::_1, std::placeholders::_2));
        } else {
            std::cerr << "Erreur de connexion : " << ec.message() << "\n";
        }
    }

    void on_read(const boost::system::error_code& ec, std::size_t length) {
        if (!ec) {
            std::cout << "Message reçu : " << buffer_.substr(0, length);
            buffer_.erase(0, length); // Nettoie le tampon après lecture
        } else {
            std::cerr << "Erreur de lecture : " << ec.message() << "\n";
        }
    }

    tcp::socket socket_;
    std::string buffer_;
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
