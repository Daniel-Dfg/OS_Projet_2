#include "Chat.hpp"
#include <iostream>

void Chat::start() {
  std::cout << "Chat démarré , nom de l'utilisateur : " << sender << std::endl;
  client.run();

  // Thread d'envoi des messages
  std::thread senderThread([this]() {
    std::string message;
    while (true) {
      std::cout << sender << " : ";
      std::getline(std::cin, message);
      client.sendMessages(message);
    }
  });

  // thread de réception des messages
  std::thread receiverThread([this]() {
    while (true) {
      client.readMessages();
    }
  });

  senderThread.join();
  receiverThread.detach(); // Continue à écouter les messages
}
