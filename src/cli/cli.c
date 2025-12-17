#include <string.h>

#include "cli.h"

ParseResult parse_args(int argc, const char *const *argv, CliCmd *out) {
  if (argc == 1) {
    *out = CMD_ERROR;
    return PARSE_ERR_NOT_ENOUGH_ARGS;
  } else if (argc > 2) {
    *out = CMD_ERROR;
    return PARSE_ERR_TOO_MANY_ARGS;
  }
  if (strcmp(argv[1], "help") == 0) {
    *out = CMD_HELP;
    return PARSE_OK;
  } else if (strcmp(argv[1], "run") == 0) {
    *out = CMD_RUN;
    return PARSE_OK;

  } else if (strcmp(argv[1], "stats") == 0) {
    *out = CMD_STATS;
    return PARSE_OK;
  }
  *out = CMD_ERROR;
  return PARSE_ERR_INVALID_CMD;
}
