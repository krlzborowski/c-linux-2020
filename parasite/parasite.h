#ifndef PARASITE
#define PARASITE

#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#define BUFF_SIZE 256
#define ever ;;

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
int responseCount;
int totalRemindersCount;
int lastSigRT;

void readParameters(int argc, char *argv[]);
void initGlobals();
void sendReminder();
void setSignalHandler();
void setSigRTHandler();
void signalHandler(int signal);
void setInterval(struct timespec *timeInterval);
void report(int pid);

#endif