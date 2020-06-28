#define MAIN
#include "provider.h"

int main(int argc, char *argv[]) {
  
  int pid = -1;
  float toSubstract = -1.0;
  resource = 0.0f;
  srand(time(NULL));

  readParameters(argc, argv);
  setSignalHandlers();
  createTimer();
  for (ever) {
    scanf("%d %f", &pid, &toSubstract);
    processRequest(pid, givenData.signal);
  }
  timer_delete(timer);
  return 0;
}