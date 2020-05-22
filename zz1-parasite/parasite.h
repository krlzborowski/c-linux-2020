#ifndef PARASITE
#define PARASITE

#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>

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
int isSigPipe;
int isConfirmation;
int isResponse;
int satisfiedRequestsCount;
int remindersCount;
char *lastWhim;

void readParameters(int argc, char *argv[]);
void sendRequest();
void sendReminder();
// TODO
void checkResponse();

char *itostr(int i);
void setSignalHandler();
// TODO
void setSigRTHandler(int rtSignal);

void signalHandler(int signal);
void setInterval(struct timespec* timeInterval);
void report();

#endif