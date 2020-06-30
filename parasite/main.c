#define MAIN

#include "parasite.h"
#include <time.h>

int main(int argc, char *argv[]) {


  
  int pid = getpid();
  struct timespec interval;
  struct timespec remain;
  srand(time(NULL));

  givenData = (DataStruct){.signal = -1,
                           .pid = -1,
                           .requestsInterval = -1,
                           .initialRegisterValue = -1};
  readParameters(argc, argv);
  initGlobals();
  setSignalHandler();
  setInterval(&interval);

  fprintf(stderr, "Parasite ready\n");
  fflush(stderr);

  for(ever) {
    if (isSigPipe) {
      report(pid);
      exit(141);
    }

    fprintf(stdout, "%d %f\n", pid, requestsRegister);
    fflush(stdout);
    nanosleep(&interval, &remain);

    if (isConfirmation) {
      requestsRegister += requestsRegister / 4;
      satisfiedRequestsCount++;
      isConfirmation = 0;
    } else {

      sendReminder();
      if (isResponse) {
        requestsRegister -= requestsRegister / 5;
        responseCount++;
        remindersCount = 0;
        isResponse = 0;
      }
    }

    nanosleep(&remain, NULL);
  }

  return 0;
}