#include "parasite.h"

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

void initGlobals() {
  isSigPipe = 0;
  isConfirmation = 0;
  isResponse = 0;
  satisfiedRequestsCount = 0;
  remindersCount = 0;
  totalRemindersCount = 0;
  requestsRegister = givenData.initialRegisterValue;
}

void sendRequest() {
  char buffer[BUFF_SIZE] = {0};
  char requestsString[BUFF_SIZE] = {0};

  strcpy(buffer, itostr(givenData.pid));
  strcat(buffer, " ");
  write(1, buffer, sizeof(buffer));

  gcvt(requestsRegister, 5, requestsString);
  strcat(requestsString, "\n");
  write(1, requestsString, sizeof(requestsString));
}

void sendReminder() {
  int signal = (rand() % (SIGRTMAX - SIGRTMIN)) + SIGRTMIN;
  union sigval sv;
  sv.sival_ptr = NULL;
  if (sigqueue(givenData.pid, signal, sv) == -1) {
    perror("Sigque failure");
    exit(EXIT_FAILURE);
  }
  remindersCount++;
  totalRemindersCount++;
  lastSigRT = signal;
  setSigRTHandler();
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
  memset(&signalAction, '\0', sizeof(signalAction));
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

void setSigRTHandler() {
  struct sigaction signalAction;
  sigemptyset(&signalAction.sa_mask);
  signalAction.sa_flags = SA_RESETHAND;
  signalAction.sa_handler = signalHandler;
  if (sigaction(lastSigRT, &signalAction, NULL) == -1) {
    perror("Sigaction error:");
    exit(EXIT_FAILURE);
  }
}

void signalHandler(int signal) {

  if (signal == givenData.signal) {
    isConfirmation = 1;
  } else if (signal == SIGPIPE) {
    isSigPipe = 1;
  } else if (signal == lastSigRT) {
    isResponse = 1;
  }
}

void setInterval(struct timespec *timeInterval) {
  float seconds = givenData.requestsInterval / 10;
  timeInterval->tv_sec = (time_t)seconds;
  timeInterval->tv_nsec = (seconds - (float)timeInterval->tv_sec) * 1000000000;
}

void report() {
  char buf[BUFF_SIZE] = {0};
  char requestsString[8];
  strcpy(buf, "\nPID                  ");
  strcat(buf, itostr(getpid()));
  strcat(buf, "\nSatisfied requests   ");
  strcat(buf, itostr(satisfiedRequestsCount));
  strcat(buf, "\nTotal sent reminders ");
  strcat(buf, itostr(totalRemindersCount));
  strcat(buf, "\nRequests value       ");
  strcat(buf, gcvt(requestsRegister, 8, requestsString));
  strcat(buf, "\nSent reminders       ");
  strcat(buf, itostr(remindersCount));
  strcat(buf, "\nResponses            ");
  strcat(buf, itostr(responseCount));
  strcat(buf, "\nConfirmed            ");
  strcat(buf, itostr(isConfirmation));
  strcat(buf, "\n");
  write(2, buf, sizeof(buf));
}