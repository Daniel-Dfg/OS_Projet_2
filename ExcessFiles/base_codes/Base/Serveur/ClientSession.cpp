#include "ClientSession.hpp"
// Instance d'une session client, s'occupe des envois / récepetion message au
// socket client

void ClientSession::start() {
  // Démarre lançant les opérations de lecture et d'écriture asynchrone
  read_message();
  send_message();
}

// Place les messages à envoyé dans une file d'attente (A protégé)
void ClientSession::read_message() {
  // Attendre un message du clientA de manière asynchrone
  // 1) Message reçu => extrait l'expéditeur(clientA) destinataire(clientB) et
  // texte 2) Le message est ajouté à la file `incomingMessages` de
  // l'expediteur(ClientA)
}

// Le serveur lis les messages qu'on a envoyé au client (A protégé)
void ClientSession::send_message() {
  // Vérifier si la file `outgoingMessages` contient des messages à recevoir
  // Si oui, prendre le premier message de la file et Une fois l'envoi terminé,
  // passer au message suivant dans la file.
  // Si la file est vide, ne rien faire
}

// Affiche les messages reçu à l'écran un part un
void ClientSession::handle_received_message(const Message &msg) {
  std::cout << "Message reçu de " << msg.getSender() << ": " << msg.getText()
            << std::endl;
  incomingMessages.push(msg);
}
// Push les messages dans la queue un par un
void ClientSession::queueMessage(const Message &msg) {
  outgoingMessages.push(msg);
}

// Fonctionnement
// Scénario avec deux clients :
//    1. Le client A envoie un message : "Hello, B".
//        ◦ Le serveur reçoit ce message sur le socket lié à l'instance de
//        ClientSession correspondant au client A. ◦ La fonction read_message()
//        de l'instance de ClientSession A :
//            ▪ Lit le message.
//            ▪ Identifie que ce message est destiné à B.
//            ▪ Stocke ce message dans incomingMessages de l'instance A.
//    2. Traitement côté serveur :
//        ◦ Le serveur extrait le message de la file incomingMessages de
//        ClientSession A. ◦ Il détecte que le message doit être envoyé au
//        client B. ◦ Le message est placé dans la file outgoingMessages de
//        ClientSession B.
//    3. Le client B reçoit le message :
//        ◦ La fonction send_message() de ClientSession B prend le message de sa
//        file outgoingMessages. ◦ Le message est envoyé au socket client de B.

// Message queue nécessaire car 2 threads pourrais écrire sur un même socket en
// même temps et ça provoquerais une erreur Dans un modèle asynchrone, si
// plusieurs messages sont envoyés en même temps c'est possible que ça ne vienne
// pas dans le bonne ordre "Bonjour", "ça va", "Alice" pourrais devenir "ça va",
// "Bonjour", "Alice"
// Ca permet aussi d'implémenter un affichage controlé du std::cout au terminal,
// il peut il avoir des erreurs sinon,