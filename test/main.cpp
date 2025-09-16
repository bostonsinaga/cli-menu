#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Program<cm::Toggle> *users = cm::Program<cm::Toggle>::create(
    "users",
    cm::ProgramAbout(
      "Describe your loyalty users",
      "Boston Sinaga",
      "https://github.com/bostonsinaga/cli-menu",
      cm::ProgramVersion(1, 0, 0)
    ),
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Users"
      );
      return true;
    }
  );

  cm::Toggle *women = users->createToggle(
    "women",
    "Beutiful and smart",
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Women"
      );
      return true;
    }
  );

  cm::Toggle *men = users->createToggle(
    "men",
    "Born to fight",
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Men"
      );
      return true;
    }
  );

  // ultimate
  women->makeRequired();
  men->makeRequired();
  cm::Preset::applyFileIn(women, true);
  cm::Preset::applyFileOutFallback(women, true);
  cm::Preset::applyFileIn(men, true);
  cm::Preset::applyFileOutFallback(men, true);
  users->run(argc, argv);
  users->destroy();

  return 0;
}

