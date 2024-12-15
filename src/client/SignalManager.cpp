#include "SignalManager.hpp"

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
    sigaction(SIGINT, &sa, nullptr);   // Gère l'interruption (Ctrl+C)
    sigaction(SIGUSR1, &sa, nullptr);  // Signal personalisé qui indique la fin du programme
    sigaction(SIGHUP, &sa, nullptr);  // Si l'on quitte le terminal
}

// Gestionnaire de signaux pour le mode normal
void SignalManager::signalHandler(int signalSent) {
    if (signalSent == SIGINT) {
        if (!clientConnected) {
            /*Tant que la connexion (fonction connect()) n’a pas été établie, le signal SIGINT doit
            terminer proprement chat (et donc à la fois le processus d’origine et le second) avec le
            code de retour 4 */
            exit(CODE_RETOUR_ARRET_SIGINT);
        } else {
            exit(CODE_RETOUR_NORMAL);
        }
    }
}

// Gestionnaire de signaux pour le mode manuel
void SignalManager::signalHandlerManuel(int signalSent) {
    if (signalSent == SIGINT) {
        if (clientConnected) {
            /*Si la connexion a été établie et que l’option --manuel est activée, alors la réception du
         signal SIGINT par le thread d’origine doit désormais afficher les messages en attente
         conservés en mémoire*/
            showMemory = true;
        } else {
            exit(CODE_RETOUR_ARRET_SIGINT);
        }
    }
}