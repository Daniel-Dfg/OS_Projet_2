#pragma once
#include <iostream>
#include <string>

enum CodesRetour{
    NORMAL,
    PARAMETRES_MANQUANTS,
    PSEUDO_TROP_LONG,
    PSEUDO_CARACTERES_INVALIDES,
    ARRET_SIGINT,
    ERREUR_AUTRE
};

inline int check_return_value(int value, std::string elem) {
    if (value < 0) {
        std::cerr << "Error : " << elem << std::endl;
        exit(value);
    }
    return value;
}
