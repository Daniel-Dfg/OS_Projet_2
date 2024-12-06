#include "../commons.hpp"
#include <ostream>
#include <signal.h>

int main() {
    signal(SIGPIPE, SIG_IGN);

    int sock = check_return_value(socket(AF_INET, SOCK_STREAM, 0));

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    check_return_value(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));

    std::string buffer;

    while (std::getline(std::cin, buffer)) {
        /*
       PROBLÈME : getline est bloquant, donc si le serveur se déconnecte le client
       devra envoyer un message avant de s'en rendre compte...
        */
        buffer += '\n';
        check_return_value(write(sock, buffer.c_str(), buffer.length()));

        char response[1024];
        int ret = read(sock, response, 1024);
        if (ret <= 0) {
            if (ret < 0) {
                std::cerr << "Erreur de lecture\n";
            } else {
                std::cout << "Déconnexion du serveur.\n";
            }
            break;
        }

        std::cout << "Reçu." << std::endl;
    }

    close(sock);
    return 0;
}
