#ifndef FAMILIADA
#define FAMILIADA

#if defined MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_POS 256

typedef struct dataStruct {
  char *signal;
  char *pace;
  char *endurance;
  char **positionals;

} DataStruct;

EXTERN DataStruct givenData;

int readParameters(int argc, char **argv);
void produceProcesses(int processesCount, int *pipeFd);
void produceParasites(int processesCount, pid_t pid, int *pipeFd);
int *setPipe();
void setFlag(int fd);

#endif