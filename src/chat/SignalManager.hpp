#ifndef PROJETOS_SIGNALMANAGER_HPP
#define PROJETOS_SIGNALMANAGER_HPP

#include <csignal>
#include <iostream>

enum {
    CODE_RETOUR_NORMAL,
    CODE_RETOUR_PARAMETRES_MANQUANTS,
    CODE_RETOUR_PSEUDO_TROP_LONG,
    CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES,
    CODE_RETOUR_ARRET_SIGINT,
    CODE_RETOUR_ERREUR_AUTRE
};

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

bool SignalManager::clientConnected=false;
bool SignalManager::showMemory=false;

#endif //PROJETOS_SIGNALMANAGER_HPP
