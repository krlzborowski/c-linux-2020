#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

typedef struct dataStruct
{
  int signal;
  int pid;
  float requestsInterval;
  float initialRegisterValue;

} DataStruct;

DataStruct givenData = {.signal=-1, .pid=-1, .requestsInterval=-1, .initialRegisterValue=-1};

void readParameters(int argc, char **argv);
void sendRequest();
void signalHandler();
void sendReminder();
void checkResponse();
