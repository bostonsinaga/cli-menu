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

        static_cast<Command*>(getChildren())->forEach(
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
        forEach(
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
          stringifiedTypeIndex != STRINGIFIED_TYPE_INPUT_BOOLEAN
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
          if (required.first && stringifiedTypeIndex == STRINGIFIED_TYPE_INPUT_BOOLEAN) {
            pushInputUnormap("1");
          }

          if (firstChild) return firstChild->match();
          return firstNeighbor->match();
        }
      }
      else { // push argument to 'Data::Input'
        pushInputUnormap(Command::raws.back());
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
      getChildren()->forEach(
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

  void Command::setMode(mt::CR_BOL isSelecting) {
    editing = !isSelecting;
    static bool displayed[2] = {false, false};

    if (!displayed[isSelecting]) {
      displayed[isSelecting] = true;

      Langu::ageMessage::printResponse(isSelecting ?
        SENTENCE_MODE_SWITCH_TO_SELECTION : SENTENCE_MODE_SWITCH_TO_MODIFICATION
      );
    }
  }

  Command *Command::setStatus(const COMMAND_CODE & code) {
    statusCode = code;
    return this;
  }

  Command *Command::dialog() {
    Command::phaseCode = COMMAND_PHASE_DIALOG;
    std::string rawstr;

    // outline or fill style
    Console::logStylishHeader(
      generateSequentialRootNames(),
      editing
    );

    while (Control::cinDialogInput(rawstr, editing)) {
      // COMMAND HELP
      if (Control::commandHelpTest(rawstr)) {
        printHelp();
      }
      // CONTROLLER LIST
      else if (Control::controllerListTest(rawstr)) {
        Control::printAbbreviations(true, 2);
        Control::printBooleanAvailableValues(true, 2);
      }
      // ENTER CHILDREN
      else if (Control::childrenEnterTest(rawstr)) {        
        Command *lastNode = enter();
        if (lastNode->statusCode != COMMAND_ONGOING) return lastNode;
      }
      // LIST CHILDREN
      else if (Control::childrenListTest(rawstr)) {
        printList(CONSOLE_HINT_2, 0, true);
      }
      // NEXT NEIGHBOR
      else if (Control::neighborNextTest(rawstr)) {
        Command *lastNode = goToNeighbor(mt_ds::GeneralTree::RIGHT);
        if (lastNode->statusCode != COMMAND_ONGOING) return lastNode;
      }
      // PREVIOUS NEIGHBOR
      else if (Control::neighborPreviousTest(rawstr)) {
        Command *lastNode = goToNeighbor(mt_ds::GeneralTree::LEFT);
        if (lastNode->statusCode != COMMAND_ONGOING) return lastNode;
      }
      // MODIFY INPUT
      else if (Control::switchModifyTest(rawstr)) {
        // cannot repeat
        if (editing) {
          Langu::ageMessage::printResponse(SENTENCE_MODE_ALREADY_EDITING);
        }
        else { // switch to editing mode
          setMode(false);
          return dialog();
        }
      }
      // SELECT NODE
      else if (Control::switchSelectTest(rawstr)) {
        // switching mode is prohibited
        if (Command::interruptionDialogued) {
          printInterruptionDialoguedResponse();
        }
        else {
          // switch to selection mode
          if (editing) {
            setMode(true);
            return dialog();
          }
          // cannot repeat
          else Langu::ageMessage::printResponse(SENTENCE_MODE_ALREADY_SELECTING);
        }
      }
      // VIEW INPUT
      else if (Control::viewInputThisTest(rawstr)) {
        Data::Input::print(this);
      }
      else if (Control::viewOutputThisTest(rawstr)) {
        Data::Input::printAll();
      }
      // VIEW OUTPUT
      else if (Control::viewInputAllTest(rawstr)) {
        Data::Output::print(this);
      }
      else if (Control::viewOutputAllTest(rawstr)) {
        Data::Output::printAll();
      }
      // RESET THIS & DESCENDANTS INPUT/OUTPUT
      else if (Control::resetInputThisTest(rawstr)) {
        if (resetInputUnormapDescendants()) {
          Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_THIS);
        }
        else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
      }
      else if (Control::resetOutputThisTest(rawstr)) {
        // output only stores strings
        if (Data::Output::has(this)) {
          Data::Output::erase(this);
          Langu::ageMessage::printResponse(SENTENCE_RESET_OUTPUT_THIS);
        }
        else Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
      }
      // RESET ALL INPUT/OUTPUT
      else if (Control::resetInputAllTest(rawstr)) {
        if (Data::Input::clearAll()) {
          Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_ALL);
        }
        else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_ALL);
      }
      else if (Control::resetOutputAllTest(rawstr)) {
        if (Data::Output::clearAll()) {
          Langu::ageMessage::printResponse(SENTENCE_RESET_OUTPUT_ALL);
        }
        else Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_ALL);
      }
      // CLIPBOARD COPY OUTPUT
      else if (Control::copyOutputTest(rawstr)) {
        Clipboard::copyText(Data::Output::concat(this));
      }
      // CLIPBOARD PASTE INPUT
      else if (Control::pasteInputTest(rawstr)) {
        if (editing) clipboardInputPaste();
        else Langu::ageMessage::printResponse(SENTENCE_FORBIDDEN_HIDDEN_PASTE);
      }
      // BACK TO PARENT
      else if (Control::parentBackTest(rawstr)) {
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
      // EXIT PROGRAM
      else if (Control::programQuitTest(rawstr)) {
        break;
      }
      // WILD VALUE
      else {
        // push argument to 'Data::Input'
        if (editing) {
          pushInputUnormap(rawstr);
        }
        // selection (match in dialog)
        else {
          Command *lastNode = goDown(rawstr);
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
        getChildren()->forEach(
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

  COMMAND_CALLBACK_CODE Command::forEachInOutCallbacks(
    const std::function<bool(Command*)> &asWhatCallback
  ) {
    COMMAND_CALLBACK_CODE callbackCode;
    bool anyFailed = false, anyCanceled = false;

    if (hasChildren()) {
      getChildren()->forEach(
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
      COMMAND_CALLBACK_CODE inputCallbackCode = forEachInOutCallbacks(
        [](Command *current)->bool { return current->asInput; }
      );

      // process
      COMMAND_CALLBACK_CODE processCallbackCode = COMMAND_CALLBACK_SUCCEEDED;
      if (callback) processCallbackCode = callback(this);

      // output
      COMMAND_CALLBACK_CODE outputCallbackCode = forEachInOutCallbacks(
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

      if (mt_uti::StrTool::isWhitespace(ch)) {
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
    mt_uti::VecTool<std::string>::concatCopy(
      Command::raws, additionalRaws
    );

    // find first child by keyword possibility at back of string vector
    Command *firstSelected = nullptr;

    if (getChildren()) {
      resetPointer();

      getChildren()->forEach(
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
      mt_uti::VecTool<std::string>::eraseIntervalStable(
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
    forEach(
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
        mt_uti::StrTool::copyStringToUppercase(keyword) + '\n',
        Console::messageColors[CONSOLE_HINT_1]
      );

      // description
      Console::logItalicString(
        description + "\n\n",
        Console::messageColors[CONSOLE_HINT_2]
      );
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
      getChildren()->forEach(
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

    forEach(
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

  bool Command::resetInputUnormapDescendants() {
    bool resetSucceeded = resetInputUnormap();

    if (getChildren()) {
      getChildren()->traverse(
        mt_ds::LinkedList::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {
          static_cast<Command*>(node)->resetInputUnormap();
          return true;
        }
      );
    }

    return resetSucceeded;
  }

  /** Belows are declared in 'data.hpp' */

  void Data::Input::print(Command *node) {
    if (node && !node->printInput()) {
      Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
    }
  }

  void Data::Input::printAll() {
    Data::printType<std::string>(
      Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_INPUT_WORD), words
    );

    Data::printType<mt::LD>(
      Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_INPUT_NUMBER), numbers
    );

    Data::printType<bool>(
      Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_INPUT_BOOLEAN), booleans
    );
  }

  void Data::Output::print(Command *node) {
    if (Output::numberOf(node)) {
      Data::printVector<std::string>(texts[node], false);
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
  }

  void Data::Output::printAll() {
    Data::printType<std::string>(
      Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_OUTPUT_TEXT), texts
    );
  }
}

#endif // __CLI_MENU__COMMAND_CPP__