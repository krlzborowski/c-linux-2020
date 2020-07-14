#define MAIN
#include "provider.h"

int main(int argc, char *argv[]) {

  char buff[BUF_SIZE];
  char *readPointer;
  int pid;
  float toSubstract;
  resource = 0.0f;
  srand(time(NULL));

  readParameters(argc, argv);
  setSignalHandlers();
  createTimer();

  for (ever) {
    if (read(0, buff, sizeof(buff)) == -1)
      continue;

    fprintf(stderr, "Provider: confirmation received\n");
    fflush(stderr);
    pid = -1;
    toSubstract = -1.0;
    pid = (int)strtol(buff, &readPointer, 10);
    toSubstract = strtof(readPointer, NULL);

    if (pid != -1 && toSubstract != -1.0) {
      processRequest(pid, toSubstract);
    }
  }

  timer_delete(timer);
  return 0;
}