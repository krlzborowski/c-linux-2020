#include "provider.h"

int main(int argc, char *argv[]) {
  int status = -1;
  int pid = -1;
  float toSubstract = -1.0;
  char buf[BUF_SIZE];
  resource = 0.0f;
  
  readParameters(argc, argv);
  createTimer();
  for (ever) {
    scanf("%d %f", &pid, &toSubstract);
    printf("%d %f", pid, toSubstract);
  }
  timer_delete(timer);
  return 0;
}