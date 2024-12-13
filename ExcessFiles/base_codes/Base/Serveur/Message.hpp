#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

// Classe représentant un message échangé entre deux clients
class Message {
private:
  std::string sender;   // L'expéditeur du message
  std::string receiver; // Le destinataire du message
  std::string text;     // Le contenu du message

public:
  // Constructeur avec expéditeur et texte brut
  Message(const std::string &sender_, const std::string &raw_text)
      : sender(sender_), text(raw_text) {}

  // Constructeur complet avec expéditeur, destinataire et texte
  Message(const std::string &sender_, const std::string &receiver_,
          const std::string &text_)
      : sender(sender_), receiver(receiver_), text(text_) {}

  // Constructeur par défaut
  Message() = default;

  // Accesseurs
  std::string getSender() const { return sender; }
  std::string getReceiver() const { return receiver; }
  std::string getText() const { return text; }

  // Mutateurs (si nécessaire, à éviter si la classe doit rester immuable)
  void setReceiver(const std::string &receiver_) { receiver = receiver_; }
  void setText(const std::string &text_) { text = text_; }
};

#endif // MESSAGE_HPP
