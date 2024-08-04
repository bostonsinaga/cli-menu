#include <iostream>
#include "cli-menu.h"

int main(int argc, char *argv[]) {

  cli_menu::Message::print(
    cli_menu::Message::ERROR,
    "please don't waste a time!",
    argv[0]
  );

  return 0;
}

