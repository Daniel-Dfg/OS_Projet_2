#include "SignalManager.hpp"
#include "codeserreurs.hpp"

bool SignalManager::clientConnected=false;
bool SignalManager::showMemory=false;

void SignalManager::initSignalHandling(bool manuel) {
    struct sigaction actions_signaux{};  // Structure pour configurer les actions des signaux

    // Définit le gestionnaire de signaux en fonction du mode
    manuel ?
        actions_signaux.sa_handler = SignalManager::signalHandlerManuel :
        actions_signaux.sa_handler = SignalManager::signalHandler;

    sigemptyset(&actions_signaux.sa_mask);  // aucun signal bloqué
    actions_signaux.sa_flags = 0;           // Aucun drapeau supplémentaire

    // Associe les signaux aux gestionnaires
    sigaction(SIGINT, &actions_signaux, nullptr); // Gère l'interruption (Ctrl+C)
    sigaction(SIGPIPE, &actions_signaux, nullptr);
    sigaction(SIGUSR1, &actions_signaux, nullptr);  // Signal personalisé qui indique la fin du programme
    sigaction(SIGHUP, &actions_signaux, nullptr);  // Si l'on quitte le terminal
}

void SignalManager::signalHandler(int signalSent) {
    if (signalSent == SIGINT || signalSent == SIGPIPE) {
        if (!clientConnected) {
            exit(ARRET_SIGINT);
        } else {
            clientConnected= false;
            exit(NORMAL);
        }
    }
}

void SignalManager::signalHandlerManuel(int signalSent) {
    if (signalSent == SIGINT) {
        if (clientConnected) {
            showMemory = true;
        } else {
            exit(ARRET_SIGINT);
        }
    }
    else if (signalSent == SIGPIPE){
        clientConnected= false;
    }
}
