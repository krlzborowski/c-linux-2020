#include "parasite.h"
#include <stdlib.h>
#include <time.h>
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
      if (givenData.signal == SIGPIPE) {
        fprintf(stderr, "Can't use SIGPIPE\n");
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
  char *buffer = calloc(BUFF_SIZE, sizeof(char));
  char *requestsString = calloc(16, sizeof(char));

  strcpy(buffer, itostr(givenData.pid));
  strcat(buffer, " ");
  strcat(buffer, gcvt(requestsRegister, 5, requestsString));
  write(1, buffer, sizeof(buffer));
  write(1, "\n", sizeof(char));
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

void setSignalHandler() {
  struct sigaction signalAction;
  signalAction.sa_flags = 0;
  signalAction.sa_handler = signalHandler;
  if (sigaction(SIGPIPE, &signalAction, NULL) == -1) {
    perror("Sigaction error:");
    exit(EXIT_FAILURE);
  }
  if (sigaction(givenData.signal, &signalAction, NULL) == -1) {
    perror("Sigaction error:");
    exit(EXIT_FAILURE);
  }
}

void signalHandler(int signal) {

  if (signal == givenData.signal)
    isConfirmation = 1;
  else if (signal == SIGPIPE) {
    isSigPipe = 1;
  }
}

void setInterval(struct timespec *timeInterval) {
  float seconds = givenData.requestsInterval / 10;
  timeInterval->tv_sec = (time_t)seconds;
  timeInterval->tv_nsec = (seconds - (float)timeInterval->tv_sec) * 1000000000;
}

void report() {
  char buf[BUFF_SIZE];
  strcpy(buf, "PID:\t");
  strcat(buf, itostr(getpid()));
  write(2, buf, sizeof(buf));
  write(2, "\n", sizeof(char));
  strcpy(buf, "Satisfied requests:\t");
  strcat(buf, itostr(satisfiedRequestsCount));
  write(2, buf, sizeof(buf));
  write(2, "\n", sizeof(char));
}