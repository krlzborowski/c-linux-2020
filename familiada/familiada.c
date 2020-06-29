#include "familiada.h"

int readParameters(int argc, char **argv) {
  int processesCount = 0;
  int opt;
  int signal = SIGHUP;
  givenData.signal = "1";
  errno = 0;
  while ((opt = getopt(argc, argv, "s:h:r:")) != -1) {
    char *p;
    switch (opt) {
    case 's':
      signal = (int)strtol(optarg, &p, 10);

      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong signal format\n");
        exit(errno);
      }

      if (signal == SIGPIPE) {
        fprintf(stderr, "Can't use SIGPIPE\n");
        exit(EXIT_FAILURE);
      }

      givenData.signal = strdup(optarg);

      break;
    case 'h': {
      strtof(optarg, &p);

      if (errno != 0 || *p != '/') {
        fprintf(stderr, "Wrong pace format\n");
        exit(EXIT_FAILURE);
      }
      p++;
      strtof(p, &p);
      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong pace format\n");
        exit(EXIT_FAILURE);
      }
      givenData.pace = strdup(optarg);
      break;
    }
    case 'r': {
      strtof(optarg, &p);
      if (errno != 0 || *p != '/') {
        fprintf(stderr, "Wrong endurance format\n");
        exit(EXIT_FAILURE);
      }
      p++;
      strtof(p, &p);
      if (optarg == p || errno != 0 || *p != 0) {
        fprintf(stderr, "Wrong edurance format\n");
        exit(EXIT_FAILURE);
      }
      givenData.endurance = strdup(optarg);
      break;
    }

    default:
      fprintf(stderr,
              "Usage: %s -s <signal> -h <float>/<float> -r <float>/<float> "
              "<float>:<float> ...\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  for (int i = optind; i < argc; i++) {

    givenData.positionals[processesCount] = strdup(argv[i]);
    processesCount++;
  }

  return processesCount;
}

void produceProcesses(int processesCount, int *pipeFd) {
  pid_t childPid;
  pid_t pid = getpid();

  switch (childPid = fork()) {
  case -1:
    fprintf(stderr, "Fork failure\n");
    exit(EXIT_FAILURE);
  case 0:
    produceParasites(processesCount, pid, pipeFd);
    exit(EXIT_SUCCESS);
  default: {
    char *args[] = {"./provider", "-s",           givenData.signal,
                    "-h",         givenData.pace, givenData.endurance,
                    NULL};

    if (dup2(pipeFd[0], STDIN_FILENO) == -1) {
      perror("Dup2 failure");
      exit(EXIT_FAILURE);
    }
    if (execv("./provider", args) == -1) {
      perror("Execv failure");
      exit(EXIT_FAILURE);
    }
  }
  }
}

void produceParasites(int processesCount, pid_t pid, int *pipeFd) {
  pid_t childPid;
  for (size_t i = 0; i < processesCount; i++) {
    switch (childPid = fork()) {
    case -1:
      fprintf(stderr, "Fork failure\n");
      exit(EXIT_FAILURE);
    case 0: {
      char *toSeparate = strdup(givenData.positionals[i]);
      char *interval = strsep(&toSeparate, ":");
      char *initialValue = strsep(&toSeparate, ":");
      char *args[] = {"./parasite", strcat("-s", givenData.signal),
                      "-p",         strcat("-d", interval),
                      "-v",         initialValue,
                      NULL};

      if (dup2(pipeFd[1], STDOUT_FILENO) == -1) {
        perror("Dup2 failure");
        exit(EXIT_FAILURE);
      }

      if (execv("./parasite", args) == -1) {
        fprintf(stderr, "Execv failure\n");
        exit(EXIT_FAILURE);
      }
    }
    default:
      break;
    }
  }
}

int *setPipe() {
  static int pipeFd[2];

  if (pipe(pipeFd) == -1) {
    perror("Pipe failure");
    exit(EXIT_FAILURE);
  }
  setFlag(pipeFd[0]);
  setFlag(pipeFd[1]);
  return pipeFd;
}

void setFlag(int fd) {
  int flags = fcntl(fd, F_GETFD, 0);
  if (flags == -1) {
    perror("Fcntl failure");
    exit(EXIT_FAILURE);
  }
  flags |= FD_CLOEXEC;
  if (fcntl(fd, F_SETFD, flags) == -1) {
    perror("Fcntl failure");
    exit(EXIT_FAILURE);
  }
}