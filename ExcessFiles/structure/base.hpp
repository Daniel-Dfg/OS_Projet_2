#include <cstdio>
#include <queue>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <thread>
#include <unistd.h> // Pour usleep()
#include <poll.h>
#include <unordered_map>

using std::string, std::unordered_map;

class Message {
  string sender, receiver, text;
  ssize_t size;

public:
  Message(string &sender_, string &text) {
    this->sender = sender_;
    std::stringstream ss(text);
    if (getline(ss, receiver, ' ') && getline(ss, text)) {
      this->size = this->text.length();
    } else {
      // Traitement pour message invalide...
    }
  }
  Message() {}
  string getSender() const { return sender; }
  string getReceiver() const { return receiver; }
  string getText() const { return text; }
};

class MessageQueue {
private:
  std::queue<Message> messages;
  pthread_mutex_t mtx;

public:
  MessageQueue() { pthread_mutex_init(&mtx, NULL); }

  ~MessageQueue() { pthread_mutex_destroy(&mtx); }

  void push(Message msg) {
    pthread_mutex_lock(&mtx);
    messages.push(msg);
    pthread_mutex_unlock(&mtx);
  }

  bool pop(Message &msg) {
    pthread_mutex_lock(&mtx);
    if (messages.empty()) {
      pthread_mutex_unlock(&mtx);
      return false;
    }
    msg = messages.front();
    messages.pop();
    pthread_mutex_unlock(&mtx);
    return true;
  }
};

class Client {
private:
  string name;
  std::thread senderThread;
  std::thread receiverThread;
  MessageQueue outgoingMessages;
  MessageQueue incomingMessages;
  bool running;

  void senderLoop() {
    while (running) {
      Message msg;
      if (outgoingMessages.pop(msg)) {
        // Envoyer message...
      }
      // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  void receiverLoop() {
    while (running) {
      // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

public:
  Client(string name_) : name(name_), running(true) {
    senderThread = std::thread(&Client::senderLoop, this);
    receiverThread = std::thread(&Client::receiverLoop, this);
  }

  void sendMessage(string text) {
    Message msg(name, text);
    outgoingMessages.push(msg);
  }

  void stop() {
    running = false;
    if (senderThread.joinable())
      senderThread.join();
    if (receiverThread.joinable())
      receiverThread.join();
  }

  ~Client() { stop(); }
};

class Server{
    static constexpr int MAX_CLIENTS = 1000;
    static int server_fd;
    static std::vector<pollfd> poll_fds;
    static unordered_map<string, pollfd> clients_fds;


    //Pour chaque connexion entrante :
        //Prendre en compte le poll


};
