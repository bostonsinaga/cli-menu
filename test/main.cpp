#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Program<cm::Boolean> *users = cm::Program<cm::Boolean>::create(
    "users",
    cm::ProgramAbout(
      "Describe your loyalty users.",
      "Boston Sinaga",
      "https://github.com/bostonsinaga/cli-menu",
      cm::ProgramVersion(1, 0, 0)
    ),
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {

      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Users"
      );

      return cm::COMMAND_CALLBACK_SUCCEEDED;
    }
  );

  cm::Boolean *men = users->createBoolean(
    "men",
    "Born to fight",
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {

      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Men"
      );

      return cm::COMMAND_CALLBACK_SUCCEEDED;
    }
  );

  men->makeRequired();
  cm::PresetFile::applyTextIn(men, true);
  cm::PresetFile::applyTextOutFallback(men, true);

  cm::Boolean *women = users->createBoolean(
    "women",
    "Beutiful and smart",
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {

      cm::Console::logResponse(
        cm::CONSOLE_HINT_1, "Hello Women"
      );

      return cm::COMMAND_CALLBACK_SUCCEEDED;
    }
  );

  women->makeRequired();
  cm::PresetFile::applyTextIn(women, true);
  cm::PresetFile::applyTextOutFallback(women, true);

  // proceed
  users->run(argc, argv);
  users->destroy();

  return 0;
}
