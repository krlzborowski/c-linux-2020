#define MAIN

#include "familiada.h"

int main(int arg, char *argv[]) {
  int processesCount;
  int *pipeFd;
  givenData.positionals = malloc(MAX_POS * sizeof(char *));

  processesCount = readParameters(arg, argv);

  pipeFd = setPipe();

  produceProcesses(processesCount, pipeFd);
  return 0;
}