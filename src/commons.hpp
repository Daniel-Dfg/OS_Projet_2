#pragma once
#include <csignal>
#include <iostream>
#include <string>

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
    Message(const string& sender_, const string& receiver_ ,const string& raw_text)
        : sender(sender_), receiver{receiver_}, text(raw_text) {}

    const string& getSender() const { return sender; }
    const string& getReceiver() const { return receiver; }
    const string& getText() const { return text; }
};
