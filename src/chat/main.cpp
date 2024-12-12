#include <boost/asio.hpp>
#include "client.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
   try {
      boost::asio::io_context io_context;

      // Initialise et démarre le client
      Client client(io_context, "127.0.0.1", "1234");
      client.run();
   } catch (const std::exception& e) {
      std::cerr << "Erreur : " << e.what() << "\n";
   }

   return 0;
}
