#ifndef PROJETOS_SIGNALMANAGER_HPP
#define PROJETOS_SIGNALMANAGER_HPP

#include <csignal>


// Classe SignalManager
// Gère les signaux système pour contrôler le comportement du programme
class SignalManager {
public:
    // Initialise la gestion des signaux
    static void initSignalHandling(bool manuel);

    // Gestionnaire de signaux standard (mode normal)
    static void signalHandler(int signalSent);

    // Gestionnaire de signaux pour le mode manuel
    static void signalHandlerManuel(int signalSent);

    static bool clientConnected;
    static bool showMemory;
};


#endif //PROJETOS_SIGNALMANAGER_HPP
