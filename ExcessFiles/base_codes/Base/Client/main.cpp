#include "Chat.hpp"
#include <boost/asio.hpp>

// Lancer faire ./Chat username, après rajouter les ports et autre
int main(int argc, char *argv[]) {
  try {
    // code d'erreur et autre à foutre avant pour le
    boost::asio::io_context io_context;

    std::string username = argv[1];
    Client client = Client(username, io_context, "127.0.0.1", "1234");
    Chat chat(client, username);
    chat.start();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
