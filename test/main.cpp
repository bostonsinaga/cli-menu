#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Program<cm::Toggle> *organism = cm::Program<cm::Toggle>::create(
    "organism",
    cm::ProgramAbout(
      "Describe your lovely animals or plants",
      "Boston Sinaga",
      "https://github.com/bostonsinaga/cli-menu",
      cm::ProgramVersion(1, 0, 0)
    ),
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Organism"
      );
      return true;
    }
  );

  cm::Toggle *animals = organism->createToggle(
    "animals",
    "Oxygen consumers",
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Animals"
      );
      return true;
    }
  );

  cm::Toggle *plants = organism->createToggle(
    "plants",
    "Carbon dioxide consumers",
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Plants"
      );
      return true;
    }
  );

  // ultimate
  animals->makeRequired();
  plants->makeRequired();
  cm::Preset::applyFileFull(animals);
  cm::Preset::applyFileFull(plants);
  organism->run(argc, argv);
  organism->destroy();

  return 0;
}

