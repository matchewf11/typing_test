#ifndef CLI_H
#define CLI_H

typedef enum { CMD_HELP, CMD_RUN, CMD_STATS, CMD_ERROR } CliCmd;
typedef enum {
  PARSE_OK,
  PARSE_ERR_NOT_ENOUGH_ARGS,
  PARSE_ERR_TOO_MANY_ARGS,
  PARSE_ERR_INVALID_CMD,
} ParseResult;

ParseResult parse_args(int argc, char **argv, CliCmd *out);

#endif
