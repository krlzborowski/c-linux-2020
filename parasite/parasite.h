#ifndef PARASITE
#define PARASITE

#if defined MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

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

EXTERN DataStruct givenData;
EXTERN float requestsRegister;
EXTERN int isSigPipe;
EXTERN int isConfirmation;
EXTERN int isResponse;
EXTERN int satisfiedRequestsCount;
EXTERN int remindersCount;
EXTERN int responseCount;
EXTERN int totalRemindersCount;
EXTERN int lastSigRT;

void readParameters(int argc, char *argv[]);
void initGlobals();
void sendReminder();
void setSignalHandler();
void setSigRTHandler();
void signalHandler(int signal);
void setInterval(struct timespec *timeInterval);
void report(int pid);

#endif