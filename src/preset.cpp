#ifndef __CLI_MENU__PRESET_CPP__
#define __CLI_MENU__PRESET_CPP__

#include "preset.hpp"

namespace cli_menu {

  void Preset::applyFileIn(
    Creator *owner,
    const COMMAND_CALLBACK &customCallback
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
    const COMMAND_CALLBACK &customCallback
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
      [&]()->bool {
        bool found = false;

        mt::VEC_STR filenames = Result::useWords(
          Langu::agePreset::getKeyword(PRESET_IN)
        );

        for (int i = 0; i < filenames.size(); i++) {
          if (mt_uti::Scanner::isFileExist(filenames[i])) {
            found = true;

            Result::useUltimates(owner->keyword).push_back(
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
      [&]()->bool {
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
          Result::concatUltimates(owner->keyword),
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
    Toggle *in = new Toggle(
      Langu::agePreset::getKeyword(PRESET_HELP),
      Langu::agePreset::getDescription(PRESET_HELP),
      [&]()->bool {
        owner->printHelp();
        return true;
      }
    );

    in->sterilize();
    in->makeRequired();
    owner->replaceExistingKeyword(in);
  }

  void Preset::applyList(Creator *owner) {
    Toggle *out = new Toggle(
      Langu::agePreset::getKeyword(PRESET_LIST),
      Langu::agePreset::getDescription(PRESET_LIST),
      [&]()->bool {
        owner->printList(false);
        return true;
      }
    );

    out->sterilize();
    out->makeRequired();
    owner->replaceExistingKeyword(out);
  }

  void Creator::setPresetHelpList() {
    Preset::applyHelp(this);
    Preset::applyList(this);
  }
}

#endif // __CLI_MENU__PRESET_CPP__