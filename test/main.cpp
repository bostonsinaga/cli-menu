#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Toggle *organism = new cm::Toggle(
    "organism",
    "Describe your lovely animals or plants",
    true
  );

  cm::Toggle *animals = new cm::Toggle(
    "animals",
    "Oxygen Consumers",
    true
  );

  cm::Toggle *plants = new cm::Toggle(
    "plants",
    "Oxygen Consumers",
    true
  );

  organism->run(argc, argv);
  organism->annihilate();
  return 0;
}

