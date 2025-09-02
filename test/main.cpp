#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Program<cm::Toggle> *organism = cm::Program<cm::Toggle>::create(
    "organism",
    "Describe your lovely animals or plants",
    cm::Version(1, 0, 0),
    "Boston Sinaga",
    "https://github.com/bostonsinaga/cli-menu",
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Organism"
      );
      return true;
    }
  );

  cm::Toggle *animals = organism->createToggle(
    "animals",
    "Oxygen consumers",
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Animals"
      );
      return true;
    }
  );

  cm::Toggle *plants = organism->createToggle(
    "plants",
    "Carbon dioxide consumers",
    []()->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Plants"
      );
      return true;
    }
  );

  // ultimate
  organism->makeStrict();
  animals->makeRequired();
  plants->makeRequired();
  cm::Preset::applyFile(animals);
  cm::Preset::applyFile(plants);
  organism->run(argc, argv);
  organism->destroy();

  return 0;
}

