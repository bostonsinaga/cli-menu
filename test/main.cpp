#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Program<cm::Boolean> *users = cm::Program<cm::Boolean>::create(
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

  cm::Boolean *men = users->createBoolean(
    "men",
    "Born to fight",
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Men"
      );
      return true;
    }
  );

  cm::Boolean *women = users->createBoolean(
    "women",
    "Beutiful and smart",
    [](cm::Command *current)->bool {
      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Women"
      );
      return true;
    }
  );

  // connect and run
  men->makeRequired();
  women->makeRequired();
  cm::Preset::applyFileIn(men, true);
  cm::Preset::applyFileOutFallback(men, true);
  cm::Preset::applyFileIn(women, true);
  cm::Preset::applyFileOutFallback(women, true);
  users->run(argc, argv);
  users->destroy();

  return 0;
}

