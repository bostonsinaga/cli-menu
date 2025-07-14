#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

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

    if (matchCode == COMMAND_ONGOING) {
      return COMMAND_SUCCEEDED;
    }
    else if (dialogued) {
      return dialog();
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
    else {
      if (propagation) {
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

  COMMAND_CODE Command::dialog() {
    std::string input;

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
        printList();
      }
      // MODIFY
      else if (Control::modifyTest(input)) {
        selecting = false;
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
        selecting = true;
        return dialog();
      }
      // VIEW
      else if (Control::viewTest(input)) {
        Result::printInputs();
      }
      // RESULT INPUT OR MATCH IN DIALOG
      else {
        COMMAND_CODE code = resultInput(input);
        if (code != COMMAND_ONGOING) return code;
      }
    }

    return COMMAND_FAILED;
  }

  void Command::printHelp() {
    std::cout << "HELP INTERFACE:\n";
  }

  void Command::printList() {
    std::cout << "LIST INTERFACE:\n";
  }
}

#endif // __CLI_MENU__COMMAND_CPP__