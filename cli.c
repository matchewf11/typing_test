#include <string.h>

#include "cli.h"

CliOptions parse_cli(char **args, int len) {
  if (len == 1) {
    return (CliOptions){
        .cli_cmd = ERROR,
        .err_msg = "too little arguments. Options: 'help', 'run', 'stats'",
    };
  } else if (len > 2) {
    return (CliOptions){
        .cli_cmd = ERROR,
        .err_msg = "too many arguments, currently only support one arg",
    };
  }
  if (strcmp(args[1], "help") == 0) {
    return (CliOptions){
        .cli_cmd = HELP,
        .err_msg = NULL,
    };
  } else if (strcmp(args[1], "run") == 0) {
    return (CliOptions){
        .cli_cmd = RUN,
        .err_msg = NULL,
    };
  } else if (strcmp(args[1], "stats") == 0) {
    return (CliOptions){
        .cli_cmd = STATS,
        .err_msg = NULL,
    };
  } else {
    return (CliOptions){
        .cli_cmd = ERROR,
        .err_msg = "could not recognize args. Options: 'help', 'run', 'stats'",
    };
  }
}
