#pragma once
#include <iostream>
#include <string>
#include <sstream>

using std::string;

inline int check_return_value(int value, string elem = "???") {
  if (value < 0) {
    std::cerr << "Error : " << elem << std::endl;
    exit(1);
  }
  return value;
}

class Message {
private:
    const char* sender;
    const char* receiver;
    const char* text;

public:

    Message(const char* sender_, const char* raw_text)
        : sender(sender_){
            //parser le tableau de char pour en extraire le destinataire
            //(ne devrait-on pas utiliser des strings ?)
        }
// devrais crée une méthode ici pour récuperer le destinataire, car les méthodes précédente ne marche que sur les string

    const char* getSender() const { return sender; }
    const char* getReceiver() const { return receiver; }
    const char* getText() const { return text; }

};
