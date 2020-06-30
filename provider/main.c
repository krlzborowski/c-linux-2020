#define MAIN
#include "provider.h"

int main(int argc, char *argv[]) {

  char buff[BUF_SIZE];
  char* readPointer;
  int pid;
  float toSubstract;
  resource = 0.0f;
  srand(time(NULL));

  readParameters(argc, argv);
  setSignalHandlers();
  createTimer();

  for (ever) {

    if (read(0, buff, sizeof(buff))== -1)
      continue;
    toSubstract = -1.0;
    pid = -1;
    pid = (int)strtol(buff, &readPointer, 10);
    toSubstract = strtof(readPointer, NULL);

    if (pid != -1 && toSubstract != -1.0) {
      fprintf(stderr, "Provider:%d %f\n", pid, toSubstract);
      fflush(stderr);

      processRequest(pid, toSubstract);
    }

    pid = -1;
    toSubstract = -1.0;
  }

  timer_delete(timer);
  return 0;
}