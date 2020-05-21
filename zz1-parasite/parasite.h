#ifndef PARASITE
#define PARASITE

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

#define BUFF_SIZE 64

typedef struct dataStruct
{
  int signal;
  int pid;
  float requestsInterval;
  float initialRegisterValue;

} DataStruct;

DataStruct givenData;
float requestsRegister;

void readParameters(int argc, char *argv[]);
void sendRequest();
void signalHandler();
void sendReminder();
void checkResponse();
char *itostr(int i);

#endif