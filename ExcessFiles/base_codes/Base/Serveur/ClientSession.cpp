#include "ClientSession.hpp"
// Instance d'une session client, s'occupe des envois / récepetion message au
// socket client

void ClientSession::start(
    std::unordered_map<std::string, std::shared_ptr<ClientSession>>
        &connectedclient) {
  read_message(connectedclient); // Lis les messages du client et les envoies
                                 // dans les outgoing message du destinataire
  send_message();                // lis les ongoing message du client actuel
}

// On lis ce que notre client envoie
void ClientSession::read_message(
    std::unordered_map<std::string, std::shared_ptr<ClientSession>>
        &clientconnected) {
  auto self =
      shared_from_this(); // Maintenir l'objet vivant pendant le callback

  // Lecture en mode asynchrone, non bloquante
  socket_->async_read_some(
      boost::asio::buffer(buffer_),
      [this, self, &clientconnected](boost::system::error_code ec,
                                     std::size_t length) {
        if (!ec) {
          // Si on est ici, on a reçu le message avec succès

          // Temporaire, c'est pour affiché qu'on a bien reçu le message sur le
          // serveur
          std::string message(buffer_.data(),
                              length); // Convertir le buffer en std::string
          std::cout << "Reçu : " << message << std::endl;

          Message msg(name_, message); // Crée le message avec le bon format

          // Un essai pour envoyer à user1 un message, en récuperant la socket :
          std::string destinataire = "user1";
          auto pair = clientconnected.find(destinataire);
          std::shared_ptr<boost::asio::ip::tcp::socket> socket =
              pair->second->get_socket();
          // écriture de manière synchrone

          // Ajouter le message dans la file outgoingMessages du destinataire
          pair->second->queueMessage(msg);

          // Relancer une lecture asynchrone, il y a une méthode asychrone
          // dans read_message, donc elle seras placer dans io_context et
          // traité possiblement par un autre thread, contrairement aux
          // fonctions et action synchrone réalisé dans cette partie du code
          // qui seras exécuté dans le même thread
          read_message(clientconnected);
        }
        // Devras faire la gestion des erreurs ici
      });
}

// On envoie les messages à notre client
void ClientSession::send_message() {
  auto self =
      shared_from_this(); // Maintenir l'objet vivant pendant le callback

  if (!outgoingMessages.empty()) { // Vérifie si la file contient des messages
    Message msg;
    if (outgoingMessages.pop(msg)) { // Récupère le premier message de la file

      auto data = std::make_shared<std::string>(msg.getText());
      std::cout << "message : " << msg.getText() << std::endl;
      // Envoi du message de manière asynchrones
      boost::asio::async_write(*socket_, boost::asio::buffer(*data),
                               [this, self, data](boost::system::error_code ec,
                                                  std::size_t /*length*/) {
                                 if (!ec) {
                                   // Si l'envoi est réussi, vérifier s'il reste
                                   // d'autres messages
                                   send_message();
                                 } else {
                                   std::cerr
                                       << "Erreur d'écriture : " << ec.message()
                                       << std::endl;
                                 }
                               });
    }
  }
}

// Affiche les messages reçu à l'écran un part un
void ClientSession::handle_received_message(const Message &msg) {
  std::cout << "Message reçu de " << msg.getSender() << ": " << msg.getText()
            << std::endl;
  incomingMessages.push(msg);
}
// Push les messages dans la queue un par un
void ClientSession::queueMessage(const Message &msg) {
  bool wasEmpty = outgoingMessages.empty();
  std::cout << "Message ajouté à la file sortante : " << msg.getText()
            << std::endl;
  outgoingMessages.push(msg);
  if (wasEmpty) { // Si la file était vide, déclenche `send_message`
    auto self = shared_from_this();
    boost::asio::post(socket_->get_executor(),
                      [this, self]() { send_message(); });
  }
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