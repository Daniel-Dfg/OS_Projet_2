#include "Client.hpp"
#include "SetUp.hpp"

int main(int argc, char *argv[]) {
  std::string displayName;
  std::string IP = "127.0.0.1";
  int PORT = 1234;

  OptionsProgramme options;

  SetUp(argc, argv, displayName, IP, PORT, &options);
  std::cout << IP << " " << PORT << "\n";

  Client client(displayName, options.isManuel, options.isBot, IP, PORT);
  return 0;
}
