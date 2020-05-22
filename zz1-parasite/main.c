#include "parasite.h"

int main(int argc, char *argv[]) {
  struct timespec interval;

  givenData = (DataStruct){.signal = -1,
                           .pid = -1,
                           .requestsInterval = -1,
                           .initialRegisterValue = -1};
  readParameters(argc, argv);

  isSigPipe = 0;
  isConfirmation = 0;
  isResponse = 0;
  satisfiedRequestsCount = 0;
  remindersCount = 0;
  requestsRegister = givenData.initialRegisterValue;

  setSignalHandler();
  setInterval(&interval);

  while (1) {
    if (isSigPipe) {
      report();
      exit(141);
    }

    sendRequest();
    nanosleep(&interval, NULL);
    if (isConfirmation) {
      requestsRegister += requestsRegister / 4;
      satisfiedRequestsCount++;
      isConfirmation = 0;
    }
  }

  return 0;
}