#ifndef __CLI_MENU__PRESET_CPP__
#define __CLI_MENU__PRESET_CPP__

#include <windows.h>
#include "preset.hpp"

namespace cli_menu {

  void Preset::completeFileInWildcards(Command *node) {

    std::string pattern;
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    std::string::size_type lastSlashIndex;
    std::string basePath;

    for (int i = 0; i < Result::numberOfWords(node); i++) {
      pattern = Result::getWordAt(node, i);

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
          Result::popWord(node);
          i--;

          do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
              Result::pushWord(node, basePath + findFileData.cFileName);
              i++;
            }
          } while (FindNextFileA(hFind, &findFileData) != 0);

          FindClose(hFind);
        }
      }
    }
  }

  void Preset::applyFileIn(
    Creator *owner,
    mt::CR_BOL isRequired,
    COMMAND_CALLBACK customCallback
  ) {
    Word *in = owner->createWord(
      Langu::agePreset::getKeyword(PRESET_IN),
      Langu::agePreset::getDescription(PRESET_IN),
      customCallback
    );

    in->registerAsInput();
    if (isRequired) in->makeRequired();
  }

  void Preset::applyFileOut(
    Creator *owner,
    mt::CR_BOL isRequired,
    COMMAND_CALLBACK customCallback
  ) {
    Word *out = owner->createWord(
      Langu::agePreset::getKeyword(PRESET_OUT),
      Langu::agePreset::getDescription(PRESET_OUT),
      customCallback
    );

    out->registerAsOutput();
    if (isRequired) out->makeRequired();
  }

  void Preset::applyFileIn(
    Creator *owner,
    mt::CR_BOL isRequired
  ) {
    applyFileIn(
      owner, isRequired,
      [](Command *current)->COMMAND_CALLBACK_CODE {
        bool found = false;
        std::string filename;
        completeFileInWildcards(current);

        // read multiple files
        for (int i = 0; i < Result::numberOfWords(current); i++) {
          filename = Result::getWordAt(current, i);

          if (mt_uti::Scanner::isFileExist(filename)) {
            found = true;

            // read file content
            Result::pushOutput(
              static_cast<Command*>(current->getParent()),
              mt_uti::Scanner::readFileString(filename)
            );
          }
          else Langu::ageMessage::printTemplateResponse(
            SENTENCE_KEYWORD_NOT_FOUND,
            filename
          );
        }

        if (found || !Result::numberOfWords(current)) {
          return COMMAND_CALLBACK_SUCCEEDED;
        }

        return COMMAND_CALLBACK_FAILED;
      }
    );
  }

  COMMAND_CALLBACK_CODE Preset::setFileOut(
    Command *node,
    std::string &filename
  ) {
    if (!filename.empty()) {

      std::string outputText = Result::concatOutputs(
        static_cast<Command*>(node->getParent())
      );

      if (!outputText.empty()) {
        BOOLEAN_INSTANT_QUESTION_CODE
          booleanInstantQuestionCode = BOOLEAN_INSTANT_QUESTION_NO;

        // existing file require verification to be overwritten
        if (mt_uti::Scanner::isFileExist(filename)) {

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
            } while (mt_uti::Scanner::isFileExist(numname));

            filename = numname;
          }
        }

        // canceled
        if (booleanInstantQuestionCode == BOOLEAN_INSTANT_QUESTION_CANCELED) {
          return COMMAND_CALLBACK_CANCELED;
        }
        // write string vector to text file (yes/no)
        else if (mt_uti::Printer::write(
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
        SENTENCE_EMPTY_OUTPUT,
        static_cast<Command*>(node->getParent())->getKeyword()
      );
    }

    return COMMAND_CALLBACK_FAILED;
  }

  void Preset::applyFileOutFallback(
    Creator *owner,
    mt::CR_BOL isRequired
  ) {
    applyFileOut(
      owner, isRequired,
      [](Command *current)->COMMAND_CALLBACK_CODE {
        std::string filename = Result::getLastWord(current);

        if (filename.empty()) {

          // set filename with file-in last argument
          current->iterate(
            mt_ds::GeneralTree::RIGHT,
            [&](mt_ds::LinkedList *node)->bool {

              if (static_cast<Command*>(node)->getKeyword()
                == Langu::agePreset::getKeyword(PRESET_IN)
              ) {
                filename = Result::getLastWord(static_cast<Command*>(node));
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

        COMMAND_CALLBACK_CODE callbackCode = setFileOut(current, filename);

        // file write failed message
        if (callbackCode == COMMAND_CALLBACK_FAILED) {
          Langu::ageMessage::printResponse(SENTENCE_FILE_WRITE_FAILURE);
        }

        return callbackCode;
      }
    );
  }

  void Preset::applyFileOutOptional(Creator *owner) {
    applyFileOut(
      owner, false,
      [](Command *current)->COMMAND_CALLBACK_CODE {
        std::string filename = Result::getLastWord(current);
        setFileOut(current, filename);
        return COMMAND_CALLBACK_SUCCEEDED;
      }
    );
  }

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
}

#endif // __CLI_MENU__PRESET_CPP__