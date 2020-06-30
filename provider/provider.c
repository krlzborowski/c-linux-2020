#include "provider.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void readParameters(int argc, char *argv[]) {
  int opt;
  char *p;
  while ((opt = getopt(argc, argv, "s:h:")) != -1) {
    switch (opt) {
    case 's':

      givenData.signal = (int)strtol(optarg, &p, 10);

      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong signal format\n");
        exit(EXIT_FAILURE);
      }
      break;

    case 'h':
      givenData.incrementValue = strtof(optarg, &p);
      if (errno != 0 || *p != '/') {
        fprintf(stderr, "Wrong pace format\n");
        exit(EXIT_FAILURE);
      }

      p++;
      givenData.interval = strtof(p, &p);
      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong interval format\n");
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

  givenData.notKillingPercent = strtof(argv[optind], &p);

  if (errno != 0 || *p != '/') {
    fprintf(stderr, "Wrong RT tolerancy format\n");
    exit(EXIT_FAILURE);
  }

  p++;
  givenData.sendResponsePercent = strtof(p, &p);
  if (optarg == p || errno != 0 || *p != 0) {
    fprintf(stderr, "Wrong response percentage format\n");
    exit(EXIT_FAILURE);
  }
}

void createTimer() {
  struct sigevent signalEvent;
  memset(&signalEvent, 0, sizeof(struct sigevent));
  signalEvent.sigev_notify = SIGEV_THREAD;
  signalEvent.sigev_value.sival_ptr = (void *)&resource;
  signalEvent.sigev_notify_function = &onTimer;
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

void onTimer(union sigval sv) {

  int s;

  s = pthread_mutex_lock(&mtx);
  if (s != 0) {
    perror("Mutex lock failure");
    exit(EXIT_FAILURE);
  }

  *(float *)sv.sival_ptr += givenData.incrementValue;

  s = pthread_mutex_unlock(&mtx);
  if (s != 0) {
    perror("Mutex unlock failure");
    exit(EXIT_FAILURE);
  }

  s = pthread_cond_signal(&cond);
  if (s != 0) {
    perror("Thread condition failure");
    exit(EXIT_FAILURE);
  }
}

void processRequest(int pid, float toSubstract) {

  if (toSubstract > 0 && toSubstract <= resource) {
    resource -= toSubstract;

    if (kill(pid, givenData.signal) == -1) {
      perror("Signal kill failure");
      exit(EXIT_FAILURE);
    }
  } else if (toSubstract < 0 && -toSubstract <= resource) {
    resource += toSubstract;
    if (kill(pid, givenData.signal) == -1) {
      perror("Signal kill failure");
      exit(EXIT_FAILURE);
    }
  }
}

void setSignalHandlers() {
  struct sigaction signalAction, signalAction2;
  int *handling = calloc((SIGRT_NUM), sizeof(int));
  int *answering = calloc((SIGRT_NUM), sizeof(int));

  memset(&signalAction, '\0', sizeof(signalAction));
  memset(&signalAction2, '\0', sizeof(signalAction));
  signalAction.sa_handler = SIG_IGN;
  signalAction2.sa_sigaction = answer;
  signalAction2.sa_flags = SA_SIGINFO;

  int i = 0;
  int toHandle = (int)((float)(SIGRT_NUM)*givenData.notKillingPercent / 100.0);

  while (i < toHandle) {
    for (int j = SIGRTMIN; j < SIGRTMAX; j++) {
      if (i >= toHandle)
        break;
      else if (handling[j - SIGRTMIN] == 1)
        continue;
      else if (rand() % 2 == 0) {
        if (sigaction(j, &signalAction, NULL) == -1) {
          perror("Sigaction error:");
          exit(EXIT_FAILURE);
        }
        handling[j - SIGRTMIN] = 1;
        i++;
      }
    }
  }

  i = 0;
  toHandle = (int)((float)toHandle * givenData.sendResponsePercent / 100.0);

  while (i < toHandle) {
    for (int j = SIGRTMIN; j < SIGRTMAX; j++) {
      if (i >= toHandle)
        break;
      else if (handling[j - SIGRTMIN] == 0 || answering[j - SIGRTMIN] == 1)
        continue;
      else if (rand() % 2 == 0) {
        if (sigaction(j, &signalAction2, NULL) == -1) {
          perror("Sigaction error:");
          exit(EXIT_FAILURE);
        }
        answering[j - SIGRTMIN] = 1;
        i++;
      }
    }
  }
  free(handling);
  free(answering);
}

void answer(int signal, siginfo_t *info, void *ucontext) {
  kill(info->si_pid, signal);
}