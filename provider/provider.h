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

typedef struct dataStruct {
  int signal;
  float incrementValue;
  float interval;
  float notKilledPercent;
  float sendResponsePercent;

} DataStruct;

DataStruct givenData;
float resource;
timer_t timer;

void readParameters(int argc, char *argv[]);
void createTimer();
void setTimer();
void harvest(union sigval sv);
void processRequest(int pid, float toSubstract);
// void setSignalHandler();
// void signalHandler();
// void timerHandler();

#endif