#include "../commons.hpp"

int main() {

  int server_fd = socket(AF_INET, SOCK_STREAM, 0); // Créer le socket
  check_return_value(server_fd, "server fd");

  // Permet la réutilisation du port/de l'adresse
  int opt = 1;

  check_return_value(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
             sizeof(opt)), "setsockopt");

  struct sockaddr_in address;

  address.sin_family = AF_INET; // AdressFamily_Internet
  address.sin_addr.s_addr = INADDR_ANY; // InternetAdress_Any (une machine peut avoir plusieurs
                                        // adresses Internet : locale, distante...)
  address.sin_port = htons(8080); // si mis à 0 : laisser l'OS choisir le port

  // Définit l'adresse et le port d'écoute, réserve le port (autrement dit :
  // attacher l'adresse au socket)
  check_return_value(bind(server_fd, (struct sockaddr *)&address, sizeof(address)), "bind");

  // Commence l'écoute
  check_return_value(listen(server_fd, 3), "listen"); // max 3 connexions en attente
  size_t addrlen = sizeof(address);

  // Ouvre une nouvelle connexion si besoin
  int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  check_return_value(new_socket, "new_socket");
  char buffer[1024];
  // Reçoit un message
  check_return_value(read(new_socket, buffer, 1024), "read");
  printf("Message reçu :%s\n", buffer);

  close(new_socket);
  close(server_fd);
  return 0;
}
