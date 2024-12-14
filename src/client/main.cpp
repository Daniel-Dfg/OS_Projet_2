#include "SetUp.hpp"
#include "Client.hpp"


int main(int argc, char* argv[]) {
    std::string name ="";
    std::string IP = "127.0.0.1";
    int PORT = 1234;

    OptionsProgramme options;
    SetUp(argc,argv,name,IP,PORT,&options);


    Client(name,options.isManuel,options.isBot,IP,PORT);
    return 0;
}
