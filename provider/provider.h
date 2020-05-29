#ifndef PROVIDER
#define PROVIDER

#include <bits/types/struct_itimerspec.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

DataStruct givenData;
float resource;
timer_t timer;
int isAnsweringSignal;

void readParameters(int argc, char *argv[]);
void createTimer();
void setTimer();
void onTimer(union sigval sv);
void processRequest(int pid, float toSubstract);
void setSignalHandlers();
void answer(int signal, siginfo_t *info, void *ucontext);

#endif