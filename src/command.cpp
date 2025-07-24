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

  COMMAND_CODE Command::match() {
    Command *neighbor = nullptr, *child = nullptr;

    // the match will be paused until arguments are given from the dialog
    mt::CR_BOL needDialog = required && Command::globalDialogued && localDialogued;

    while (!Command::raws.empty()) {

      // neighbors iteration
      if (!isolated()) iterate(
        mt_ds::LinkedList::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {
          if (node != this) {

            // find keyword
            if (static_cast<Command*>(node)->testHyphens(
              Command::raws.back()
            )) {
              neighbor = static_cast<Command*>(node);
              return false;
            }
          }

          return true;
        }
      );

      // children iteration
      if (!neighbor && getChildren()) getChildren()->iterate(
        mt_ds::LinkedList::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          // find keyword
          if (static_cast<Command*>(node)->testHyphens(
            Command::raws.back()
          )) {
            child = static_cast<Command*>(node);
            return false;
          }

          return true;
        }
      );

      // keyword detected
      if (neighbor || child) {

        // interruption dialogue
        if (needDialog) { // the 'Command::raws' is not 'pop_back()'
          Language::printResponse(LANGUAGE_ARGUMENT_REQUIRED);
          return dialog();
        }
        else { // go to other node
          Command::raws.pop_back();
          if (neighbor) return neighbor->match();
          return child->match();
        }
      }
      else { // push argument to 'Result'
        pushUnormap(Command::raws.back());
        Command::raws.pop_back();
      }
    }

    // extended runtime input
    if (needDialog) return dialog();

    // no required nodes (done)
    return callCallback();
  }

  COMMAND_CODE Command::dialog() {

    static bool hinted = false;
    std::string input;

    // prohibit controllers
    bool interruptionDialogue = !Command::raws.empty();

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
        printList(false);
      }
      // ENTER
      else if (Control::enterTest(input)) {
        // continue the interrupted match
        if (interruptionDialogue) {
          return static_cast<Command*>(getChildren())->match();
        }
        else { // go down
          COMMAND_CODE code = enter();
          if (code != COMMAND_ONGOING) return code;
        }
      }
      // BACK
      else if (Control::backTest(input)) {
        if (getParent()) {
          // moving is prohibited
          if (interruptionDialogue) {
            Language::printResponse(LANGUAGE_MIDDLE_DIALOG);
          }
          // go to parent
          else return static_cast<Command*>(getParent())->dialog();
        }
        // this is root
        else Language::printResponse(LANGUAGE_PARAMETER_AT_ROOT);
      }
      // NEXT
      else if (Control::nextTest(input)) {
        // moving is prohibited
        if (interruptionDialogue) {
          Language::printResponse(LANGUAGE_MIDDLE_DIALOG);
        }
        else { // go to neighbor
          COMMAND_CODE code = goToNeighbor(next());
          if (code != COMMAND_ONGOING) return code;
        }
      }
      // PREVIOUS
      else if (Control::previousTest(input)) {
        // moving is prohibited
        if (interruptionDialogue) {
          Language::printResponse(LANGUAGE_MIDDLE_DIALOG);
        }
        else { // go to neighbor
          COMMAND_CODE code = goToNeighbor(prev());
          if (code != COMMAND_ONGOING) return code;
        }
      }
      // MODIFY
      else if (Control::modifyTest(input)) {
        if (editing) Language::printResponse(LANGUAGE_ALREADY_MODIFYING);
        else { // switch to edit mode
          editing = true;
          return dialog();
        }
      }
      // SELECT
      else if (Control::selectTest(input)) {
        if (editing) { // switch to selection mode
          editing = false;
          return dialog();
        }
        else Language::printResponse(LANGUAGE_ALREADY_SELECTING);
      }
      // RESET
      else if (Control::resetTest(input)) {
        resetUnormap();
      }
      // VIEW
      else if (Control::viewTest(input)) {
        Result::printInputs();
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
        // push argument to 'Result'
        if (editing) {
          pushUnormap(input);
        }
        // selecting is prohibited
        else if (interruptionDialogue) {
          Language::printResponse(LANGUAGE_MIDDLE_DIALOG);
        }
        // selection (match in dialog)
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

    if (Command::globalPropagation && localPropagation) {
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

    if (getChildren()) {
      bool spaceFound = false;

      // vector refill
      if (Command::raws.empty()) {
        Command::raws.push_back("");
      }

      // split input into the string vector using spaces as delimiters
      for (mt::CR_CH ch : input) {

        if (mt_uti::StrTools::isWhitespace(ch)) {
          if (!spaceFound) {
            spaceFound = true;
            Command::raws.push_back("");
          }
        }
        else {
          spaceFound = false;
          Command::raws.back() += ch;
        }
      }

      // reverse the string vector order
      std::reverse(Command::raws.begin(), Command::raws.end());

      Command *selected = nullptr;

      // find child by keyword possibility at back of string vector
      getChildren()->iterate(
        mt_ds::LinkedList::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          if (static_cast<Command*>(node)->testHyphens(
            Command::raws.back()
          )) {
            selected = static_cast<Command*>(node);
            Command::raws.pop_back();
            return false;
          }

          return true;
        }
      );

      // match in dialog
      if (selected) {
        COMMAND_CODE code = selected->match();
        if (code != COMMAND_ONGOING) return code;
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
    // keyword
    Console::logString(
      "\n" + keyword + " ["
      + Language::getStringifiedType(stringifiedTypeIndex) + "]\n",
      Console::messageColors[CONSOLE_HINT_1]
    );

    // description
    Console::logItalicString(
      description + '\n',
      Console::messageColors[CONSOLE_HINT_2]
    );

    printList(true);
  }

  void Command::printList(mt::CR_BOL withHelp) {
    if (getChildren()) {
      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          Console::logString(
            "  " + static_cast<Command*>(node)->keyword + " ["
            + Language::getStringifiedType(stringifiedTypeIndex) + "]\n",
            Console::messageColors[withHelp ? CONSOLE_HINT_3 : CONSOLE_HINT_2]
          );

          return true;
        }
      );
    }
    // print error
    else if (!withHelp) {
      Language::printResponse(LANGUAGE_PARAMETER_AT_LEAF);
    }

    std::cout << std::endl;
  }

  void Command::run(mt::CR_INT argc, char *argv[]) {

    // register signal handler for Ctrl+C (SIGINT)
    std::signal(SIGINT, Control::setInterruptedCtrlC);

    // skip the first 'argv' which is unpredictable executable filename
    for (int i = argc - 1; i > 0; i--) {
      Command::raws.push_back(argv[i]);
    }

    // start recursion
    COMMAND_CODE commandCode = match();

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