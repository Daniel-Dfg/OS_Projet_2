#ifndef CHAT_HPP
#define CHAT_HPP

#include "Client.hpp"
#include <string>
#include <thread>

class Chat {
private:
  std::string sender;
  Client &client; // Référence au client Boost.Asio

public:
  Chat(Client &client, const std::string &sender)
      : client(client), sender(sender) {}

  void start();
};

#endif // CHAT_HPP
