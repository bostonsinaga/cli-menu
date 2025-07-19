#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Toggle *organism = new cm::Toggle(
    "organism",
    "Describe your lovely animals or plants",
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Organism"
      );
      return true;
    }
  );

  organism->makeRequired();
  organism->makeStrict();

  cm::Toggle *animals = new cm::Toggle(
    "animals",
    "Oxygen consumers",
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Animals"
      );
      return true;
    }
  );

  animals->makeRequired();
  organism->addChild(animals);

  cm::Toggle *plants = new cm::Toggle(
    "plants",
    "Carbon dioxide consumers",
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Plants"
      );
      return true;
    }
  );

  plants->makeRequired();
  organism->addChild(plants);

  // ultimate
  organism->run(argc, argv);
  organism->destroy();

  return 0;
}

