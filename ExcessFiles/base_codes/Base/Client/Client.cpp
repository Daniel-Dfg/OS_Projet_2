#include "Client.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_context &io_context, const std::string &ip,
               const std::string &port)
    : socket_(io_context), io_context_(io_context) {
  tcp::endpoint endpoint(boost::asio::ip::make_address(ip), std::stoi(port));

  socket_.async_connect(endpoint,
                        [this](const boost::system::error_code &errorCode) {
                          tryConnect(errorCode);
                        });
}

void Client::run() {
  std::thread readThread([this]() { io_context_.run(); });
  userInput();
  readThread.join();
}

void Client::tryConnect(const boost::system::error_code &errorCode) {
  if (!errorCode) {
    std::cout << "Connexion réussi " << std::endl;
    readMessages();
  } else {
    std::cerr << "Erreur de connexion " << errorCode.message() << "\n";
  }
}

void Client::readMessages() {
  boost::asio::async_read_until(
      socket_, boost::asio::dynamic_buffer(buffer_), '\n',
      [this](const boost::system::error_code &errorReading,
             std::size_t length) {
        if (!errorReading) {
          std::cout << buffer_.substr(0, length);
          buffer_.erase(0, length);
          readMessages();
        } else {
          std::cerr << "Erreur lecture de message: " << errorReading.message()
                    << std::endl;
        }
      });
}

void Client::sendMessages(const std::string &message) {
  boost::asio::async_write(socket_, boost::asio::buffer(message + "\n"),
                           [](const boost::system::error_code &errorSending,
                              std::size_t /*length*/) {
                             if (errorSending) {
                               std::cerr << "Erreur envoie message: "
                                         << errorSending.message() << std::endl;
                             }
                           });
}

void Client::userInput() {
  while (running) {
    std::string message;
    std::getline(std::cin, message);

    if (message == "quit") {
      io_context_.stop();
      running = false;
    }
    sendMessages(message);
  }
}