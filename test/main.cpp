#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Toggle *organism = new cm::Toggle(
    "organism",
    "Describe your lovely animals or plants",
    true,
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HIGHLIGHT, "Hello Organism"
      );
      return true;
    }
  );

  cm::Toggle *animals = new cm::Toggle(
    "animals",
    "Oxygen consumers",
    true,
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HIGHLIGHT, "Hello Animals"
      );
      return true;
    }
  );

  cm::Toggle *plants = new cm::Toggle(
    "plants",
    "Carbon dioxide consumers",
    true,
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HIGHLIGHT, "Hello Plants"
      );
      return true;
    }
  );

  organism->addChild(animals);
  organism->addChild(plants);
  organism->run(argc, argv);

  return 0;
}

