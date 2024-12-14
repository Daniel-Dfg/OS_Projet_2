#include "client.hpp"


void Client::senderLoop() {
    while (running) {
        Message msg;
        try{
            outgoingMessages.pop(); //FIX : incorrect, devrait être assigné à un Message
        }
        catch(std::exception){
        //...
    }
        if (msg.isEmpty()) {
            // Format: "receiver message"
            string formatted = msg.getReceiver() + " " + msg.getText() + "\n";
            if (write(sock_fd, formatted.c_str(), formatted.length()) < 0) {
                std::cerr << "Erreur d'envoi\n";
                running = false;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Client::receiverLoop() {
    char buffer[1024];
    while (running) {
        int bytes = read(sock_fd, buffer, 1024);
        if (bytes <= 0) {
            if (bytes < 0) {
                std::cerr << "Erreur de lecture\n";
            } else {
                std::cout << "Serveur déconnecté\n";
            }
            running = false;
            break;
        }
        buffer[bytes] = '\0';
        std::cout << "Message reçu: " << buffer << std::endl;
    }
}

void Client::connectToServer() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        throw std::runtime_error("Création socket échouée");
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);  // Même port que le serveur
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Connexion échouée");
    }

    // Envoyer le nom au serveur
    write(sock_fd, name.c_str(), name.length());
}



void Client::sendMessage(const string& receiver, const string& text) {
    string formatted = receiver + " " + text;
    Message msg(name, formatted);
    outgoingMessages.push(msg);
}

void Client::stop() {
    running = false;
    if (sender_thread.joinable())
        sender_thread.join();
    if (receiver_thread.joinable())
        receiver_thread.join();
    close(sock_fd);
}
