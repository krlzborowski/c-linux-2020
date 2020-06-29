#ifndef PROVIDER
#define PROVIDER

#if defined MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define ever ;;
#define BUF_SIZE 128
#define SIGRT_NUM SIGRTMAX-SIGRTMIN+1

typedef struct dataStruct {
  int signal;
  float incrementValue;
  float interval;
  float notKillingPercent;
  float sendResponsePercent;

} DataStruct;

EXTERN DataStruct givenData;
EXTERN float resource;
EXTERN timer_t timer;
EXTERN int isAnsweringSignal;

void readParameters(int argc, char *argv[]);
void createTimer();
void setTimer();
void onTimer(union sigval sv);
void processRequest(int pid, float toSubstract);
void setSignalHandlers();
void answer(int signal, siginfo_t *info, void *ucontext);

#endif