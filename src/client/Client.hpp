#pragma once
#include "SignalManager.hpp"
#include "codeserreurs.hpp"
#include <arpa/inet.h>
#include <atomic>
#include <cstring>
#include <mutex>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <unistd.h>

#define MAX_MEMOIRE 4096

class Client {
public:
  Client(std::string name_, bool modManuel, bool modBot, std::string addressIp,
         int port);
  void Connect();
  void SendMessage();
  void ReceiveMessage();
  void DisplayMessage(const char *message);
  void DisplayMemory();
  void FormatAndSetNickName(std::string &nick);
  void Disconnect();
  ~Client();

private:
  // Données personnelles
  std::string name_, nameClassic_, memory_;
  bool modManuel_;
  bool modBot_;

  // Données connexion
  std::string addressIP_;
  int port_;
  int socket_; // TCP-IPv4
  std::atomic<bool> connected{false};

  // Thread lecture, mutex et gestion des signaux
  std::mutex memoryMutex;  // Empêcher les accès concurrents à la mémoire
  std::mutex displayMutex; // Empêcher les accès concurrents lors de l'affichage
  pthread_t mainThreadID;
  std::thread readThread_; // Identifié au méthode ReceiveMessage()
  SignalManager signalManager;
};
