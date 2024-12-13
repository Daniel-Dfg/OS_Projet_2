#include "../commons.hpp"
#include <signal.h>


int main() {
    signal(SIGPIPE, SIG_IGN);

    int server_fd = check_return_value(socket(AF_INET, SOCK_STREAM, 0));

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    check_return_value(bind(server_fd, (struct sockaddr *)&address, sizeof(address)));
    check_return_value(listen(server_fd, 3));

    std::cout << "En attente de connexion...\n";

    socklen_t addrlen = sizeof(address);
    int new_socket = check_return_value(accept(server_fd, (struct sockaddr *)&address, &addrlen));

    std::cout << "Client connecté\n";

    char buffer[1024];
    int lu;

    while ((lu = read(new_socket, buffer, 1024)) > 0) {
        std::cout << "Message reçu du client : " << buffer;
        string to_send;
        to_send += "Renvoi : ";
        to_send += buffer;
        check_return_value(write(new_socket, to_send.c_str(), to_send.length()));
    }

    std::cout << "Déconnexion\n";

    close(new_socket);
    close(server_fd);
    return 0;
}
