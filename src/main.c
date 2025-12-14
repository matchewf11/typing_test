#include <stdio.h>

#include "cli/cli.h"

int main(int argc, char *argv[]) {

  CliCmd cmd;
  ParseResult ps = parse_args(argc, argv, &cmd);

  switch (cmd) {
  case CMD_RUN:
    printf("RUNNING\n");
    break;
  case CMD_HELP:
    printf("HELPING\n");
    break;
  case CMD_STATS:
    printf("STATING\n");
    break;
  case CMD_ERROR:
    printf("ERRORING\n");
    break;
  }

  return 0;
}
