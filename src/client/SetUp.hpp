#ifndef PROJETOS_SETUP_HPP
#define PROJETOS_SETUP_HPP
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "codeserreurs.hpp"
#define MAX_PSEUDO 30

typedef struct {
    bool isBot;
    bool isManuel;
} OptionsProgramme;


void SetUp(int argc, char* argv[],std::string& name,std::string &IP,int &PORT, OptionsProgramme* options,std::string& realname);

#endif //PROJETOS_SETUP_HPP
