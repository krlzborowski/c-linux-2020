#include "provider.h"
#include <bits/types/struct_itimerspec.h>
#include <csignal>

void readParameters(int argc, char *argv[]) {
  int opt;
  char *p;
  while ((opt = getopt(argc, argv, "sh")) != -1) {
    switch (opt) {
    case 's':
      givenData.signal = (int)strtol(argv[optind], &p, 10);
      if (*p != '\0') {
        fprintf(stderr, "Wrong signal format\n");
        exit(EXIT_FAILURE);
      }
      break;

    case 'h':
      givenData.incrementValue = strtof(argv[optind], &p);
      if (*p != '/') {
        fprintf(stderr, "Wrong pace format\n");
        exit(EXIT_FAILURE);
      }

      p++;
      givenData.interval = strtof(p, &p);

      if (*p != '\0') {
        fprintf(stderr, "Wrong pace format\n");
        exit(EXIT_FAILURE);
      }
      break;

    default:
      fprintf(stderr,
              "Usage: %s <float>/<float> -s <int> -h <float>/<float> \n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "Expected arguments\n");
    exit(EXIT_FAILURE);
  }

  givenData.notKilledPercent = strtof(argv[optind], &p);

  if (*p != '/') {
    fprintf(stderr, "Wrong RT tolerancy format\n");
    exit(EXIT_FAILURE);
  }

  p++;
  givenData.sendResponsePercent = strtof(p, &p);

  if (*p != '\0') {
    fprintf(stderr, "Usage: %s <float>/<float> -s <int> -h <float>/<float> \n",
            argv[0]);
    exit(EXIT_FAILURE);
  }
}

void createTimer() {
  struct sigevent signalEvent;
  memset(&signalEvent, 0, sizeof(struct sigevent));
  signalEvent.sigev_notify = SIGEV_THREAD;
  signalEvent.sigev_notify_function = &harvest;
  if (timer_create(CLOCK_REALTIME, &signalEvent, &timer) == -1) {
    perror("Timer creation failure");
    exit(EXIT_FAILURE);
  }
  setTimer();
}

void setTimer() {
  struct itimerspec ts;
  memset(&ts, 0, sizeof(struct itimerspec));
  float seconds = givenData.interval;
  ts.it_value.tv_sec = (time_t)seconds;
  ts.it_value.tv_nsec = (seconds - (float)ts.it_value.tv_sec) * 1000000000;
  ts.it_interval.tv_nsec = ts.it_value.tv_nsec;
  ts.it_interval.tv_sec = ts.it_value.tv_sec;
  if (timer_settime(timer, 0, &ts, NULL) == -1) {
    perror("Timer setting failure");
    exit(EXIT_FAILURE);
  }
}

void harvest(union sigval sv) { resource += givenData.incrementValue; }

void processRequest(int pid, float toSubstract) {
  if (toSubstract > 0 && toSubstract <= resource) {
    resource -= toSubstract;
    kill(pid, givenData.signal);
  } else if (toSubstract < 0 && -toSubstract <= resource) {
    resource += toSubstract;
    kill(pid, givenData.signal);
  }
}