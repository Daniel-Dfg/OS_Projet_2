#include "SetUp.hpp"
#include "Client.hpp"


int main(int argc, char* argv[]) {
    std::string name;
    std::string IP = "127.0.0.1";
    int PORT = 8080;

    OptionsProgramme options;

    SetUp(argc,argv,name,IP,PORT,&options);

    Client client(name,options.isManuel,options.isBot,IP,PORT);
    std::cout<<"ok";
    return 0;
}
