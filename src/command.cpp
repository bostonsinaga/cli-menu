#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include <csignal>
#include "command.hpp"

namespace cli_menu {

  Command::Command(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in
  ) {
    keyword = keyword_in;
    description = description_in;
    required = required_in;
  }

  COMMAND_CODE Command::match(mt::CR_VEC_STR raws) {
    COMMAND_CODE matchCode = COMMAND_FAILED;

    for (mt::CR_STR str : raws) {
      if (hyphens + keyword == str) {
        matchCode = COMMAND_ONGOING;
      }
      else {
        getChildren()->iterate(
          mt_ds::GeneralTree::RIGHT,
          [&](mt_ds::LinkedList *node)->bool {

            if (static_cast<Command*>(node)->hyphens
              + static_cast<Command*>(node)->keyword == str
            ) {
              matchCode = COMMAND_REQUIRED;
              return false;
            }

            return true;
          }
        );

        if (matchCode == COMMAND_ONGOING) {
          pushUnormap(str);
        }
        else if (matchCode == COMMAND_REQUIRED) {
          break;
        }
      }
    }

    // ask 'raws' to the children
    if (matchCode == COMMAND_REQUIRED) {
      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {
          matchCode = static_cast<Command*>(node)->match(raws);
          return true;
        }
      );
    }

    if (matchCode == COMMAND_ONGOING) {
      return COMMAND_SUCCEEDED;
    }
    else if (dialogued) {
      return dialog();
    }
    return COMMAND_FAILED;
  }

  COMMAND_CODE Command::dialog() {
    std::string input;

    while (Language::cinDialogInput(input)) {

      if (Control::backTest(input)) {
        if (getParent()) {
          return static_cast<Command*>(getParent())->dialog();
        }
        else Language::printResponse(LANGUAGE_PARAMETER_AT_ROOT);
      }
      else if (Control::quitTest(input)) {
        Language::printResponse(LANGUAGE_PROGRAM_CANCELED);
        return COMMAND_CANCELED;
      }
      else if (Control::clipboardTest(input)) {
        copyPaste();
      }
      else if (Control::enterTest(input)) {
        size_t leafCount = 0;

        // count no-children node at children's level
        getChildren()->iterate(
          mt_ds::LinkedList::RIGHT,
          [&](mt_ds::LinkedList* node)->bool {

            if (!static_cast<mt_ds::GeneralTree*>(node)->getChildren()) {
              leafCount++;
            }

            return true;
          }
        );

        if (numberOfChildren() == leafCount) {
          if (false) {

          }
          else if (callback()) {
            
          }
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
        if (next()) {
          return static_cast<Command*>(next())->dialog();
        }
        else Language::printResponse(LANGUAGE_PARAMETER_ALONE);
      }
      else if (Control::previousTest(input)) {
        if (prev()) {
          return static_cast<Command*>(prev())->dialog();
        }
        else Language::printResponse(LANGUAGE_PARAMETER_ALONE);
      }
      else if (Control::resetTest(input)) {

      }
      else if (Control::selectTest(input)) {

      }
      else if (Control::viewTest(input)) {

      }
      else {}
    }

    return COMMAND_FAILED;
  }

  void Command::run(
    mt::CR_INT argc,
    char *argv[]
  ) {
    // register signal handler for Ctrl+C (SIGINT)
    std::signal(SIGINT, Language::setInterruptedCtrlC);

    COMMAND_CODE commandCode = match(
      mt_uti::StrTools::argvToStringVector(argc, argv)
    );
  }

  void Command::printHelp() {
    std::cout << "HELP INTERFACE:\n";
  }

  void Command::printList() {
    std::cout << "LIST INTERFACE:\n";
  }
}

#endif // __CLI_MENU__COMMAND_CPP__