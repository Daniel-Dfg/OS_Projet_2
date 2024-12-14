// Ne pas oublier de brancher le client au bon port, et de faire ./Client USERNAME, et que le client envoie directement son username
// juste après la connection
#include <queue>
#include <string>
#include <sys/types.h>
#include <poll.h>
#include <unordered_map>
#include "../commons.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring> // Pour strlen
#include <iostream>

//#include <signal.h>

using std::string, std::unordered_map;

class Server {
private:
    static constexpr int MAX_CLIENTS = 1000;

    static int server_fd;
    static std::vector<pollfd> poll_fds;
    static unordered_map<const char *, int> name_to_fd;
    static unordered_map<int, const char *> fd_to_name;
    static std::queue<Message> message_queue;
    static bool running;

    static void init(int port) { //C.F base_codes cas 1 et 2

        server_fd = check_return_value(socket(AF_INET, SOCK_STREAM, 0), "socket creation");

        int opt = 1;
        check_return_value(
            setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)),
            "setsockopt"
        );

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        check_return_value(
            bind(server_fd, (struct sockaddr *)&address, sizeof(address)),
            "bind"
        );

        check_return_value(
            listen(server_fd, MAX_CLIENTS),
            "listen"
        );

        pollfd server_pollfd = {
            .fd = server_fd,
            .events = POLLIN,
            .revents = 0
        };
        poll_fds.push_back(server_pollfd);
    }

    static void handleNewConnection() {

        sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = check_return_value(
            accept(server_fd, (struct sockaddr *)&client_addr, &addr_len),
            "accept" 
        );


        // Lire le premier message envoyé par le client (censé être le pseudo)
        // A CHANGER. ligne de code barbare
        
        char buffer[20]; 
        int bytes_received = check_return_value(read(client_fd, buffer, sizeof(buffer) - 1), "lecture pseudo"); 
        buffer[bytes_received] = '\0'; // c'est le client qui devras s'occuper d'envoyer des messages sous forme correcte, donc devras retirer le "-1" plus haut et le "\0"

        char* client_name = new char[bytes_received + 1];
        std::strcpy(client_name, buffer);


        // Permet de surveillé si il y a un évenment disponible sur la socket du client, push dans le pool des truc à surveillé
        pollfd client_pollfd = {
            .fd = client_fd,
            .events = POLLIN,
            .revents = 0
        };
        poll_fds.push_back(client_pollfd);
        name_to_fd[client_name] = client_fd;
        
        fd_to_name[client_fd] = client_name;

        std::cout << "Nouveau client connecté: " << client_name << std::endl;
    }



    static void handleClientMessage(int client_fd) {
        //TODO : gérer les cas où le message est trop long
        char buffer[1024];
        int bytes = check_return_value(
            read(client_fd, buffer, sizeof(buffer)- 1),
            "lecture message client"
        );
        buffer[bytes] = '\0'; // c'est le client qui devras s'occuper d'envoyer des messages sous forme correcte, donc devras retirer le "-1" plus haut et le "\0"

        // read est uné operation bloquante, mais elle ne pose pas de problème car handleclientmessage est appelé uniquement quand il y a un evenment sur la socket
        if (bytes == 0) {
            handleDisconnection(client_fd);
            return;
        }
        std::cout << "Message reçu : " << buffer << std::endl;
        const char *sender = fd_to_name[client_fd];
        //Message msg(buffer, bytes);
        Message msg(sender, buffer); //FIX : tableau de char à la place de string
        message_queue.push(msg); 
    }

    static void handleDisconnection(int client_fd) {
        const char* name = fd_to_name[client_fd];

        name_to_fd.erase(name);
        fd_to_name.erase(client_fd);

        for (size_t i = 0; i < poll_fds.size(); i++) {
                if (poll_fds[i].fd == client_fd) {
                    poll_fds.erase(poll_fds.begin() + i);
                    break;
                }
            }

        check_return_value(close(client_fd), "fermeture socket client");
        std::cout << "Client déconnecté: " << name << std::endl;
    }

public:

    static void start(int port = 8080) {
        running = true;
        init(port);
        std::cout << "Serveur démarré sur le port " << port << std::endl;
    }

    static void sendMessage(const char* receiver, const Message& msg) {
        // /!\ l'argument 'message' étais avant la modification une RValue ici, pas sûr que notre programme final fonctionne comme ça. À vérifier.
        //it = (client, fd)
        
        if (auto it = name_to_fd.find(receiver); it != name_to_fd.end()) {
            check_return_value(
                write(it->second, msg.getText(), strlen(msg.getText())),
                "écriture au client"
            );
        }
        else {
            //TODO : raffiner cette vérification pour déterminer si le client n'existe pas où s'il n'est juste pas connecté
            std::cout << "Ce client n'est pas connecté ou n'existe pas (à déterminer)" << std::endl;
        }
    }

    static void run() {

        while (running) {
            int ret = check_return_value(
                poll(poll_fds.data(), poll_fds.size(), -1),
                "poll"
            );

            for (size_t i = 0; i < poll_fds.size(); i++) {  //vérification des polls (chatGPT)
                if (poll_fds[i].revents == 0) continue; // si rien ne se passe on continue la boucle

                if (poll_fds[i].fd == server_fd) { // si l'évenement viens de la socket principal on gère la nouvelle connexion
                    handleNewConnection();
                } else { // si on est un client on gère 
                    handleClientMessage(poll_fds[i].fd); // si l'évenement viens d'une socket client on gère la nouvelle connexion
                }
            }
        }
    }

    static void stop() {
        running = false;
        for (const auto& pfd : poll_fds) {
            check_return_value(close(pfd.fd), "close socket");
        }
    }
};

// A ENLEVER APRES QUON EST AJOUTER LES HPP, c'est juste pour faire compilé, je n'ai pas mis dans un hpp 
// pour l'instant car on pouvais toujours déléguer ces tâches à d'autre classe ou quoi
int Server::server_fd = 0; 
std::vector<pollfd> Server::poll_fds; 
std::unordered_map<const char *, int> Server::name_to_fd;
std::unordered_map<int, const char *> Server::fd_to_name;
std::queue<Message> Server::message_queue; 
bool Server::running = false; 

int main() {
    //signal(SIGPIPE, SIG_IGN);

    try {
        Server::start(8080);
        Server::run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
