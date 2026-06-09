#include "cli-menu.hpp"
#include "read.hpp"
#include "write.hpp"
#include "filter.hpp"

int main(int argc, char *argv[]) {

  // main menu
  cm::Program<cm::Boolean> *cli_log = cm::Program<cm::Boolean>::create(
    "log",
    cm::ProgramAbout(
      "Log the set of IDs, names, and values",
      "Boston Sinaga",
      "https://github.com/bostonsinaga/cli-menu",
      cm::ProgramVersion(1, 0, 0)
    )
  );

  // partial initializations
  initRead(cli_log);
  initWrite(cli_log);
  initFilter(cli_log);

  cli_log->run(argc, argv);
  return 0;
}
