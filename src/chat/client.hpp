#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
#include <string>

class Client {
public:
    Client(boost::asio::io_context& io_context, const std::string& ip, const std::string& port);
    void run();

private:
    void tryConnect(const boost::system::error_code& errorCode);
    void readMessages();
    void sendMessages(const std::string& message);
    void userInput();

    boost::asio::ip::tcp::socket socket_;
    boost::asio::io_context& io_context_;
    std::string buffer_;
    bool running=true;
};

#endif // CLIENT_HPP
