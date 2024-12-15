#include "SetUp.hpp"



// TODO : Faites de même si le pseudo est « . » ou « .. ».
static void VerifyEntries(int argc, char *argv[],OptionsProgramme *opt,std::string& name) {
    if (argc < 2) {
        fputs("chat pseudo_utilisateur [--bot] [--manuel]\n", stderr);
        exit(CODE_RETOUR_PARAMETRES_MANQUANTS);
    }

    const std::string charsInterdits = "/[]-";
    int index = 1;
    name += argv[index];
    index++;
    while (index<argc && argv[index][0] != '-') {
        name += ' ';
        name += argv[index];
        index++;
    }
    if (name.length() > MAX_PSEUDO ) {
        fprintf(stderr, "La longueur des pseudonymes ne peut excéder %d caractères.\n", MAX_PSEUDO);
        exit(CODE_RETOUR_PSEUDO_TROP_LONG);
    }

    for (const char &c: charsInterdits) {
        if (name.find(c) != std::string::npos) {
            fprintf(stderr, "Erreur : Le pseudonyme ne peut pas contenir des caractères interdits (/ ou [ ou ] ou -)\n");
            exit(CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES);
        }
    }
    std::string firstParam = (argc > index) ? argv[index] : "";
    std::string secondParam = (argc > index+1) ? argv[index+1] : "";
    opt->isBot = (firstParam == "--bot" || secondParam == "--bot");
    opt->isManuel = (firstParam == "--manuel" || secondParam == "--manuel");
}

static void SetIpAndPort(std::string &ip, int &port) {
    const char *ipEnv = std::getenv("IP_SERVEUR");
    const char *portEnv = std::getenv("PORT_SERVEUR");
    if (ipEnv!= nullptr) { // TODO checker si respecte le format IPv4
        ip = ipEnv;
    }
    if (portEnv != nullptr) {
        int portValue = std::stoi(portEnv);
        if (portValue>=1 && portValue<=65535) port=portValue;
    }
}

void SetUp(int argc, char *argv[],std::string& name, std::string &IP, int &PORT, OptionsProgramme *options) {
    VerifyEntries(argc, argv, options,name);
    SetIpAndPort(IP,PORT);
}

