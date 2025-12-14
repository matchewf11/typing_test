#ifndef CLI_H
#define CLI_H

typedef enum { ERROR, HELP, RUN, STATS } CliCmd;

typedef struct {
  CliCmd cli_cmd;
  char *err_msg;
} CliOptions;

// no need to free err_msg
CliOptions parse_cli(char **args, int len);

#endif
