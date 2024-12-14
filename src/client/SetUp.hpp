#ifndef PROJETOS_SETUP_HPP
#define PROJETOS_SETUP_HPP
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_PSEUDO_DESTINATAIRE 30

enum { // temp
    CODE_RETOUR_NORMAL,
    CODE_RETOUR_PARAMETRES_MANQUANTS,
    CODE_RETOUR_PSEUDO_TROP_LONG,
    CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES,
    CODE_RETOUR_ARRET_SIGINT,
    CODE_RETOUR_ERREUR_AUTRE
};

typedef struct {
    bool isBot;
    bool isManuel;
} OptionsProgramme;


void SetUp(int argc, char* argv[],std::string& name,std::string &ipServer,int &portServer, OptionsProgramme* options);

#endif //PROJETOS_SETUP_HPP
