#include "parasite.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  givenData = (DataStruct){.signal = -1,
                           .pid = -1,
                           .requestsInterval = -1,
                           .initialRegisterValue = -1};
  readParameters(argc, argv);
  requestsRegister = givenData.initialRegisterValue;
  sendRequest();
  return 0;
}