#pragma once
#include <csignal>

class SignalManager {
public:
  static void initSignalHandling(bool manuel);
  static void signalHandler(int signalSent); // Gestionnaire de signaux standard
  static void signalHandlerManuel(int signalSent); // Gestionnaire mode manuel

  static bool clientConnected;
  static bool showMemory;
};
