#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Word *organism = new cm::Program(
    "organism",
    "Describe your lovely animals or plants",
    true
  );

  cm::Parameter *animals = new cm::Parameter(
    "animals",
    "Oxygen Consumers",
    true
  );

  cm::Parameter *plants = new cm::Parameter(
    "plants",
    "Oxygen Consumers",
    true
  );

  organism->run(argc, argv);
  organism->annihilate();
  return 0;
}

