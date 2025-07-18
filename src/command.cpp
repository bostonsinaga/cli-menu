#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include <csignal>
#include "command.hpp"

namespace cli_menu {

  Command::Command(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    const COMMAND_CALLBACK &callback_in
  ) {
    keyword = keyword_in;
    description = description_in;
    callback = callback_in;
  }

  std::string Command::generateSequentialRootNames() {
    std::string sequentialNames;

    bubble([&](mt_ds::LinkedList *node)->bool {
      sequentialNames = static_cast<Command*>(node)->keyword + ' ' + sequentialNames;
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
    static bool hinted = false;
    std::string input;

    // outline or fill style
    Console::logStylishHeader(
      generateSequentialRootNames(),
      editing
    );

    // hints for controllers
    if (!hinted) {
      hinted = true;
      Control::printAbbreviations();
      Control::printToggleAvailableValues();
    }

    while (Control::cinDialogInput(input, editing)) {
      // HELP
      if (Control::helpTest(input)) {
        printHelp();
      }
      // LIST
      else if (Control::listTest(input)) {
        printList(true);
      }
      // ENTER
      else if (Control::enterTest(input)) {
        COMMAND_CODE code = enter();
        if (code != COMMAND_ONGOING) return code;
      }
      // BACK
      else if (Control::backTest(input)) {
        if (getParent()) {
          return static_cast<Command*>(getParent())->dialog();
        }
        else Language::printResponse(LANGUAGE_PARAMETER_AT_ROOT);
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
      // MODIFY
      else if (Control::modifyTest(input)) {
        if (editing) Language::printResponse(LANGUAGE_ALREADY_MODIFYING);
        else editing = true;
        return dialog();
      }
      // SELECT
      else if (Control::selectTest(input)) {
        if (editing) editing = false;
        else Language::printResponse(LANGUAGE_ALREADY_SELECTING);
        return dialog();
      }
      // RESET
      else if (Control::resetTest(input)) {
        resetUnormap();
      }
      // VIEW
      else if (Control::viewTest(input)) {
        Result::printInputs(editing);
      }
      // CLIPBOARD COPY
      else if (Control::copyTest(input)) {
        copyPaste();
      }
      // CLIPBOARD PASTE
      else if (Control::pasteTest(input)) {
        copyPaste();
      }
      // QUIT
      else if (Control::quitTest(input)) {
        return COMMAND_CANCELED;
      }
      // WILD VALUE
      else {
        // edit mode
        if (editing) {
          required = false;
          pushUnormap(input);
        }
        // selection mode (can be a match in dialog)
        else {
          COMMAND_CODE code = goDown(input);
          if (code != COMMAND_ONGOING) return code;
        }
      }
    }

    return COMMAND_FAILED;
  }

  COMMAND_CODE Command::enter() {
    bool neighborRequired = false;

    // check required nodes at current level
    iterate(
      mt_ds::LinkedList::RIGHT,
      [&](mt_ds::LinkedList* node)->bool {

        // at least 1 is required
        if (neighborRequired) return false;

        if (static_cast<Command*>(node)->required) {
          neighborRequired = true;
        }

        return true;
      }
    );

    // uncompleted required neighbors with strict parent
    if (getParent() && static_cast<Command*>(getParent())->strict && neighborRequired) {
      Language::printResponse(LANGUAGE_PARENT_STRICT);
      return COMMAND_ONGOING;
    }
    // no parent or non-strict parent with this is required
    else if (
      (getParent() && !static_cast<Command*>(getParent())->strict && required) ||
      (!getParent() && required)
    ) {
      Language::printResponse(LANGUAGE_ARGUMENT_REQUIRED);
      return COMMAND_ONGOING;
    }
    // go to children level
    else if (getChildren()) {
      return static_cast<Command*>(getChildren())->dialog();
    }

    // no required nodes (done)
    return callCallback();
  }

  COMMAND_CODE Command::callCallback() {

    if (Command::propagation) {
      COMMAND_CODE propagatingCode;

      bubble([&](mt_ds::LinkedList *node)->bool {

        if (static_cast<Command*>(node)->callback()) {
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

  COMMAND_CODE Command::goToNeighbor(mt_ds::LinkedList* node) {
    if (node) {
      return static_cast<Command*>(node)->dialog();
    }
    else Language::printResponse(LANGUAGE_PARAMETER_ALONE);

    return COMMAND_ONGOING;
  }

  // always in selection mode
  COMMAND_CODE Command::goDown(mt::CR_STR input) {
    bool willGo = false;

    if (getChildren()) {
      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          if (static_cast<Command*>(node)->keyword == input) {
            willGo = true;
            return false;
          }

          return true;
        }
      );

      // go to children level
      if (willGo) {
        return static_cast<Command*>(getChildren())->dialog();
      }

      /**
       * Find first whitespace to indicate that
       * 'input' is raws for match in dialog.
       */
      bool spaceFound = false;

      for (mt::CR_CH ch : input) {
        if (mt_uti::StrTools::isWhitespace(ch)) {
          spaceFound = true;
          break;
        }
      }

      // match in dialog
      if (spaceFound) {
        spaceFound = false;
        mt::VEC_STR raws = {""};

        for (mt::CR_CH ch : input) {
          if (!spaceFound && mt_uti::StrTools::isWhitespace(ch)) {
            spaceFound = true;
            raws.push_back("");
          }
          else raws.back() += ch;
        }

        return match(raws);
      }

      // child not found
      Language::printResponse(LANGUAGE_PARAMETER_NOT_FOUND);
    }
    else { // this is a leaf
      Language::printResponse(LANGUAGE_PARAMETER_AT_LEAF);
    }

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