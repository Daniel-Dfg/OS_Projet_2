#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <sstream>
#include <string>

// Classe représentant un message échangé entre deux clients
class Message {
private:
  std::string sender, receiver, text;

public:
  Message(const std::string &sender_, const std::string &raw_text) {
    sender = sender_;
    std::stringstream ss(raw_text);
    getline(ss, receiver, ' '); // Extraction du destinataire
    getline(ss, text);          // Extraction du texte
  }

  Message() = default;

  std::string getSender() const { return sender; }
  std::string getReceiver() const { return receiver; }
  std::string getText() const { return text; }
};

#endif // MESSAGE_HPP
