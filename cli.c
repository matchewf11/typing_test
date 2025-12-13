
// typedef enum {
//   EXECUTABLE,
// } CliTokens;

void parse_cli(char **args, int len) {}

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
