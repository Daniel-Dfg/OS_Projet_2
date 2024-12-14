#pragma once
// TODO A RECHOISIR les include, (NECESSAIRE PENDANT LA FUSION)
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <string>
#include <sys/types.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <queue>

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
    static int constexpr MAX_MESSAGE_SIZE = 1024;
    string sender;
    string receiver;
    string text;

public:
    Message(const string& sender_, const string& raw_text)
            : sender(sender_), text(raw_text) {
      size_t pos = raw_text.find(" ");
      if (pos != string::npos) {
        receiver = raw_text.substr(0, pos);
        text = raw_text.substr(pos + 1);
      }
      else {
        //TODO : gérer le cas d'un message au format incorrect.
      }

      if(sizeof(text) > MAX_MESSAGE_SIZE){
        //TODO : gérer le cas d'un message trop long
      }
    }

    const string& getSender() const { return sender; }
    const string& getReceiver() const { return receiver; }
    const string& getText() const { return text; }
};