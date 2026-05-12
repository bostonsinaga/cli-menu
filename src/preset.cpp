#ifndef __CLI_MENU__PRESET_CPP__
#define __CLI_MENU__PRESET_CPP__

#include <windows.h>
#include "preset.hpp"

namespace cli_menu {

  void Preset::applyHelp(Creator *owner) {
    Boolean *help = new Boolean(
      Langu::agePreset::getKeyword(PRESET_HELP),
      Langu::agePreset::getDescription(PRESET_HELP),
      [](Command *current)->COMMAND_CALLBACK_CODE {
        static_cast<Creator*>(current->getParent())->printHelp();
        return COMMAND_CALLBACK_SUCCEEDED;
      }
    );

    owner->replaceExistingKeyword(help);
    help->makePseudo();
    help->makeSterilized();
  }

  void Preset::applyList(Creator *owner) {
    Boolean *list = new Boolean(
      Langu::agePreset::getKeyword(PRESET_LIST),
      Langu::agePreset::getDescription(PRESET_LIST),
      [](Command *current)->COMMAND_CALLBACK_CODE {
        static_cast<Creator*>(current->getParent())->printList(CONSOLE_HINT_2, 0, true);
        return COMMAND_CALLBACK_SUCCEEDED;
      }
    );

    owner->replaceExistingKeyword(list);
    list->makePseudo();
    list->makeSterilized();
  }

  void Creator::setPresetHelpList() {
    Preset::applyHelp(this);
    Preset::applyList(this);
  }

  /** FILE OPERATIONS */

  void Preset::File::completePathWildcards(Command *node) {

    std::string pattern;
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    std::string::size_type lastSlashIndex;
    std::string basePath;

    for (int i = 0; i < Data::Input::numberOfWords(node); i++) {
      pattern = Data::Input::getWordAt(node, i);

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
          Data::Input::popWord(node);
          i--;

          do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
              Data::Input::pushWord(node, basePath + findFileData.cFileName);
              i++;
            }
          } while (FindNextFileA(hFind, &findFileData) != 0);

          FindClose(hFind);
        }
      }
    }
  }

  void Preset::File::applyCustomIn(
    Creator *owner,
    mt::CR_BOL isRequired,
    COMMAND_CALLBACK callback
  ) {
    Word *in = owner->createWord(
      Langu::agePreset::getKeyword(PRESET_IN),
      Langu::agePreset::getDescription(PRESET_IN),
      callback
    );

    in->registerAsInput();
    if (isRequired) in->makeRequired();
  }

  void Preset::File::applyTextIn(
    Creator *owner,
    mt::CR_BOL isRequired
  ) {
    applyCustomIn(
      owner, isRequired,
      [](Command *current)->COMMAND_CALLBACK_CODE {
        bool found = false;
        std::string filename;
        completePathWildcards(current);

        // read multiple files
        for (int i = 0; i < Data::Input::numberOfWords(current); i++) {
          filename = Data::Input::getWordAt(current, i);

          if (mt::FS::is_regular_file(filename)) {
            found = true;

            // read file content
            Data::Output::push(
              static_cast<Command*>(current->getParent()),
              mt_uti::Scanner::readFileString(filename)
            );
          }
          else Langu::ageMessage::printTemplateResponse(
            SENTENCE_KEYWORD_NOT_FOUND,
            filename
          );
        }

        if (found || !Data::Input::numberOfWords(current)) {
          return COMMAND_CALLBACK_SUCCEEDED;
        }

        return COMMAND_CALLBACK_FAILED;
      }
    );
  }

  COMMAND_CALLBACK_CODE Preset::File::useTextOut(
    Command *node,
    std::string &filename
  ) {
    if (!filename.empty()) {

      std::string outputText = Data::Output::concat(
        static_cast<Command*>(node->getParent())
      );

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
            SENTENCE_FILE_WRITE_SUCCEED,
            filename
          );

          return COMMAND_CALLBACK_SUCCEEDED;
        }
      }
      else Langu::ageMessage::printTemplateResponse(
        SENTENCE_EMPTY_WRITE_OUTPUT_THIS,
        static_cast<Command*>(node->getParent())->getKeyword()
      );
    }

    return COMMAND_CALLBACK_FAILED;
  }

  void Preset::File::applyCustomOut(
    Creator *owner,
    mt::CR_BOL isRequired,
    COMMAND_CALLBACK callback
  ) {
    Word *out = owner->createWord(
      Langu::agePreset::getKeyword(PRESET_OUT),
      Langu::agePreset::getDescription(PRESET_OUT),
      callback
    );

    out->registerAsOutput();
    if (isRequired) out->makeRequired();
  }

  void Preset::File::applyTextOutFallback(
    Creator *owner,
    mt::CR_BOL isRequired
  ) {
    applyCustomOut(
      owner, isRequired,
      [](Command *current)->COMMAND_CALLBACK_CODE {
        std::string filename = Data::Input::getLastWord(current);

        if (filename.empty()) {

          // set filename with file-in last argument
          current->forEach(
            mt_ds::LinkedList::RIGHT,
            [&](mt_ds::LinkedList *node)->bool {

              if (static_cast<Command*>(node)->getKeyword()
                == Langu::agePreset::getKeyword(PRESET_IN)
              ) {
                filename = Data::Input::getLastWord(static_cast<Command*>(node));
                return false;
              }

              return true;
            }
          );

          // set filename with program keyword
          if (filename.empty()) {
            filename = static_cast<Command*>(current->getRoot())->getKeyword()
              + Langu::agePreset::fileOutDefaultExtension;
          }
        }

        COMMAND_CALLBACK_CODE callbackCode = useTextOut(current, filename);

        // file write failed message
        if (callbackCode == COMMAND_CALLBACK_FAILED) {
          Langu::ageMessage::printResponse(SENTENCE_FILE_WRITE_FAILURE);
        }

        return callbackCode;
      }
    );
  }

  void Preset::File::applyTextOutOptional(Creator *owner) {
    applyCustomOut(
      owner, false,
      [](Command *current)->COMMAND_CALLBACK_CODE {
        std::string filename = Data::Input::getLastWord(current);
        useTextOut(current, filename);
        return COMMAND_CALLBACK_SUCCEEDED;
      }
    );
  }
}

#endif // __CLI_MENU__PRESET_CPP__