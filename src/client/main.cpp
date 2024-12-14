#include "SetUp.hpp"
#include "Client.hpp"


int main(int argc, char* argv[]) {
    std::string displayName;
    std::string realName;
    std::string IP = "127.0.0.1";
    int PORT = 1234;

    OptionsProgramme options;

    SetUp(argc,argv,displayName,IP,PORT,&options,realName);

    Client client(displayName,options.isManuel,options.isBot,IP,PORT,realName);
    std::cout<<"ok";
    return 0;
}
