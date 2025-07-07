#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.hpp"

namespace cli_menu {

  Command(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in = false
  ) {
    keyword = keyword_in;
    description = description_in;
    required = required_in;
  }

  Command::CODE Command::match(CR_VEC_STR raws) {
    Language::Code languageCode = Language::COMMAND_NOT_FOUND;

    for (mt::CR_STR str : raws) {
      if (hyphens + keyword == str) {
        languageCode = Language::COMMAND_FOUND;
      }
      else {
        children.iterate(
          mt_ds::GeneralTree::RIGHT,
          [&](mt_ds::LinkedList *node)->bool {

            if (static_cast<Command*>(node)->hyphens
              + static_cast<Command*>(node)->keyword == str
            ) {
              languageCode = Language::REQUIRED_ARGUMENT;
              return false;
            }

            return true;
          }
        );

        if (languageCode == Language::COMMAND_FOUND) {
          pushUnormap(str);
        }
        else if (languageCode == Language::REQUIRED_ARGUMENT) {
          break;
        }
      }
    }

    // ask 'raws' to the children
    if (languageCode == Language::REQUIRED_ARGUMENT) {
      children.iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {
          languageCode = static_cast<Command*>(node)->match(raws);
          return true;
        }
      );
    }

    if (languageCode == Language::COMMAND_FOUND) {
      return COMMAND_SUCCEED;
    }
    else if (dialogued) {
      return dialog();
    }
    return COMMAND_ERROR;
  }

  Command::CODE Command::dialog() {
    std::string input;

    while (Console::cinDialogInput(input)) {

      if (Control::backTest(input)) {

        if (parent) return parent->dialog();
        else Language::printResponse(Language::AT_ROOT);
      }
      else if (Control::quitTest(input)) {

        Language::printResponse(Language::PROGRAM_CANCELED);
        return COMMAND_CANCELED;
      }
      else if (Control::clipboardTest(input)) {
        copyPaste();
      }
      else if (Control::enterTest(input)) {
        size_t leafCount = 0;

        children->iterate(
          mt_ds::LinkedList::RIGHT,
          [&](mt_ds::LinkedList* node)->bool {

            if (!node->getChildren()) {
              leafCount++;
            }
          }
        );

        if (numberOfChildren() == leafCount) {
          if (false) {

          }
          else callback(result);
        }
        else {}
      }
      else if (Control::helpTest(input)) {
        printHelp();
      }
      else if (Control::listTest(input)) {
        printList();
      }
      else if (Control::modifyTest(input)) {

      }
      else if (Control::nextTest(input)) {

        if (next()) return next()->dialog();
        else Language::printResponse();
      }
      else if (Control::previousTest(input)) {
        return previous()->dialog();
      }
      else if (Control::resetTest(input)) {

      }
      else if (Control::selectTest(input)) {

      }
      else if (Control::viewTest(input)) {

      }
      else {}
    }

    return ERROR;
  }

  void Command::run(
    mt::CR_INT argc,
    char *argv[]
  ) {
    // register signal handler for Ctrl+C (SIGINT)
    std::signal(SIGINT, ctrlCSignalHandler);

    CODE commandCode = match(
      mt_uti::StrTools::argvToStringVector(argc, argv)
    );
  }
}

#endif // __CLI_MENU__COMMAND_CPP__