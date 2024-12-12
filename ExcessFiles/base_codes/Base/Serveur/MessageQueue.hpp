#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

#include "Message.hpp"
#include <mutex>
#include <queue>

// Crée des queues local pour chaque client
class MessageQueue {
private:
  std::queue<Message> messages; // File d'attente des messages
  mutable std::mutex mtx; // Ajoutez le mot-clé `mutable` car les méthodes const
                          // doivent pouvoir modifier le mutex
public:
  // Ajoute le message à la file
  void push(const Message &msg) {
    std::lock_guard<std::mutex> lock(mtx);
    messages.push(msg);
  }
  // Pop un message de la queue et le traite
  bool pop(Message &msg) {
    std::lock_guard<std::mutex> lock(mtx);
    if (messages.empty()) {
      return false;
    }
    msg = messages.front();
    messages.pop();
    return true;
  }
  // Return true si la file est vide
  bool empty() const {
    std::lock_guard<std::mutex> lock(mtx);
    return messages.empty();
  }
};

#endif // MESSAGEQUEUE_HPP
