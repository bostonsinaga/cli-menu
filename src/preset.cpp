#ifndef __CLI_MENU__PRESET_CPP__
#define __CLI_MENU__PRESET_CPP__

#include "preset.hpp"

namespace cli_menu {

  void Preset::applyFileIn(
    Creator *owner,
    COMMAND_CALLBACK customCallback
  ) {
    Word *in = owner->createWord(
      Langu::agePreset::getKeyword(PRESET_IN),
      Langu::agePreset::getDescription(PRESET_IN),
      customCallback
    );

    in->makeRequired();
    owner->replaceExistingKeyword(in);
  }

  void Preset::applyFileOut(
    Creator *owner,
    COMMAND_CALLBACK customCallback
  ) {
    Word *out = owner->createWord(
      Langu::agePreset::getKeyword(PRESET_OUT),
      Langu::agePreset::getDescription(PRESET_OUT),
      customCallback
    );

    out->makeRequired();
    owner->replaceExistingKeyword(out);
  }

  void Preset::applyFileIn(Creator *owner) {
    applyFileIn(
      owner,
      [](Command *current)->bool {
        bool found = false;

        mt::VEC_STR filenames = Result::useWords(
          Langu::agePreset::getKeyword(PRESET_IN)
        );

        for (int i = 0; i < filenames.size(); i++) {
          if (mt_uti::Scanner::isFileExist(filenames[i])) {
            found = true;

            Result::useUltimates(current->getKeyword()).push_back(
              mt_uti::Scanner::readFileString(filenames[i])
            );
          }
          else std::cout << "'" << filenames[i] << "' IS NOT FOUND!\n";
        }

        return found;
      }
    );
  }

  void Preset::applyFileOut(Creator *owner) {
    applyFileOut(
      owner,
      [](Command *current)->bool {
        std::string filename = Result::getLastWord(
          Langu::agePreset::getKeyword(PRESET_OUT)
        );

        if (mt_uti::Scanner::isFileExist(filename) &&
          !Toggle::instantBooleanize()
        ) {
          int counter = 0;
          std::string numname;

          do {
            numname = filename + '[' + std::to_string(counter) + ']';
            counter++;
          } while (mt_uti::Scanner::isFileExist(numname));

          filename = numname;
        }

        mt_uti::Printer::write(
          Result::concatUltimates(current->getKeyword()),
          filename,
          false
        );

        return true;
      }
    );
  }

  void Preset::applyFile(Creator *owner) {
    Preset::applyFileIn(owner);
    Preset::applyFileOut(owner);
  }

  void Preset::applyHelp(Creator *owner) {
    Toggle *help = new Toggle(
      Langu::agePreset::getKeyword(PRESET_HELP),
      Langu::agePreset::getDescription(PRESET_HELP),
      [](Command *current)->bool {
        static_cast<Creator*>(current)->printHelp();
        return true;
      }
    );

    owner->replaceExistingKeyword(help);
    help->sterilize();
    help->makePseudo();
    help->makeRequired();
  }

  void Preset::applyList(Creator *owner) {
    Toggle *list = new Toggle(
      Langu::agePreset::getKeyword(PRESET_LIST),
      Langu::agePreset::getDescription(PRESET_LIST),
      [](Command *current)->bool {
        static_cast<Creator*>(current)->printList(false);
        return true;
      }
    );

    owner->replaceExistingKeyword(list);
    list->sterilize();
    list->makePseudo();
    list->makeRequired();
  }

  void Creator::setPresetHelpList() {
    Preset::applyHelp(this);
    Preset::applyList(this);
  }
}

#endif // __CLI_MENU__PRESET_CPP__