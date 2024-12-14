#include "SetUp.hpp"


static void SetNickName(char* nick,bool bot) {
    std::string newNick ="";
    if (bot) newNick="["+std::string(nick)+"]";
    else newNick="\x1B[4m["+std::string(nick)+"]\x1B[0m";
    strncpy(nick, newNick.c_str(), strlen(nick));
};

// TODO : A REFAIRE COMPLETEMENT
static char* VerifyEntries(int argc, char* argv[]) {
    char* name;

    if (argc < 2) {
        fputs("chat pseudo_utilisateur [--bot] [--manuel]\n", stderr);
        exit(CODE_RETOUR_PARAMETRES_MANQUANTS);
    }

    if (strlen(argv[1]) > MAX_PSEUDO_DESTINATAIRE || strlen(argv[2]) > MAX_PSEUDO_DESTINATAIRE) {
        fprintf(stderr, "La longueur des pseudonymes ne peut excéder %d caractères.\n", MAX_PSEUDO_DESTINATAIRE);
        exit(CODE_RETOUR_PSEUDO_TROP_LONG);
    }

    const char caracteresInterdits[] = { '/', '-', '[', ']' };

    for (int i = 1; i < 3; ++i) {
        for (unsigned int j = 0; j < sizeof(caracteresInterdits) / sizeof(*caracteresInterdits); ++j) {
            if (strchr(argv[i], caracteresInterdits[j]) != NULL) {
                fprintf(stderr, "Le caractère '%c' n'est pas autorisé dans un pseudonyme.\n", caracteresInterdits[j]);
                exit(CODE_RETOUR_PSEUDO_CARACTERES_INVALIDES);
            }
        }
    }
    return name;
}




// TODO : A REFAIRE COMPLETEMENT
void SetUp(int argc, char* argv[]) {
    const char* ipServer = std::getenv("IP_SERVEUR");
    const char* portServer = std::getenv("PORT_SERVEUR");
    char* name;
    std::string firstParam = (argc > 3) ? argv[3] : "";
    std::string secondParam = (argc > 4) ? argv[4] : "";
    bool isBot = (firstParam == "--bot" || secondParam == "--bot");
    bool isManuel = (firstParam == "--manuel" || secondParam == "--manuel");

    name = VerifyEntries(argc, argv);
    SetNickName(name,isBot);
}

