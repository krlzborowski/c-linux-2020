#include "parasite.h"

void readParameters(int argc, char *argv[]) {
  int opt;

  while ((opt = getopt(argc, argv, "s:p:d:v:")) != -1) {
    char *p;
    errno = 0;
    switch (opt) {
    case 's':

      givenData.signal = (int)strtol(optarg, &p, 10);

      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong signal format\n");
        exit(EXIT_FAILURE);
      }

      if (givenData.signal == SIGPIPE) {
        fprintf(stderr, "Can't use SIGPIPE\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'p':
      givenData.pid = (int)strtol(optarg, &p, 10);
      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong pid format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'd':
      givenData.requestsInterval = strtof(optarg, &p);
      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong interval format\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'v':
      givenData.initialRegisterValue = strtof(optarg, &p);
      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong register value format\n");
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

void sendReminder() {
  int signal = (rand() % (SIGRTMAX - SIGRTMIN)) + SIGRTMIN;
  union sigval sv;
  sv.sival_ptr = NULL;
  lastSigRT = signal;
  setSigRTHandler();
  sigqueue(givenData.pid, signal, sv);
  remindersCount++;
  totalRemindersCount++;
}

void setSignalHandler() {
  struct sigaction signalAction;
  memset(&signalAction, '\0', sizeof(signalAction));
  signalAction.sa_flags = 0;
  signalAction.sa_handler = signalHandler;
  if (sigaction(SIGPIPE, &signalAction, NULL) == -1) {
    perror("Sigaction error");
    exit(EXIT_FAILURE);
  }

  if (sigaction(givenData.signal, &signalAction, NULL) == -1) {
    perror("Sigaction error");
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

void report(int pid) {
  fprintf(stderr, "\nPID                  %d\n", pid);
  fprintf(stderr, "\nSatisfied requests   %d", satisfiedRequestsCount);
  fprintf(stderr, "\nTotal sent reminders %d", totalRemindersCount);
  fprintf(stderr, "\nRequests value       %f", requestsRegister);
  fprintf(stderr, "\nSent reminders       %d", remindersCount);
  fprintf(stderr, "\nResponses            %d", responseCount);
  fprintf(stderr, "\nConfirmed            %d", isConfirmation);
  fflush(stderr);
}