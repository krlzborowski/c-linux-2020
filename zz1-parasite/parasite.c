#include "parasite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void readParameters(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "spdv")) != -1) {
    char *p;
    switch (opt) {
    case 's':
      givenData.signal = (int)strtol(argv[optind], &p, 10);
      if (*p != '\0') {
        fprintf(stderr, "Wrong signal format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'p':
      givenData.pid = (int)strtol(argv[optind], &p, 10);
      if (*p != '\0') {
        fprintf(stderr, "Wrong PID format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'd':
      givenData.requestsInterval = (float)strtof(argv[optind], &p);
      if (*p != '\0') {
        fprintf(stderr, "Wrong requests interval format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'v':
      givenData.initialRegisterValue = (float)strtof(argv[optind], &p);
      if (*p != '\0') {
        fprintf(stderr, "Wrong initial register value format\n");
        exit(EXIT_FAILURE);
      }
      break;

    default:
      fprintf(
          stderr,
          "Usage: %s -s <signal> -p <pid> -d <interval> -v <initial value>\n",
          argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "Expected arguments\n");
    exit(EXIT_FAILURE);
  }

  if (givenData.signal == -1 || givenData.pid == -1 ||
      givenData.requestsInterval == -1 ||
      givenData.initialRegisterValue == -1) {
    fprintf(stderr,
            "Usage: %s -s <signal> -p <pid> -d <interval> -v <initial value>\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }
}

void sendRequest() {
  char *buffer = itostr(givenData.pid);
  strcat(buffer, " ");
  char *requestsString = malloc(BUFF_SIZE * sizeof(char));
  strcat(buffer, gcvt(requestsRegister, 10, requestsString));
  strcat(buffer, "\n");
  write(1, buffer, sizeof(buffer));
  
}

char *itostr(int x) {
  int i = x;
  char buf[BUFF_SIZE];
  char *p = &buf[sizeof buf - 1];
  *p = '\0';
  if (i >= 0) {
    i = -i;
  }
  do {
    p--;
    *p = (char)('0' - i % 10);
    i /= 10;
  } while (i);
  if (x < 0) {
    p--;
    *p = '-';
  }
  size_t len = (size_t)(&buf[sizeof buf] - p);
  char *s = malloc(len);
  if (s) {
    memcpy(s, p, len);
  }
  return s;
}