#include "SetUp.hpp"
#include "codeserreurs.hpp"

static void VerifyEntries(int argc, char *argv[], OptionsProgramme *opt,
                          std::string &name) {
  const std::string charsInterdits = "/[]-";
  int argv_index = 1;

  if (argc < 2) {
    std::cerr << "chat pseudo_utilisateur [--bot] [--manuel]\n";
    exit(CodesRetour::PARAMETRES_MANQUANTS); // Code retour 1
  }
  name += argv[argv_index]; // Premier mot est toujours le pseudo même si c'est une
                       // option ex: --manuel
  argv_index++;

  while (argv_index < argc &&
         argv[argv_index][0] != '-') { // Tant que le mot ne commence pas par un "-"
                                  // on rajoute au pseudo
    name += ' ';
    name += argv[argv_index];
    argv_index++;
  }

  if (name.length() > MAX_PSEUDO) {
    std::cerr << "La longueur des pseudonymes ne peut excéder " << MAX_PSEUDO
              << " caractères.\n";
    exit(CodesRetour::PSEUDO_TROP_LONG); // 2
  }

  for (const char &c : charsInterdits) {
    if (name.find(c) != std::string::npos) {
      std::cerr << "Erreur : Le pseudonyme ne peut pas contenir des caractères "
                   "interdits (/ ou [ ou ] ou -)\n";
      exit(CodesRetour::PSEUDO_CARACTERES_INVALIDES); // 3
    }
    if (name == "." || name == "..") {
      std::cerr << "Erreur : Le pseudonyme ne peut pas être '.' ou '..' \n";
      exit(CodesRetour::PSEUDO_CARACTERES_INVALIDES);
    }
  }
  std::string firstParam = (argc > argv_index) ? argv[argv_index] : "";
  std::string secondParam = (argc > argv_index + 1) ? argv[argv_index + 1] : "";
  opt->isBot = (firstParam == "--bot" || secondParam == "--bot");
  opt->isManuel = (firstParam == "--manuel" || secondParam == "--manuel");
}

static void SetIpAndPort(std::string &ip, int &port) {
  const char *ipEnv = std::getenv("IP_SERVEUR");
  const char *portEnv = std::getenv("PORT_SERVEUR");
  if (ipEnv != nullptr) {
    struct in_addr addr;
    if (inet_pton(AF_INET, ipEnv, &addr) == 1) {
      ip = ipEnv;
    }
  }
  if (portEnv != nullptr) {
    try {
      int portValue = std::stoi(portEnv);
      if (portValue >= 1 && portValue <= 65535)
        port = portValue;
    } catch (const std::exception &e) {
      // Ignorer les erreurs de conversion
    }
  }
}

void SetUp(int argc, char *argv[], std::string &name, std::string &IP,
           int &PORT, OptionsProgramme *options) {
  VerifyEntries(argc, argv, options, name);
  SetIpAndPort(IP, PORT);
}
