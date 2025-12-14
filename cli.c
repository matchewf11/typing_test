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
  if (strcmp(args[1], "help")) {
    return (CliOptions){
        .cli_cmd = HELP,
        .err_msg = NULL,
    };
  } else if (strcmp(args[1], "run")) {
    return (CliOptions){
        .cli_cmd = RUN,
        .err_msg = NULL,
    };
  } else if (strcmp(args[1], "stats")) {
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

// typedef enum {
//   EXECUTABLE,
// } CliTokens;

// typedef enum { HELP, RUN, STATS } CliCmds;
//
// typedef struct {
//   CliCmds cmd;
// } CliOptions;

// typedef struct {
//
// } CliHelp;

// if (len == 1) { // if there are no argumnets
//   return (CliOptions){.cmd = HELP};
// }

// check second arg
// if stats then handle that
// if run then hanlde that
// if it is neither than return an error

// [ ] `./a.out` will start a help menu for all cmds
// this should require one arg and throw (except if -h is present)
// -h basically overwrites any other cmds (help instead of error)
//
// [ ] `./a.out stats` will print stats
// [ ] `./a.out stats --help` will start help menu for start
// [ ] `./a.out --help stats ` will start help menu for start
// [ ] `./a.out stats -h` will start a help menu for start
// [ ] `./a.out -h stats` will start a help menu for start
// [ ] `./a.out run` will start the test
// [ ] `./a.out run --help` will start help menu for the test
// [ ] `./a.out run -h` will start a help menu for the test
// [ ] `./a.out run --help` will start help menu for the test
// [ ] `./a.out run -h` will start a help menu for the test
// [ ] `./a.out help` will staart help menu
// [ ] `./a.out help -h` will staart help menu
// [ ] `./a.out help --help` will staart help menu
// [ ] `./a.out -h help` will staart help menu
// [ ] `./a.out --help help` will staart help menu
//
// make it so flags can be combined -hf == -h -f
