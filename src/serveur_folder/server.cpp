// TODO : Vérifier/refaire Gestionnaire de signaux
// TODO : Faire handlemessage connexion en 2 fonction
#include "server.hpp"
using std::string;

// A enlevé selon les implementations, définition des variables statiques
int Server::server_fd = 0;
std::vector<pollfd> Server::poll_fds;
std::unordered_map<string, int> Server::name_to_fd;
std::unordered_map<int, string> Server::fd_to_name;
std::queue<Message> Server::message_queue;
bool Server::running = false;


// Gestionnaire de signaux
void signalHandler(int signum) {
    std::cerr << "Signal reçu : " << signum << " (SIGINT). Fermeture du serveur...\n";
    Server::stop(); // Appelle la méthode statique pour arrêter le serveur
}

// Crée le socket principal pour écouter les connexions entrantes
void Server::init(int port){

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
    poll_fds.push_back(server_pollfd); // Ajoute le socket principal à la liste des sockets surveillés
}

 // Gère l'arrivée d'une nouvelle connexion client
void Server::handleNewConnection() {
    const int MAX_PSEUDO_SIZE = 30;
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = check_return_value(
        accept(server_fd, (struct sockaddr *)&client_addr, &addr_len),
        "accept"
    );


    // Lire le pseudo envoyé par le client (premier message automatique du client)
    char buffer[MAX_PSEUDO_SIZE + 1];  // +1 pour la terminaison '\0'
    int bytes_received = check_return_value(read(client_fd, buffer, sizeof(buffer) - 1), "lecture pseudo");
    buffer[bytes_received] = '\0';  // Ajoute la terminaison à la chaîne


    string client_name(buffer);

    // Vérification si le pseudo est déjà utilisé, si oui,  on déconnecte le client
    if (name_to_fd.find(client_name) != name_to_fd.end()) {
        std::cerr << "Pseudo déjà utilisé : " << client_name << ". Déconnexion du nouveau client.\n";
        string error_message = "[SYTEM] Pseudo déjà utilisé. Connetez vous avec un autre pseudo.\n";
        write(client_fd, error_message.c_str(), error_message.size());
        close(client_fd); // Ferme la connexion pour le client ayant le pseudo en double
        return;
    }

    // Permet de surveiller si il y a un événment disponible sur la socket du client, push dans le pool des truc à surveillé
    pollfd client_pollfd = {
        .fd = client_fd,
        .events = POLLIN,
        .revents = 0
    };

    poll_fds.push_back(client_pollfd); // ajoute la socket client dans la liste à surveillé

    name_to_fd[client_name] = client_fd;
    fd_to_name[client_fd] = client_name;

    std::cout << "Nouveau client connecté: " << client_name << std::endl;
}

// Gère les messages reçus d'un client, //TODO séparé cette fonction en plusieurs partie
void Server::handleClientMessage(int client_fd) {
    const int MAX_MESSAGE_SIZE = 1024;
    const int MAX_PSEUDO_SIZE = 30;
    const int MAX_BUFFER_SIZE = MAX_PSEUDO_SIZE + MAX_MESSAGE_SIZE + 2; // Inclut le pseudo et laise la place pour rajouter un \0 et l'espace entre pseudo et message
    char buffer[MAX_BUFFER_SIZE];

    // Lis le message du client
    int bytes = check_return_value(
        read(client_fd, buffer, sizeof(buffer)- 1),
        "lecture message client"
    );
    buffer[bytes] = '\0';
    // Vérification du contenu entier
    if (bytes > MAX_BUFFER_SIZE) {
            std::cerr << "Erreur durant la réception du message (trop long)";
            handleDisconnection(client_fd);
            return;
    }

    // Extraire le pseudo et le texte
    const string sender = fd_to_name[client_fd];
    string raw_message(buffer);
    size_t pos = raw_message.find(" ");


    // Verification du format
    if (pos == string::npos) {
        // Aucun espace trouvé : format invalide
        string error_message = "Erreur : format de message incorrect. Utilisez '<destinataire> <message>'.\n";
        send(client_fd, error_message.c_str(), error_message.size(), 0);
        handleDisconnection(client_fd);
        return;
    }

    string receiver = raw_message.substr(0, pos); // Partie avant l'espace : destinataire
    string message_text = raw_message.substr(pos + 1); // Partie après l'espace : texte du message

    // Vérification de la taille du texte seul
    if (message_text.size() > MAX_MESSAGE_SIZE) {
        string error = "Erreur : le texte du message dépasse " + std::to_string(MAX_MESSAGE_SIZE) + " octets.\n";
        send(client_fd, error.c_str(), error.size(), 0);
        handleDisconnection(client_fd);
        return;
    }

    // Verification Si le destinataire est connecté
    if (name_to_fd.find(receiver) == name_to_fd.end()) {
        write(client_fd, receiver.c_str(), sizeof(receiver)); // a etudier
        return;
    }

    message_text = sender + " " + message_text;
    // Création du message
    Message msg(sender,receiver, message_text);
    sendMessage(msg.getReceiver(), msg);
}

// Gère la déconnexion d'un client
void Server::handleDisconnection(int client_fd) {
    const string name = fd_to_name[client_fd];
    // Supprime les pseudo du mapp
    name_to_fd.erase(name);
    fd_to_name.erase(client_fd);

    // Suppresion du pseudo en parcourant le poll
    for (size_t i = 0; i < poll_fds.size(); i++) {
            if (poll_fds[i].fd == client_fd) {
                poll_fds.erase(poll_fds.begin() + i);
                break;
            }
        }

    check_return_value(close(client_fd), "fermeture socket client"); // on exit si on arrive pas à fermé la socket client??
    std::cout << "Client déconnecté: " << name << std::endl;
}


// Initialise le serveur
void Server::start(int port) {
    running = true;
    init(port);
    std::cout << "Serveur démarré sur le port " << port << std::endl;
}

// Envoie un message au client destinataire
void Server::sendMessage(const string receiver, const Message& msg) {
    auto it = name_to_fd.find(receiver);
    if (it != name_to_fd.end()) {
        ssize_t bytesSent = write(it->second, msg.getText().c_str(), msg.getText().size());
        if (bytesSent == -1) {
                // Erreur critique par exemple buffer de reception plein
                std::cerr << "Erreur  lors de l'envoi à " << receiver << ". Déconnexion...\n";
                handleDisconnection(it->second);
            }
        else {
            std::cout << "Message envoyé à " << receiver << " : " << msg.getText() << std::endl;
        }
    } else {
        // On envoie un message à l'expediteur que l'utilisateur à qui il a envoyé le message n'est pas connecté
        std::cerr << "Cette personne (" << receiver << ") n'est pas disponible.\n"; // affichage coté serveur
        auto sender_fd = name_to_fd[msg.getSender()];
        string error_message = receiver;
        if (write(sender_fd, error_message.c_str(), error_message.size()) == -1) {
            std::cerr << "Impossible d'envoyer un message d'erreur à l'expéditeur.\n";
            handleDisconnection(sender_fd);
        }
        }
}

// Boucle principale du serveur
void Server::run() {

    while (running) {
        int ret = poll(poll_fds.data(), poll_fds.size(), -1);
        if (ret == -1) {
            if (errno == EINTR) { // Vérifier si le poll a été interrompu par un CTRL + C
                    running = false;
                    break;
                continue; // Autre signal, on continue
            } else {
                perror("Erreur dans poll");
                exit(EXIT_FAILURE);
            }
        }
        // Ajout des socket à surveillé avec poll
        for (size_t i = 0; i < poll_fds.size(); i++) {
            if (poll_fds[i].revents == 0) continue;

            if (poll_fds[i].fd == server_fd) {
                handleNewConnection(); // Si l'evenement est sur le socket principal
            } else {
                handleClientMessage(poll_fds[i].fd); // Si l'evenement est sur le socket client
            }
        }
    }

    stop(); // Arrête le serveur
}

// Arrête le serveur et ferme toutes les connexions
void Server::stop() {
    if (!running) return; // Empêche plusieurs appels
    running = false;

    std::cerr << "Arrêt du serveur en cours...\n";
    for (const auto& pfd : poll_fds) {
        close(pfd.fd);
    }
    poll_fds.clear();
    std::cerr << "Serveur arrêté proprement.\n";
    exit(EXIT_SUCCESS);
}

int main() {
    // TODO géré correctement les signaux Ctrl + c ici
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, nullptr) < 0) {
        perror("Erreur lors de la configuration du gestionnaire de signaux");
        return 1;
    }
    try {
        Server::start(1234); // Démarre le serveur
        Server::run();       // Lance la boucle principale
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
