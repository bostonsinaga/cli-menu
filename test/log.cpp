#include "cli-menu.hpp"
#include "reader.hpp"
#include "writer.hpp"

int main(int argc, char *argv[]) {

  cm::Program<cm::Boolean> *cli_log = cm::Program<cm::Boolean>::create(
    "log",
    cm::ProgramAbout(
      "Record the set of IDs, names, and values into a file or display them in the terminal",
      "Boston Sinaga",
      "https://github.com/bostonsinaga/cli-menu",
      cm::ProgramVersion(1, 0, 0)
    )
  );

  initRead(cli_log);
  initWrite(cli_log);
  initFilter(cli_log);

  users->run(argc, argv);
  return 0;
}
