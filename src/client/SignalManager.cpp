#include "SignalManager.hpp"
#include "codeserreurs.hpp"

bool SignalManager::clientConnected=false;
bool SignalManager::showMemory=false;

// Initialise la gestion des signaux pour le programme
void SignalManager::initSignalHandling(bool manuel) {
    struct sigaction sa{};  // Structure pour configurer les actions des signaux

    // Définit le gestionnaire de signaux en fonction du mode
    if (!manuel) sa.sa_handler = SignalManager::signalHandler;  // Mode normal
    else sa.sa_handler = SignalManager::signalHandlerManuel;  // Mode manuel

    sigemptyset(&sa.sa_mask);  // Vide le masque de signaux (aucun bloqué)
    sa.sa_flags = 0;           // Aucun drapeau supplémentaire

    // Associe les signaux aux gestionnaires
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGPIPE, &sa, nullptr);// Gère l'interruption (Ctrl+C)
    sigaction(SIGUSR1, &sa, nullptr);  // Signal personalisé qui indique la fin du programme
    sigaction(SIGHUP, &sa, nullptr);  // Si l'on quitte le terminal
}

// Gestionnaire de signaux pour le mode normal
void SignalManager::signalHandler(int signalSent) {
    if (signalSent == SIGINT || signalSent == SIGPIPE) {
        if (!clientConnected) {
            /*Tant que la connexion (fonction connect()) n’a pas été établie, le signal SIGINT doit
            terminer proprement chat (et donc à la fois le processus d’origine et le second) avec le
            code de retour 4 */
            exit(CODE_RETOUR_ARRET_SIGINT);
        } else {
            clientConnected= false;
            exit(CODE_RETOUR_NORMAL);
        }
    }
}

// Gestionnaire de signaux pour le mode manuel
void SignalManager::signalHandlerManuel(int signalSent) {
    if (signalSent == SIGINT) {
        if (clientConnected) {
            showMemory = true;
        } else {
            exit(CODE_RETOUR_ARRET_SIGINT);
        }
    }
    else if (signalSent == SIGPIPE){
        clientConnected= false;
    }
}
