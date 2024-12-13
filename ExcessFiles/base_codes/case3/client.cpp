#include "../commons.hpp"
#include <ostream>
#include <signal.h>
#include <thread>
#include <atomic>

// Variable pour contrôler l'arrêt des threads
std::atomic<bool> running{true};

// Fonction pour le thread de réception
void receiver_thread(int sock) {
    char response[1024];
    while (running) {
        int ret = read(sock, response, 1024);
        if (ret <= 0) {
            if (ret < 0) {
                std::cerr << "Erreur de lecture\n";
            } else {
                std::cout << "Déconnexion du serveur.\n";
            }
            running = false;
            break;
        }
        response[ret] = '\0';  // Assure que la chaîne est terminée
        std::cout << "Reçu : " << response;
    }
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    int sock = check_return_value(socket(AF_INET, SOCK_STREAM, 0));

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    check_return_value(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));

    // Création du thread de réception
    std::thread receiver(receiver_thread, sock);

    // Thread principal pour l'envoi
    std::string buffer;
    while (running && std::getline(std::cin, buffer)) {
        buffer += '\n';
        int ret = write(sock, buffer.c_str(), buffer.length());
        if (ret <= 0) {
            std::cerr << "Erreur d'écriture\n";
            running = false;
            break;
        }
    }

    // Nettoyage
    running = false;
    receiver.join();  // Attendre la fin du thread de réception
    close(sock);
    return 0;
}
