#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.hpp"

namespace cli_menu {

  Command::Command(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) {
    keyword = keyword_in;
    description = description_in;
    if (callback_in) callback = callback_in;
  }

  std::string Command::generateSequentialRootNames() {
    std::string sequentialNames;

    bubble([&](mt_ds::LinkedList *node)->bool {

      sequentialNames = (static_cast<Command*>(node)->getParent() ?
        static_cast<Command*>(node)->hyphens : ""
      ) + static_cast<Command*>(node)->keyword + ' ' + sequentialNames;

      return true;
    });

    return sequentialNames;
  }

  Command *Command::match() {
    Command *firstNeighbor = nullptr,
      *firstChild = nullptr;

    /**
     * The string vector will 'pop_back()'
     * until it is empty to stop this loop.
     */
    while (!Command::raws.empty()) {

      // find first child of the same keyword with 'raws.back()'
      if (getChildren()) {
        resetPointer();

        static_cast<Command*>(getChildren())->iterate(
          mt_ds::LinkedList::RIGHT,
          [&](mt_ds::LinkedList *node)->bool {

            if (static_cast<Command*>(node)->testHyphens(Command::raws.back())) {
              firstChild = static_cast<Command*>(node);
              return false;
            }

            return true;
          }
        );
      }

      // find first neighbor of the same keyword with 'raws.back()'
      if (!firstChild) {
        iterate(
          mt_ds::LinkedList::RIGHT,
          [&](mt_ds::LinkedList *node)->bool {

            if (node != this &&
              static_cast<Command*>(node)->testHyphens(Command::raws.back())
            ) {
              firstNeighbor = static_cast<Command*>(node);
              return false;
            }

            return true;
          }
        );
      }

      // keyword is detected
      if (firstChild || firstNeighbor) {
        Command::raws.pop_back();

        // pseudo-child callbacks and program ends at initial match
        if (firstChild && firstChild->pseudo) {
          firstChild->triggerCallbacks();
          return setStatus(COMMAND_PSEUDO_ENDED);
        }
        /**
         * The match will be paused until arguments are given from the dialog.
         * The 'Command::raws' is not 'pop_back()'.
         */
        else if (
          required.first && isDialogued() &&
          stringifiedTypeIndex != STRINGIFIED_TYPE_BOOLEAN
        ) {
          printWelcome();

          Langu::ageMessage::printTemplateResponse(
            SENTENCE_PARAMETER_REQUIRED,
            keyword
          );

          Command::interruptionDialogued = true;
          return dialog();
        }
        else { // go to other node

          // the required boolean automatically has value 'true'
          if (required.first && stringifiedTypeIndex == STRINGIFIED_TYPE_BOOLEAN) {
            pushUnormap("1");
          }

          if (firstChild) return firstChild->match();
          return firstNeighbor->match();
        }
      }
      else { // push argument to 'Result'
        pushUnormap(Command::raws.back());
        Command::raws.pop_back();
      }
    }

    // extended runtime input
    if (required.first && isDialogued()) {
      printWelcome();

      Langu::ageMessage::printTemplateResponse(
        SENTENCE_PARAMETER_REQUIRED,
        keyword
      );

      return dialog();
    }

    Command *firstRequiredNeighbor = strictParentHasRequired(false);

    // uncompleted required neighbors with strict parent
    if (firstRequiredNeighbor) {
      return firstRequiredNeighbor->dialog();
    }
    // parent may not be strict, but at least one required child must be completed
    else if (hasChildren()) {

      Command *firstRequiredChild = nullptr;
      resetPointer();

      // find first required child
      getChildren()->iterate(
        mt_ds::LinkedList::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          if (static_cast<Command*>(node)->required.first) {
            firstRequiredChild = static_cast<Command*>(node);
            return false;
          }

          return true;
        }
      );

      // go to first required child
      if (firstRequiredChild) {
        Langu::ageMessage::printTemplateResponse(
          SENTENCE_PARAMETER_REQUIRED,
          firstRequiredChild->keyword
        );

        return firstRequiredChild->dialog();
      }
    }

    // completed required neighbors / non-strict parent
    return igniteCallbacks();
  }

  Command *Command::setStatus(const COMMAND_CODE & code) {
    statusCode = code;
    return this;
  }

  Command *Command::dialog() {
    Command::phaseCode = COMMAND_PHASE_DIALOG;
    std::string input;

    // outline or fill style
    Console::logStylishHeader(
      generateSequentialRootNames(),
      editing
    );

    printControllersHints();

    while (Control::cinDialogInput(input, editing)) {
      // HELP
      if (Control::helpTest(input)) {
        printHelp();
      }
      // LIST
      else if (Control::listTest(input)) {
        printList(CONSOLE_HINT_2, 0, true);
      }
      // ENTER
      else if (Control::enterTest(input)) {        
        Command *lastNode = enter();
        if (lastNode->statusCode != COMMAND_ONGOING) return lastNode;
      }
      // BACK
      else if (Control::backTest(input)) {
        if (getParent()) {
          // moving is prohibited
          if (Command::interruptionDialogued) {
            printInterruptionDialoguedResponse();
          }
          // go to parent
          else return static_cast<Command*>(getParent())->dialog();
        }
        // this is root
        else Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_ROOT);
      }
      // NEXT
      else if (Control::nextTest(input)) {
        Command *lastNode = goToNeighbor(mt_ds::GeneralTree::RIGHT);
        if (lastNode->statusCode != COMMAND_ONGOING) return lastNode;
      }
      // PREVIOUS
      else if (Control::previousTest(input)) {
        Command *lastNode = goToNeighbor(mt_ds::GeneralTree::LEFT);
        if (lastNode->statusCode != COMMAND_ONGOING) return lastNode;
      }
      // MODIFY
      else if (Control::modifyTest(input)) {
        // cannot repeat
        if (editing) {
          Langu::ageMessage::printResponse(SENTENCE_ALREADY_MODIFYING);
        }
        else { // switch to edit mode
          editing = true;
          return dialog();
        }
      }
      // SELECT
      else if (Control::selectTest(input)) {
        // switching mode is prohibited
        if (Command::interruptionDialogued) {
          printInterruptionDialoguedResponse();
        }
        else {
          // switch to selection mode
          if (editing) {
            editing = false;
            return dialog();
          }
          // cannot repeat
          else Langu::ageMessage::printResponse(SENTENCE_ALREADY_SELECTING);
        }
      }
      // RESET
      else if (Control::resetThisTest(input)) {
        resetUnormap();
      }
      else if (Control::resetAllTest(input)) {
        Result::clearAll();
      }
      // VIEW
      else if (Control::viewThisTest(input)) {
        Result::printInputs(this);
      }
      else if (Control::viewAllTest(input)) {
        Result::printInputs(nullptr);
      }
      // CLIPBOARD COPY
      else if (Control::copyTest(input)) {
        clipboardCopy();
      }
      // CLIPBOARD PASTE
      else if (Control::pasteTest(input)) {
        if (editing) clipboardPaste();
        else Langu::ageMessage::printResponse(SENTENCE_FORBIDDEN_HIDDEN_PASTE);
      }
      // QUIT
      else if (Control::quitTest(input)) {
        break;
      }
      // WILD VALUE
      else {
        // push argument to 'Result'
        if (editing) {
          pushUnormap(input);
        }
        // selection (match in dialog)
        else {
          Command *lastNode = goDown(input);
          if (lastNode->statusCode != COMMAND_ONGOING) return lastNode;
        }
      }
    }

    return setStatus(COMMAND_TERMINATED);
  }

  Command *Command::enter() {

    // continue the interrupted match
    if (Command::interruptionDialogued && !required.first) {
      Command::interruptionDialogued = false;
      return match();
    }
    // trying to go down
    else {
      Command *firstRequiredNeighbor = strictParentHasRequired(true);

      // go to children level
      if (!firstRequiredNeighbor && hasChildren()) {
        Command *firsOrthoChild = nullptr;
        resetPointer();

        // find first ortho child
        getChildren()->iterate(
          mt_ds::GeneralTree::RIGHT,
          [&](mt_ds::LinkedList *node)->bool {

            if (!static_cast<Command*>(node)->pseudo) {
              firsOrthoChild = static_cast<Command*>(node);
              return false;
            }

            return true;
          }
        );

        if (firsOrthoChild) return firsOrthoChild->dialog();
      }
      // uncompleted required neighbors with strict parent
      else if (firstRequiredNeighbor) {
        return setStatus(COMMAND_ONGOING);
      }
    }

    // no required nodes (done)
    return igniteCallbacks();
  }

  COMMAND_CALLBACK_CODE Command::iterateInOutCallbacks(
    const std::function<bool(Command*)> &asWhatCallback
  ) {
    COMMAND_CALLBACK_CODE callbackCode;
    bool anyFailed = false, anyCanceled = false;

    if (hasChildren()) {
      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          if (static_cast<Command*>(node)->callback &&
            asWhatCallback(static_cast<Command*>(node))
          ) {
            callbackCode = static_cast<Command*>(node)->callback(
              static_cast<Command*>(node)
            );

            if (callbackCode == COMMAND_CALLBACK_FAILED) {
              anyFailed = true;
            }
            else if (callbackCode == COMMAND_CALLBACK_CANCELED) {
              anyCanceled = true;
            }
          }

          return true;
        }
      );
    }

    if (anyFailed) return COMMAND_CALLBACK_FAILED;
    else if (anyCanceled) return COMMAND_CALLBACK_CANCELED;
    return COMMAND_CALLBACK_SUCCEEDED;
  }

  COMMAND_CALLBACK_CODE Command::triggerCallbacks() {
    if (!asInput && !asOutput) {
      // input
      COMMAND_CALLBACK_CODE inputCallbackCode = iterateInOutCallbacks(
        [](Command *current)->bool { return current->asInput; }
      );

      // process
      COMMAND_CALLBACK_CODE processCallbackCode = COMMAND_CALLBACK_SUCCEEDED;
      if (callback) processCallbackCode = callback(this);

      // output
      COMMAND_CALLBACK_CODE outputCallbackCode = iterateInOutCallbacks(
        [](Command *current)->bool { return current->asOutput; }
      );

      if (outputCallbackCode != COMMAND_CALLBACK_SUCCEEDED) {
        return outputCallbackCode;
      }
      else if (processCallbackCode != COMMAND_CALLBACK_SUCCEEDED) {
        return processCallbackCode;
      }
      else if (inputCallbackCode != COMMAND_CALLBACK_SUCCEEDED) {
        return inputCallbackCode;
      }
    }

    return COMMAND_CALLBACK_SUCCEEDED;
  }

  Command *Command::igniteCallbacks() {
    if (isPropagated()) {
      COMMAND_CODE propagatingCode;
      COMMAND_CALLBACK_CODE callbackCode;

      bubble([&](mt_ds::LinkedList *node)->bool {
        callbackCode = static_cast<Command*>(node)->triggerCallbacks();

        if (callbackCode == COMMAND_CALLBACK_FAILED) {
          propagatingCode = COMMAND_FAILED;
          return false;
        }
        else if (callbackCode == COMMAND_CALLBACK_CANCELED) {
          propagatingCode = COMMAND_CANCELED;
          return false;
        }
        else propagatingCode = COMMAND_SUCCEEDED;

        return static_cast<Command*>(node)->localPropagation;
      });

      return setStatus(propagatingCode);
    }
    else { // not propagated
      COMMAND_CALLBACK_CODE callbackCode = triggerCallbacks();

      if (callbackCode == COMMAND_CALLBACK_FAILED) {
        return setStatus(COMMAND_FAILED);
      }
      else if (callbackCode == COMMAND_CALLBACK_CANCELED) {
        return setStatus(COMMAND_CANCELED);
      }

      return setStatus(COMMAND_SUCCEEDED);
    }
  }

  // always in selection mode
  Command *Command::goDown(mt::CR_STR input) {

    bool spaceFound = false;
    mt::VEC_STR additionalRaws = {""};
    Command::phaseCode = COMMAND_PHASE_MATCH_IN_DIALOG;

    // split input into the string vector using spaces as delimiters
    for (mt::CR_CH ch : input) {

      if (mt_uti::StrTools::isWhitespace(ch)) {
        if (!spaceFound) {
          spaceFound = true;
          additionalRaws.push_back("");
        }
      }
      else {
        spaceFound = false;
        additionalRaws.back() += ch;
      }
    }

    // reverse additional raws order
    std::reverse(additionalRaws.begin(), additionalRaws.end());

    // insert to back of main raws
    mt_uti::VecTools<std::string>::concatCopy(
      Command::raws, additionalRaws
    );

    // find first child by keyword possibility at back of string vector
    Command *firstSelected = nullptr;

    if (getChildren()) {
      resetPointer();
      
      getChildren()->iterate(
        mt_ds::LinkedList::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          if (static_cast<Command*>(node)->testHyphens(
            Command::raws.back()
          )) {
            firstSelected = static_cast<Command*>(node);
            return false;
          }

          return true;
        }
      );
    }

    // match in dialog
    if (firstSelected) {
      // still on the current node
      if (firstSelected->pseudo) {
        firstSelected->igniteCallbacks();
      }
      // move to child
      else if (!strictParentHasRequired(false)) {
        Command *lastNode = firstSelected->match();

        /**
         * Exclude the return of 'COMMAND_PSEUDO_ENDED' to prevent the program
         * from terminating when selecting a node followed by its pseudo-child.
         */
        if (lastNode->statusCode != COMMAND_ONGOING &&
          lastNode->statusCode != COMMAND_PSEUDO_ENDED
        ) return lastNode;
      }
    }
    else {
      // child not found
      if (hasChildren()) {
        Langu::ageMessage::printResponse(SENTENCE_PARAMETER_NOT_FOUND);
      }
      else { // this is a leaf
        Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);
      }
    }

    // remove the recently added strings
    if (!Command::raws.empty()) {
      mt_uti::VecTools<std::string>::eraseIntervalStable(
        Command::raws,
        {
          Command::raws.size() - additionalRaws.size(),
          Command::raws.size() - 1
        }
      );
    }

    return setStatus(COMMAND_ONGOING);
  }

  Command *Command::goToNeighbor(
    const mt_ds::GeneralTree::DIRECTION &direction
  ) {
    Command *firstOrthoNeighbor = nullptr;

    // find first ortho neighbor
    iterate(
      direction,
      [&](mt_ds::LinkedList *node)->bool {

        if (node != this &&
          !static_cast<Command*>(node)->pseudo
        ) {
          firstOrthoNeighbor = static_cast<Command*>(node);
          return false;
        }

        return true;
      }
    );

    if (firstOrthoNeighbor) {
      // moving is prohibited
      if (Command::interruptionDialogued) {
        printInterruptionDialoguedResponse();
      }
      // go to neighbor
      else return static_cast<Command*>(firstOrthoNeighbor)->dialog();
    }
    // has no neighbors
    else Langu::ageMessage::printResponse(SENTENCE_PARAMETER_ALONE);

    return setStatus(COMMAND_ONGOING);
  }

  void Command::printWelcome() {
    static bool displayed = false;

    if (!displayed && !getParent()) {
      displayed = true;

      // keyword
      Console::logString(
        Langu::ageMessage::getWelcomeToString() +
        mt_uti::StrTools::copyStringToUppercase(keyword) + '\n',
        Console::messageColors[CONSOLE_HINT_1]
      );

      // description
      Console::logItalicString(
        Command::description + "\n\n",
        Console::messageColors[CONSOLE_HINT_2]
      );
    }
  }

  void Command::printControllersHints() {
    static bool displayed = false;

    if (!displayed) {
      displayed = true;
      Control::printAbbreviations(true, 2);
      Control::printBooleanAvailableValues(true, 2);
    }
  }

  void Command::printHelp() {
    printKeyword(CONSOLE_HINT_1, 0);

    // description
    Console::logItalicString(
      description + '\n',
      Console::messageColors[CONSOLE_HINT_2]
    );

    printList(CONSOLE_HINT_3, 2, false);
  }

  void Command::printKeyword(
    const CONSOLE_CODE &consoleCode,
    mt::CR_SZ numberOfIndents
  ) {
    Console::logString(
      std::string(numberOfIndents, ' ') + keyword + " ["
      + Langu::ageCommand::getStringifiedType(stringifiedTypeIndex)
      + ']' + (required.first ? '*' : '\0') + '\n',
      Console::messageColors[consoleCode]
    );
  }

  void Command::printList(
    const CONSOLE_CODE &consoleCode,
    mt::CR_SZ numberOfIndents,
    mt::CR_BOL displayAtLeafWarning
  ) {
    if (hasChildren()) {
      resetPointer();

      // print children keyword
      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          if (!static_cast<Command*>(node)->pseudo) {
            static_cast<Command*>(node)->printKeyword(
              consoleCode, numberOfIndents
            );
          }

          return true;
        }
      );
    }
    // print warning
    else if (displayAtLeafWarning) {
      Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);
      return;
    }

    // additional newline only at runtime
    if (Command::phaseCode == COMMAND_PHASE_DIALOG ||
      Command::phaseCode == COMMAND_PHASE_MATCH_IN_DIALOG
    ) {
      std::cout << std::endl;
    }
  }

  void Command::printInterruptionDialoguedResponse() {
    Langu::ageMessage::printResponse(required.first?
      SENTENCE_ARGUMENT_REQUIRED:
      SENTENCE_INTERRUPTION_DIALOG
    );
  }

  Command *Command::strictParentHasRequired(mt::CR_BOL onlyOrtho) {
    Command *found = nullptr;

    iterate(
      mt_ds::LinkedList::RIGHT,
      [&](mt_ds::LinkedList *node)->bool {

        if (static_cast<Command*>(node)->required.first &&
          (!onlyOrtho || (onlyOrtho && !static_cast<Command*>(node)->pseudo))
        ) {
          found = static_cast<Command*>(node);
          return false;
        }

        return true;
      }
    );

    if (found) {
      // strict parent disallowed
      if (getParent() &&
        static_cast<Command*>(getParent())->strict
      ) {
        Langu::ageMessage::printResponse(SENTENCE_PARENT_STRICT);
      }
      // root or non-strict parent with this is required
      else if (found == this && (!getParent() || (
        getParent() && !static_cast<Command*>(getParent())->strict
      ))) {
        // displayed at match in dialog
        Langu::ageMessage::printResponse(SENTENCE_ARGUMENT_REQUIRED);
      }
      else found = nullptr;
    }

    /**
     * If 'found' is nullptr it means
     * strict parent allowed or parent is non-strict.
     */
    return found;
  }

  void Command::makePseudo(mt::CR_BOL condition) {
    if (getParent()) {
      pseudo = condition;
      localPropagation = !condition;
      static_cast<Command*>(getParent())->pseudosCount += 1 - !condition * 2;
    }
  }

  void Command::makeSterilized(
    mt::CR_BOL condition,
    mt::CR_BOL willDestroy
  ) {
    if (condition && willDestroy && getChildren()) {
      getChildren()->destroy();
    }

    sterilized = condition;
  }

  /** From class 'Result' */

  void Result::printInputs(Command *onlyThis) {
    if (onlyThis) {
      if (!onlyThis->printInput()) {
        Langu::ageMessage::printResponse(SENTENCE_EMPTY_SINGLE_VIEW_INPUT);
      }
    }
    else {
      printType<std::string>(
        Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_WORD),
        words
      );

      printType<mt::LD>(
        Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_NUMBER),
        numbers
      );

      printType<bool>(
        Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_BOOLEAN),
        booleans
      );
    }
  }
}

#endif // __CLI_MENU__COMMAND_CPP__