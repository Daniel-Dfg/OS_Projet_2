#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>

using std::string;


inline int check_return_value(int value, string elem = "???"){
    if(value < 0){
        std::cerr << "Error : " << elem << std::endl;
        exit(1);
    }
    return value;
}

class Message {
public:
    size_t length;
    std::string text;

    Message(const std::string& t = "") : text(t), length(t.length()) {}
};
