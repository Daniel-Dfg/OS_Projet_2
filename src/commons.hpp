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
  string sender, receiver, text;
  ssize_t size;

public:
  Message(string &sender_, string &text) {
    this->sender = sender_;
    std::stringstream ss(text);
    if (getline(ss, receiver, ' ') && getline(ss, text)) {
      this->size = this->text.length();
    } else {
      // Traitement pour message invalide...
    }
  }
  Message() {}
  string getSender() const { return sender; }
  string getReceiver() const { return receiver; }
  string getText() const { return text; }
  bool isEmpty() const {return text == "";}
};
