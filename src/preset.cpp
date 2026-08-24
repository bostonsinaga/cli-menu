#ifndef __CLI_MENU__PRESET_CPP__
#define __CLI_MENU__PRESET_CPP__

#include <windows.h>
#include "preset.hpp"

namespace cli_menu {

  void Preset::hide(Command *command) {
    if (command) {
      command->makePseudo();
      command->makeSterilized();
      Data::unregisterWords(command);
      Data::unregisterTexts(command);
    }
  }

  void Preset::applyHelp(Parameter *owner) {
    if (!owner) return;

    Boolean *help = owner->addBoolean(
      Langu::agePreset::getKeyword(PRESET_KEYWORD_HELP),
      Langu::agePreset::getDescription(PRESET_KEYWORD_HELP),
      [](Command *self)->COMMAND_CALLBACK_CODE {
        static_cast<Command*>(self->getParent())->printHelp();
        return COMMAND_CALLBACK_DONE;
      }, false
    );

    hide(help);
  }

  void Preset::applyList(Parameter *owner) {
    if (!owner) return;

    Boolean *list = owner->addBoolean(
      Langu::agePreset::getKeyword(PRESET_KEYWORD_LIST),
      Langu::agePreset::getDescription(PRESET_KEYWORD_LIST),
      [](Command *self)->COMMAND_CALLBACK_CODE {

        static_cast<Command*>(self->getParent())->printList(
          CONLOR_LIST, Console::IndentSticked(), true
        );

        return COMMAND_CALLBACK_DONE;
      }, false
    );

    hide(list);
  }

  void Parameter::setPresetHelpList() {
    Preset::applyHelp(this);
    Preset::applyList(this);
  }

  /** FILE OPERATIONS */

  void Preset::File::completePathWildcards(Command *command) {

    std::string pattern;
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    std::string::size_type lastSlashIndex;
    std::string basePath;

    for (int i = 0; i < Data::getWordsSize(command); i++) {
      pattern = Data::xgetWord(command, i);

      if (pattern.find('*') != std::string::npos ||
        pattern.find('?') != std::string::npos
      ) {
        hFind = FindFirstFileA(pattern.c_str(), &findFileData);

        // keep the pattern path
        lastSlashIndex = pattern.find_last_of("\\/");
        basePath = (lastSlashIndex == std::string::npos) ?
          "" : pattern.substr(0, lastSlashIndex + 1);

        // expand wildcard pattern to path with filename
        if (hFind != INVALID_HANDLE_VALUE) {
          Data::xpopWord(command);
          i--;

          do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
              Data::xpushWord(command, basePath + findFileData.cFileName);
              i++;
            }
          } while (FindNextFileA(hFind, &findFileData) != 0);

          FindClose(hFind);
        }
      }
    }
  }

  void Preset::File::applyCustomIn(
    Parameter *owner,
    mt::CR_BOL isRequired,
    mt::CR<CODE_CALLBACK> callback
  ) {
    if (!owner) return;

    Word *in = owner->addWord(
      Langu::agePreset::getKeyword(PRESET_KEYWORD_IN),
      Langu::agePreset::getDescription(PRESET_KEYWORD_IN),
      callback
    );

    if (in) {
      in->registerAsInput();
      if (isRequired) in->makeRequired();
    }
  }

  void Preset::File::applyTextIn(
    Word *owner,
    mt::CR_BOL isRequired
  ) {
    applyCustomIn(
      owner, isRequired,
      [](Command *self)->COMMAND_CALLBACK_CODE {
        bool found = false;
        std::string filename;

        // complete paths inside the input
        completePathWildcards(self);

        // read multiple files
        for (int i = 0; i < Data::getWordsSize(self); i++) {
          filename = Data::xgetWord(self, i);

          if (mt::FS::is_regular_file(filename)) {
            found = true;

            // read file content
            Data::xpushWord(
              static_cast<Command*>(self->getParent()),
              mt_uti::Scanner::readFileString(filename)
            );

            // ..and put it into parent 'Data'
            Langu::ageMessage::printResponse(
              SENTENCE_FILE_READ_STORED_TO_PARENT_INPUT
            );
          }
          else Langu::ageMessage::printTemplateResponse(
            SENTENCE_FILE_READ_NOT_FOUND, {filename}
          );
        }

        if (found || Data::isWordsEmpty(self)) {
          return COMMAND_CALLBACK_DONE;
        }

        return COMMAND_CALLBACK_ERROR;
      }
    );
  }

  COMMAND_CALLBACK_CODE Preset::File::useTextOut(
    Command *owner,
    std::string filename
  ) {
    if (!filename.empty()) {
      std::string outputText = Data::stringifyTexts(owner);

      if (!outputText.empty()) {
        BOOLEAN_INSTANT_QUESTION_CODE
          booleanInstantQuestionCode = BOOLEAN_INSTANT_QUESTION_NO;

        // existing file require verification to be overwritten
        if (mt::FS::is_regular_file(filename)) {

          booleanInstantQuestionCode = Boolean::instantQuestion(
            SENTENCE_FILE_OVERWRITE_QUESTION,
            filename
          );

          if (booleanInstantQuestionCode == BOOLEAN_INSTANT_QUESTION_NO) {
            int counter = 0;
            std::string prefix = filename, suffix, numname;

            // separate the name and extension from the filename
            for (int i = filename.length() - 1; i >= 0; i--) {
              if (filename[i] == '.') {
                prefix = filename.substr(0, i);
                suffix = filename.substr(i);
                break;
              }
            }

            // trying to make a unique name with numbers in brackets
            do {
              numname = prefix + Langu::agePreset::fileOutBracketsForNumbering.first
                + std::to_string(counter) + Langu::agePreset::fileOutBracketsForNumbering.second + suffix;
              counter++;
            } while (mt::FS::is_regular_file(numname));

            filename = numname;
          }
        }

        // canceled
        if (booleanInstantQuestionCode == BOOLEAN_INSTANT_QUESTION_CANCELED) {
          return COMMAND_CALLBACK_CANCELED;
        }
        // write string vector to text file (yes/no)
        else if (mt_uti::Printer::writeFileString(
          outputText, filename, false
        )) {
          Langu::ageMessage::printTemplateResponse(
            SENTENCE_FILE_WRITE_SUCCEED, {filename}
          );

          return COMMAND_CALLBACK_DONE;
        }
      }
      else Langu::ageMessage::printTemplateResponse(
        SENTENCE_EMPTY_OUTPUT_NAMED, {owner->getKeyword()}
      );
    }

    return COMMAND_CALLBACK_ERROR;
  }

  void Preset::File::applyCustomOut(
    Parameter *owner,
    mt::CR_BOL isRequired,
    mt::CR<CODE_CALLBACK> callback
  ) {
    if (!owner) return;

    Word *out = owner->addWord(
      Langu::agePreset::getKeyword(PRESET_KEYWORD_OUT),
      Langu::agePreset::getDescription(PRESET_KEYWORD_OUT),
      callback
    );

    if (out) {
      out->registerAsOutput();
      if (isRequired) out->makeRequired();
    }
  }

  void Preset::File::applyTextOutFallback(
    Parameter *owner,
    mt::CR_BOL isRequired
  ) {
    applyCustomOut(
      owner, isRequired,
      [](Command *self)->COMMAND_CALLBACK_CODE {
        std::string filename = Data::getWord(self);

        if (filename.empty()) {

          // set filename with file-in last argument
          self->forEach(
            [&](mt_ds::LinkedList *current)->bool {

              if (static_cast<Command*>(current)->getKeyword()
                == Langu::agePreset::getKeyword(PRESET_KEYWORD_IN)
              ) {
                filename = Data::getWord(static_cast<Command*>(current));
                return false;
              }

              return true;
            }
          );

          // set filename with program keyword
          if (filename.empty()) {
            filename = static_cast<Command*>(self->getRoot())->getKeyword()
              + Langu::agePreset::fileOutDefaultExtension;
          }
        }

        // write to filename
        COMMAND_CALLBACK_CODE callbackCode = useTextOut(
          static_cast<Command*>(self->getParent()),
          filename
        );

        // file write failed message
        if (callbackCode == COMMAND_CALLBACK_ERROR) {
          Langu::ageMessage::printTemplateResponse(
            SENTENCE_FILE_WRITE_FAILURE, {filename}
          );
        }

        return callbackCode;
      }
    );
  }

  void Preset::File::applyTextOutOptional(Parameter *owner) {
    applyCustomOut(
      owner, false,
      [](Command *self)->COMMAND_CALLBACK_CODE {

        // write to filename of this recent input
        useTextOut(
          static_cast<Command*>(self->getParent()),
          Data::getWord(self)
        );

        return COMMAND_CALLBACK_DONE;
      }
    );
  }

  Color Preset::ColorSet::highlights[COLOR_TOTAL] = {
    Color::set[WHITE], Color::set[WHITE], Color::set[WHITE], Color::set[WHITE], Color::set[GRAY],
    Color::set[GRAY], Color::set[WHITE], Color::set[WHITE], Color::set[GRAY], Color::set[WHITE],
    Color::set[WHITE], Color::set[WHITE], Color::set[WHITE], Color::set[GRAY], Color::set[GRAY],
    Color::set[GRAY], Color::set[WHITE], Color::set[WHITE], Color::set[WHITE], Color::set[WHITE],
    Color::set[WHITE], Color::set[WHITE], Color::set[GRAY], Color::set[WHITE], Color::set[WHITE],
    Color::set[WHITE], Color::set[WHITE], Color::set[GRAY], Color::set[GRAY], Color::set[WHITE],
    Color::set[GRAY], Color::set[WHITE], Color::set[GRAY], Color::set[GRAY], Color::set[GRAY]
  };

  mt::PAIR2<std::string, size_t> Preset::ColorSet::stringify(
    const COLOR_CODE &code,
    mt::CR<Color> foreground,
    mt::CR<Color> background
  ) {
    std::string str = getSpacing() + Langu::ageColorSet::getName(code) + getSpacing();
    str += std::to_string(foreground.getR()) + getSpacing();
    str += std::to_string(foreground.getG()) + getSpacing();
    str += std::to_string(foreground.getB()) + getSpacing();

    return {
      Color::getString(str, foreground, background),
      str.length()
    };
  }

  void Preset::ColorSet::print() {
    mt::PAIR2<std::string, size_t> rgbnames[COLOR_TOTAL];
    size_t longest = 0;

    // find longest name with its RGB value
    for (int i = 0; i < COLOR_TOTAL; i++) {
      rgbnames[i] = stringify(static_cast<COLOR_CODE>(i), Color::set[i], highlights[i]);
      if (longest < rgbnames[i].second) longest = rgbnames[i].second;
    }

    // print title
    std::string title = getSpacing() + Langu::ageColorSet::getTitle() + ':' + getSpacing();

    Console::logUnderlineString(
      title, Color::set[WHITE], Color::set[GRAY]
    );

    Console::logUnderlineString(
      std::string(longest - title.length(), ' ') + '\n',
      Color::set[WHITE], Color::set[GRAY]
    );

    // print colored spaces with vertical alignment
    for (int i = 0; i < COLOR_TOTAL; i++) {
      std::cout << rgbnames[i].first;

      Console::logString(
        std::string(longest - rgbnames[i].second, ' '),
        Color::set[i], highlights[i]
      );

      std::cout << std::endl;
    }
  }

  void Preset::ColorSet::apply(Parameter *owner) {
    if (!owner) return;

    Boolean *colorSet = owner->addBoolean(
      Langu::agePreset::getKeyword(PRESET_KEYWORD_COLOR_SET),
      Langu::agePreset::getDescription(PRESET_KEYWORD_COLOR_SET),
      [](Command *self)->COMMAND_CALLBACK_CODE {
        ColorSet::print();
        return COMMAND_CALLBACK_DONE;
      }, false
    );

    hide(colorSet);
  }
}

#endif // __CLI_MENU__PRESET_CPP__