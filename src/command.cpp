#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include <csignal>
#include "command.hpp"

namespace cli_menu {

  Command::Command(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    const COMMAND_CALLBACK &callback_in
  ) {
    keyword = keyword_in;
    description = description_in;
    required = required_in;
    callback = callback_in;
  }

  std::string Command::generateSequentialRootNames() {
    std::string sequentialNames;

    bubble([&](mt_ds::LinkedList *neighbor)->bool {
      sequentialNames = static_cast<Command*>(neighbor)->keyword + ' ' + sequentialNames;
      return true;
    });

    return sequentialNames;
  }

  COMMAND_CODE Command::match(mt::CR_VEC_STR raws) {
    COMMAND_CODE matchCode = COMMAND_FAILED;

    for (mt::CR_STR str : raws) {
      // the root automatically passes the keyword check
      if (hyphens + keyword == str || !getParent()) {
        matchCode = COMMAND_ONGOING;
      }
      // check child keyword or as argument to current node
      else {
        if (getChildren()) {
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
        }

        if (matchCode == COMMAND_ONGOING) {
          pushUnormap(str);
        }
        else if (matchCode == COMMAND_REQUIRED) {
          break;
        }
      }
    }

    // ask 'raws' to the children
    if (matchCode == COMMAND_REQUIRED && getChildren()) {
      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {
          matchCode = static_cast<Command*>(node)->match(raws);
          return true;
        }
      );
    }

    // not required
    if (matchCode == COMMAND_ONGOING) {
      return COMMAND_SUCCEEDED;
    }
    // required but has no children
    else if (Command::dialogued) {
      return dialog();
    }
    // no dialogue to complete the required
    return COMMAND_FAILED;
  }

  COMMAND_CODE Command::dialog() {
    std::string input;

    Console::logStylishHeader(
      generateSequentialRootNames(),
      selecting
    );

    while (Control::cinDialogInput(input)) {

      // BACK
      if (Control::backTest(input)) {
        if (getParent()) {
          return static_cast<Command*>(getParent())->dialog();
        }
        else Language::printResponse(LANGUAGE_PARAMETER_AT_ROOT);
      }
      // CLIPBOARD
      else if (Control::clipboardTest(input)) {
        copyPaste();
      }
      // ENTER
      else if (Control::enterTest(input)) {
        COMMAND_CODE code = enter();
        if (code != COMMAND_ONGOING) return code;
      }
      // HELP
      else if (Control::helpTest(input)) {
        printHelp();
      }
      // LIST
      else if (Control::listTest(input)) {
        printList(true);
      }
      // MODIFY
      else if (Control::modifyTest(input)) {
        if (selecting) selecting = false;
        else Language::printResponse(LANGUAGE_ALREADY_MODIFYING);
        return dialog();
      }
      // NEXT
      else if (Control::nextTest(input)) {
        COMMAND_CODE code = goToNeighbor(next());
        if (code != COMMAND_ONGOING) return code;
      }
      // PREVIOUS
      else if (Control::previousTest(input)) {
        COMMAND_CODE code = goToNeighbor(prev());
        if (code != COMMAND_ONGOING) return code;
      }
      // QUIT
      else if (Control::quitTest(input)) {
        return COMMAND_CANCELED;
      }
      // RESET
      else if (Control::resetTest(input)) {
        resetUnormap();
      }
      // SELECT
      else if (Control::selectTest(input)) {
        if (selecting) Language::printResponse(LANGUAGE_ALREADY_SELECTING);
        else selecting = true;
        return dialog();
      }
      // VIEW
      else if (Control::viewTest(input)) {
        Result::printInputs(selecting);
      }
      // RESULT INPUT OR MATCH IN DIALOG
      else {
        bool goDown = false;

        if (getChildren()) {
          getChildren()->iterate(
            mt_ds::GeneralTree::RIGHT,
            [&](mt_ds::LinkedList *node)->bool {

              if (static_cast<Command*>(node)->keyword == input) {
                goDown = true;
                return false;
              }

              return true;
            }
          );
        }

        if (goDown) {
          return static_cast<Command*>(getChildren())->dialog();
        }
        else {
          COMMAND_CODE code = resultInput(input);
          if (code != COMMAND_ONGOING) return code;
        }
      }
    }

    return COMMAND_FAILED;
  }

  COMMAND_CODE Command::enter() {
    size_t requiredCount = 0;

    // count 'required' nodes
    iterate(
      mt_ds::LinkedList::RIGHT,
      [&](mt_ds::LinkedList* node)->bool {

        if (!static_cast<Command*>(node)->required) {
          requiredCount++;
        }

        return true;
      }
    );

    // required nodes still exist
    if (requiredCount) {
      return COMMAND_ONGOING;
    }

    // no required nodes
    return callCallback();
  }

  COMMAND_CODE Command::callCallback() {

    if (Command::propagation) {
      COMMAND_CODE propagatingCode;

      bubble([&](mt_ds::LinkedList *neighbor)->bool {
        if (callback()) {
          propagatingCode = COMMAND_SUCCEEDED;
          return true;
        }

        propagatingCode = COMMAND_FAILED;
        return false;
      });

      return propagatingCode;
    }
    else { // not propagated
      if (callback()) {
        return COMMAND_SUCCEEDED;
      }

      return COMMAND_FAILED;
    }
  }

  COMMAND_CODE Command::goToNeighbor(mt_ds::LinkedList* neighbor) {
    if (neighbor) {
      return static_cast<Command*>(neighbor)->dialog();
    }
    else Language::printResponse(LANGUAGE_PARAMETER_ALONE);

    return COMMAND_ONGOING;
  }

  COMMAND_CODE Command::resultInput(mt::CR_STR input) {
    if (selecting) {
      mt::VEC_STR raws = {""};
      bool spaceFound = false;

      for (mt::CR_CH ch : input) {
        if (!spaceFound && (
          ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
        ) {
          spaceFound = true;
          raws.push_back("");
        }
        else raws.back() += ch;
      }

      return match(raws);
    }

    pushUnormap(input);
    return COMMAND_ONGOING;
  }

  void Command::printHelp() {

    Console::logResponse(
      CONSOLE_HIGHLIGHT,
      keyword + " [" + Language::getStringifiedType(
        stringifiedTypeIndex
      ) + "]\n" + description + "\n\n"
    );

    printList(false);
    std::cout << std::endl;
  }

  void Command::printList(mt::CR_BOL needErrorMessage) {
    if (getChildren()) {
      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          Console::logResponse(
            CONSOLE_HINT,
            static_cast<Command*>(node)->keyword + " ["
            + Language::getStringifiedType(stringifiedTypeIndex) + "]\n"
          );

          return true;
        }
      );
    }
    else if (needErrorMessage) {
      Language::printResponse(LANGUAGE_PARAMETER_AT_LEAF);
    }
  }

  void Command::run(mt::CR_INT argc, char *argv[]) {

    // register signal handler for Ctrl+C (SIGINT)
    std::signal(SIGINT, Control::setInterruptedCtrlC);

    COMMAND_CODE commandCode = match(
      mt_uti::StrTools::argvToStringVector(argc, argv)
    );

    switch (commandCode) {
      case COMMAND_FAILED: {
        Language::printResponse(LANGUAGE_PROGRAM_FAILED);
      break;}
      case COMMAND_SUCCEEDED: {
        Language::printResponse(LANGUAGE_PROGRAM_SUCCEEDED);
      break;}
      case COMMAND_CANCELED: {
        Language::printResponse(LANGUAGE_PROGRAM_CANCELED);
      break;}
      default: {
        // COMMAND_ONGOING, COMMAND_REQUIRED
      }
    }
  }
}

#endif // __CLI_MENU__COMMAND_CPP__