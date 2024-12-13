// Rien changer partie d'haluk

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>

class Client {
public:
  Client(std::string &name_, boost::asio::io_context &io_context,
         const std::string &ip, const std::string &port);

  void run();
  void sendMessages(const std::string &
                        message); // Assurez-vous que cette méthode est publique
  void readMessages();
  std::string get_name() { return name_; }

private:
  void tryConnect(const boost::system::error_code &errorCode);
  std::string name_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::io_context &io_context_;
  std::string buffer_;
  bool running = true;

  void userInput();
};
