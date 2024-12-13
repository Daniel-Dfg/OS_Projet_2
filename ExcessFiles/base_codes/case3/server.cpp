#include "../commons.hpp"
//#include <signal.h>
#include <poll.h>
#include <vector>

const int MAX_CLIENTS = 1000;

int main() {
    //signal(SIGPIPE, SIG_IGN);

    int server_fd = check_return_value(socket(AF_INET, SOCK_STREAM, 0));

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    check_return_value(bind(server_fd, (struct sockaddr *)&address, sizeof(address)));
    check_return_value(listen(server_fd, MAX_CLIENTS));

    std::cout << "En attente de connexions...\n";

    // Initialisation du vecteur de pollfd
    std::vector<pollfd> poll_fds;

    // Ajout du socket serveur
    pollfd server_pollfd = {
        .fd = server_fd,
        .events = POLLIN,
        .revents = 0
    };
    poll_fds.push_back(server_pollfd);

    while (true) {
        // Appel à poll
        int ret = poll(poll_fds.data(), poll_fds.size(), -1);
        if (ret < 0) {
            if (errno == EINTR) continue;
            perror("poll failed");
            break;
        }

        // Parcours des événements
        for (size_t i = 0; i < poll_fds.size(); i++) {
            if (poll_fds[i].revents == 0) continue;

            // Nouvelle connexion sur le socket serveur
            if (poll_fds[i].fd == server_fd) {
                socklen_t addrlen = sizeof(address);
                int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
                if (new_socket < 0) {
                    perror("accept failed");
                    continue;
                }

                // Vérification du nombre maximum de clients
                if (poll_fds.size() >= MAX_CLIENTS + 1) {
                    std::cout << "Nombre maximum de clients atteint\n";
                    close(new_socket);
                    continue;
                }

                std::cout << "Nouveau client connecté\n";

                // Ajout du nouveau client au vecteur
                pollfd client_pollfd = {
                    .fd = new_socket,
                    .events = POLLIN,
                    .revents = 0
                };
                poll_fds.push_back(client_pollfd);
            }
            // Données reçues d'un client
            else {
                char buffer[1024] = {0};
                int lu = read(poll_fds[i].fd, buffer, 1024);

                // Déconnexion ou erreur
                if (lu <= 0) {
                    if (lu < 0) {
                        std::cerr << "Erreur de lecture\n";
                    } else {
                        std::cout << "Client déconnecté\n";
                    }
                    close(poll_fds[i].fd);
                    poll_fds.erase(poll_fds.begin() + i);
                    i--; // Important car on modifie le vecteur pendant l'itération
                    continue;
                }

                // Traitement du message
                std::cout << "Message reçu d'un client : " << buffer;
                string to_send = "Renvoi : ";
                to_send += buffer;
                check_return_value(write(poll_fds[i].fd, to_send.c_str(), to_send.length()));
            }
        }
    }

    // Nettoyage
    for (const auto& pfd : poll_fds) {
        close(pfd.fd);
    }
    return 0;
}
