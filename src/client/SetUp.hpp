#ifndef PROJETOS_SETUP_HPP
#define PROJETOS_SETUP_HPP

#include <string>
#include <cstdlib>
#include <cstdio>
#include <arpa/inet.h> // Pour inet_pton
#define MAX_PSEUDO 30

typedef struct {
    bool isBot;
    bool isManuel;
} OptionsProgramme;


void SetUp(int argc, char* argv[],std::string& name,std::string &IP,int &PORT, OptionsProgramme* options);

#endif //PROJETOS_SETUP_HPP
