#include "parasite.h"
#include <stdlib.h>

void readParameters(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "SpdT")) != -1) {
    char *p;
    switch (opt) {
    case 's':
      givenData.signal = (int)strtol(argv[optind], &p, 10);
      if (*p != '\0') {
        fprintf(stderr, "Wrong number of connections format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'p':
      givenData.pid = (int)strtol(argv[optind], &p, 10);
      if (*p != '\0') {
        fprintf(stderr, "Wrong number of connections format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'd':
      givenData.requestsInterval = (float)strtol(argv[optind], &p, 10);
      if (*p != '\0') {
        fprintf(stderr, "Wrong number of connections format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'v':
      givenData.initialRegisterValue = (float)strtol(argv[optind], &p, 10);
      if (*p != '\0') {
        fprintf(stderr, "Wrong number of connections format\n");
        exit(EXIT_FAILURE);
      }
      break;

    default:
      fprintf(stderr, "Usage: %s -s <signal> -p <pid> -d <\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}
