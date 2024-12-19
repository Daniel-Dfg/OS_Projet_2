#ifndef PROJETOS_CODESERREURS_HPP
#define PROJETOS_CODESERREURS_HPP
#include <iostream>
#include <string>

enum {
    CODE_RETOUR_NORMAL,
    CODE_RETOUR_PARAMETRES_MANQUANTS,
    CODE_RETOUR_PSEUDO_TROP_LONG,
    CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES,
    CODE_RETOUR_ARRET_SIGINT,
    CODE_RETOUR_ERREUR_AUTRE
};

inline int check_return_value(int value, std::string elem = "???") {
    if (value < 0) {
        std::cerr << "Error : " << elem << std::endl;
        exit(1);
    }
    return value;
}



#endif //PROJETOS_CODESERREURS_HPP
