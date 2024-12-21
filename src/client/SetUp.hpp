#pragma once
#include <arpa/inet.h> // inet_pton
#include <cstdlib>
#include <string>
#define MAX_PSEUDO 30

typedef struct {
  bool isBot;
  bool isManuel;
} OptionsProgramme;

void SetUp(int argc, char *argv[], std::string &name, std::string &IP,
           int &PORT, OptionsProgramme *options);
