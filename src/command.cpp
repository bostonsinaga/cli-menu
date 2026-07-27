#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.hpp"

namespace cli_menu {

  CODE_CALLBACK Command::defaultCallback = [](Command *self)
  ->COMMAND_CALLBACK_CODE {
    return COMMAND_CALLBACK_DONE;
  };

  Command::Command(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    mt::CR<CODE_CALLBACK> callback_in
  ) {
    keyword = keyword_in;
    description = description_in;
    if (callback_in) callback = callback_in;
    Data::registerTexts(this);
  }

  void Command::registerAsInput() {
    if (getParent()) {
      asInput = true;
      asOutput = false;
    }
  }

  void Command::registerAsOutput() {
    if (getParent()) {
      asInput = false;
      asOutput = true;
    }
  }

  std::string Command::generateSequentialRootNames() {
    std::string sequentialNames;

    bubble([&](mt_ds::LinkedList *current)->bool {

      sequentialNames = (static_cast<Command*>(current)->getParent() ?
        static_cast<Command*>(current)->hyphens : ""
      ) + static_cast<Command*>(current)->keyword + ' ' + sequentialNames;

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
        firstChild = static_cast<Command*>(getChildren())
        ->findEach([](Command *current)->bool {
          return current->testHyphens(Command::raws.back());
        });
      }

      // find first neighbor of the same keyword with 'raws.back()'
      if (!firstChild) {
        firstNeighbor = findEach([&](Command *current)->bool {
          return current != this && current->testHyphens(Command::raws.back());
        });
      }

      // keyword is detected
      if (firstChild || firstNeighbor) {
        Command::raws.pop_back();

        // pseudo-child callbacks and program ends at initial match
        if (firstChild && firstChild->pseudo) {
          firstChild->triggerCallbacks();
          return setStatus(COMMAND_PSEUDO_SILENT);
        }
        /**
         * The match will be paused until arguments are given from the dialog.
         * The 'Command::raws' is not 'pop_back()'.
         */
        else if (
          required.first && dialogued &&
          stringifiedTypeIndex != STRINGIFIED_TYPE_INPUT_BOOLEAN
        ) {
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
            strargv("1");
          }

          if (firstChild) return firstChild->match();
          return firstNeighbor->match();
        }
      }
      else { // push argument to 'Data' unordered map vector
        strargv(Command::raws.back());
        Command::raws.pop_back();
      }
    }

    // uncompleted required this
    if (required.first) {

      Langu::ageMessage::printTemplateResponse(
        SENTENCE_PARAMETER_REQUIRED,
        keyword
      );

      if (dialogued) return dialog();
      return setStatus(COMMAND_ERROR);
    }

    Command *firstRequiredNeighbor = strictParentHasRequired();

    // uncompleted required neighbors with strict parent
    if (firstRequiredNeighbor) {

      if (dialogued) {
        return firstRequiredNeighbor->dialog();
      }

      return setStatus(COMMAND_ERROR);
    }
    // parent may not be strict, but at least one required child must be completed
    else if (hasChildren()) {

      // find first required child
      Command *firstRequiredChild = static_cast<Command*>(getChildren()->head())
      ->findEach([](Command *current)->bool {
        return current->required.first;
      });

      // go to first required child
      if (firstRequiredChild) {

        Langu::ageMessage::printTemplateResponse(
          SENTENCE_PARAMETER_REQUIRED,
          firstRequiredChild->keyword
        );

        if (dialogued) return firstRequiredChild->dialog();
        return setStatus(COMMAND_ERROR);
      }
    }

    // completed required neighbors / non-strict parent
    return igniteCallbacks();
  }

  const COMMAND_CODE Command::getStatusCode() const {
    return statusCodes[totalCommandCodes];
  }

  void Command::silentStatus(mt::CR_VEC<COMMAND_CODE> onlyCodes) {
    // silent all
    if (onlyCodes.empty()) {
      statusCodes[COMMAND_ERROR] = COMMAND_ONGOING;
      statusCodes[COMMAND_DONE] = COMMAND_ONGOING;
      statusCodes[COMMAND_CANCELED] = COMMAND_ONGOING;
      statusCodes[COMMAND_TERMINATED] = COMMAND_TERMINATED_SILENT;
    }
    // silent some
    else for (mt::CR<COMMAND_CODE> code : onlyCodes) {
      if (code == COMMAND_ERROR ||
        code == COMMAND_DONE ||
        code == COMMAND_CANCELED
      ) {
        statusCodes[code] = COMMAND_ONGOING;
      }
      else if (code == COMMAND_TERMINATED) {
        statusCodes[COMMAND_TERMINATED] = COMMAND_TERMINATED_SILENT;
      }
    }
  }

  Command *Command::setStatus(mt::CR<COMMAND_CODE> code) {
    statusCodes[totalCommandCodes] = statusCodes[code];
    return this;
  }

  Command *Command::dialog() {
    Command::phaseCode = COMMAND_PHASE_DIALOG;
    std::string rawstr, seqNames = generateSequentialRootNames();

    // outline or fill style    
    Console::logStylishHeader(seqNames, editing);

    while (Control::cinDialogInput(rawstr, editing)) {
      // COMMAND HELP
      if (Control::commandHelpTest(rawstr)) {
        printHelp();
      }
      // CONTROLLER LIST
      else if (Control::controllerListTest(rawstr)) {
        Control::printAbbreviations(true, Console::indents[1]);
        Control::printBooleanAvailableValues(true, Console::indents[1]);
      }
      // ENTER CHILDREN
      else if (Control::childrenEnterTest(rawstr)) {        
        Command *lastCom = enter();
        if (lastCom->getStatusCode() != COMMAND_ONGOING) return lastCom;
      }
      // EXECUTE CALLBACKS
      else if (Control::childrenExecuteTest(rawstr)) {        
        Command *lastCom = execute();
        if (lastCom->getStatusCode() != COMMAND_ONGOING) return lastCom;
      }
      // LIST CHILDREN
      else if (Control::childrenListTest(rawstr)) {
        printList(CONSOLE_HINT_2, Console::indents[0], true);
      }
      // NEXT NEIGHBOR
      else if (Control::neighborNextTest(rawstr)) {
        Command *lastCom = goToNeighbor(RIGHT);
        if (lastCom->getStatusCode() != COMMAND_ONGOING) return lastCom;
      }
      // PREVIOUS NEIGHBOR
      else if (Control::neighborPreviousTest(rawstr)) {
        Command *lastCom = goToNeighbor(LEFT);
        if (lastCom->getStatusCode() != COMMAND_ONGOING) return lastCom;
      }
      // MODIFY INPUT
      else if (Control::switchModifyTest(rawstr)) {
        // repeating control warning
        if (editing) {
          Langu::ageMessage::printResponse(SENTENCE_MODE_ALREADY_MODIFYING);          
        }
        else { // switch to edit mode
          editing = true;
          Langu::ageMessage::printResponse(SENTENCE_MODE_SWITCH_TO_MODIFICATION);
          Console::logStylishHeader(seqNames, editing);
        }
      }
      // SELECT COMMAND
      else if (Control::switchSelectTest(rawstr)) {
        // switching mode is prohibited
        if (Command::interruptionDialogued) {
          printInterruptionDialoguedResponse();
        }
        else {
          // switch to select mode
          if (editing) {
            editing = false;
            Langu::ageMessage::printResponse(SENTENCE_MODE_SWITCH_TO_SELECTION);
            Console::logStylishHeader(seqNames, editing);
          }
          else { // repeating control warning
            Langu::ageMessage::printResponse(SENTENCE_MODE_ALREADY_SELECTING);
          }
        }
      }
      // CLEAR TERMINAL SCREEN
      else if (Control::clearScreenTest(rawstr)) {
        Console::clearScreen();
      }
      // VIEW THIS INPUT
      else if (Control::viewInputThisTest(rawstr)) {
        printInput();
      }
      // VIEW DESCENDANT INPUTS
      else if (Control::viewInputDescendantsTest(rawstr)) {
        printDescendantInputs();
      }
      // VIEW THIS OUTPUT
      else if (Control::viewOutputThisTest(rawstr)) {
        printOutput();
      }
      // VIEW DESCENDANT OUTPUTS
      else if (Control::viewOutputDescendantsTest(rawstr)) {
        printDescendantOutputs();
      }
      // RESET THIS INPUT
      else if (Control::resetInputThisTest(rawstr)) {
        resetInput();
      }
      // RESET DESCENDANT INPUTS
      else if (Control::resetInputDescendantsTest(rawstr)) {
        resetDescendantInputs();
      }
      // RESET THIS OUTPUT
      else if (Control::resetOutputThisTest(rawstr)) {
        resetOutput();
      }
      // RESET DESCENDANT OUTPUTS
      else if (Control::resetOutputDescendantsTest(rawstr)) {
        resetDescendantOutputs();
      }
      // RESET THIS DATA
      else if (Control::resetDataThisTest(rawstr)) {
        resetInput();
        resetOutput();
      }
      // RESET DESCENDANT DATA
      else if (Control::resetDataDescendantsTest(rawstr)) {
        resetDescendantInputs();
        resetDescendantOutputs();
      }
      // CLIPBOARD COPY OUTPUT
      else if (Control::copyOutputTest(rawstr)) {
        clipboardOutputCopy();
      }
      // CLIPBOARD PASTE INPUT
      else if (Control::pasteInputTest(rawstr)) {
        if (editing) clipboardInputPaste();
        else Langu::ageMessage::printResponse(SENTENCE_FORBIDDEN_HIDDEN_PASTE);
      }
      // BACK TO PARENT
      else if (Control::parentBackTest(rawstr)) {
        Command *lastCom = backTo(getParent());
        if (lastCom->getStatusCode() != COMMAND_ONGOING) return lastCom;
      }
      // BACK TO ROOT
      else if (Control::rootBackTest(rawstr)) {
        mt_ds::GeneralTree *root = getRoot();
        Command *lastCom = backTo(root == this ? nullptr : root);
        if (lastCom->getStatusCode() != COMMAND_ONGOING) return lastCom;
      }
      // EXIT PROGRAM
      else if (Control::programQuitTest(rawstr)) {
        break;
      }
      // WILD VALUE
      else {
        if (editing) strargv(rawstr);
        else { // selection (match in dialog)
          Command *lastCom = goDown(rawstr);
          if (lastCom->getStatusCode() != COMMAND_ONGOING) return lastCom;
        }
      }
    }

    return setStatus(COMMAND_TERMINATED);
  }

  Command *Command::backTo(mt_ds::GeneralTree *topCommand) {
    if (topCommand) {
      // moving is prohibited
      if (Command::interruptionDialogued) {
        printInterruptionDialoguedResponse();
      }
      // go back to parent
      else return static_cast<Command*>(topCommand)->dialog();
    }
    // this is root
    else Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_ROOT);

    return setStatus(COMMAND_ONGOING);
  }

  Command *Command::enter() {

    // continue the interrupted match
    if (Command::interruptionDialogued && !required.first) {
      Command::interruptionDialogued = false;
      return match();
    }
    // go to children level
    else if (hasChildren()) {
      return static_cast<Command*>(getChildren()->head())
      ->findEach([](Command *current)->bool {
        return !current->pseudo;
      })->dialog();
    }
    // no children warning
    else Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);

    return setStatus(COMMAND_ONGOING);
  }

  Command *Command::execute() {

    // continue the interrupted match
    if (Command::interruptionDialogued && !required.first) {
      Command::interruptionDialogued = false;
      return match();
    }
    // children may required
    else if (!strictParentHasRequired()) {

      Command *firstRequiredChild = hasChildren() ?
      static_cast<Command*>(getChildren()->head())
      ->findEach([](Command *current)->bool {
        return current->required.first;
      }) : nullptr;

      // cannot skip the required children
      if (firstRequiredChild) {

        Langu::ageMessage::printTemplateResponse(
          SENTENCE_PARAMETER_REQUIRED,
          firstRequiredChild->keyword
        );
      }
      else return igniteCallbacks();
    }

    return setStatus(COMMAND_ONGOING);
  }

  COMMAND_CALLBACK_CODE Command::forEachInOutCallbacks(
    mt::CR<BOOL_CALLBACK> asWhatCallback
  ) {
    COMMAND_CALLBACK_CODE callbackCode;
    bool anyError = false, anyCanceled = false;

    if (hasChildren()) {
      getChildren()->forEach(
        [&](mt_ds::LinkedList *current)->bool {

          if (asWhatCallback(static_cast<Command*>(current))) {
            callbackCode = static_cast<Command*>(current)->callback(static_cast<Command*>(current));

            if (callbackCode == COMMAND_CALLBACK_ERROR) {
              anyError = true;
            }
            else if (callbackCode == COMMAND_CALLBACK_CANCELED) {
              anyCanceled = true;
            }
          }

          return true;
        }
      );
    }

    if (anyError) return COMMAND_CALLBACK_ERROR;
    else if (anyCanceled) return COMMAND_CALLBACK_CANCELED;
    return COMMAND_CALLBACK_DONE;
  }

  COMMAND_CALLBACK_CODE Command::triggerCallbacks() {
    if (!asInput && !asOutput) {
      // input
      COMMAND_CALLBACK_CODE inputCallbackCode = forEachInOutCallbacks(
        [](Command *current)->bool { return current->asInput; }
      );

      // process
      COMMAND_CALLBACK_CODE processCallbackCode = COMMAND_CALLBACK_DONE;
      if (callback) processCallbackCode = callback(this);

      // output
      COMMAND_CALLBACK_CODE outputCallbackCode = forEachInOutCallbacks(
        [](Command *current)->bool { return current->asOutput; }
      );

      if (inputCallbackCode != COMMAND_CALLBACK_DONE) {
        return inputCallbackCode;
      }
      else if (processCallbackCode != COMMAND_CALLBACK_DONE) {
        return processCallbackCode;
      }
      else if (outputCallbackCode != COMMAND_CALLBACK_DONE) {
        return outputCallbackCode;
      }
    }

    return COMMAND_CALLBACK_DONE;
  }

  Command *Command::igniteCallbacks() {
    if (propagation) {
      COMMAND_CODE propagatingCode;
      COMMAND_CALLBACK_CODE callbackCode;

      bubble([&](mt_ds::LinkedList *current)->bool {
        callbackCode = static_cast<Command*>(current)->triggerCallbacks();

        if (callbackCode == COMMAND_CALLBACK_ERROR) {
          propagatingCode = COMMAND_ERROR;
          return false;
        }
        else if (callbackCode == COMMAND_CALLBACK_CANCELED) {
          propagatingCode = COMMAND_CANCELED;
          return false;
        }
        else propagatingCode = COMMAND_DONE;

        return static_cast<Command*>(current)->propagation;
      });

      return setStatus(propagatingCode);
    }
    else { // not propagated
      COMMAND_CALLBACK_CODE callbackCode = triggerCallbacks();

      if (callbackCode == COMMAND_CALLBACK_ERROR) {
        return setStatus(COMMAND_ERROR);
      }
      else if (callbackCode == COMMAND_CALLBACK_CANCELED) {
        return setStatus(COMMAND_CANCELED);
      }

      return setStatus(COMMAND_DONE);
    }
  }

  // always in selection mode
  Command *Command::goDown(mt::CR_STR raw) {

    bool spaceFound = false;
    mt::VEC_STR additionalRaws = {""};
    Command::phaseCode = COMMAND_PHASE_MATCH_IN_DIALOG;

    // split 'raw' into the string vector using spaces as delimiters
    for (mt::CR_CH ch : raw) {

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
    Command *firstSelected = getChildren() ? static_cast<Command*>(getChildren())
    ->findEach([](Command *current)->bool {
      return current->testHyphens(Command::raws.back());
    }) : nullptr;

    // match in dialog
    if (firstSelected) {

      // pop back detected keyword
      Command::raws.pop_back();
      additionalRaws.pop_back();

      // still on the current node
      if (firstSelected->pseudo) {
        firstSelected->igniteCallbacks();
      }
      // move to child
      else if (!strictParentHasRequired()) {
        Command *lastCom = firstSelected->match();

        /**
         * Exclude the return of 'COMMAND_PSEUDO_SILENT' to prevent the program
         * from terminating when selecting a node followed by its pseudo-child.
         */
        if (lastCom->getStatusCode() != COMMAND_ONGOING &&
          lastCom->getStatusCode() != COMMAND_PSEUDO_SILENT
        ) return lastCom;
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
    if (!additionalRaws.empty()) {
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

  Command *Command::goToNeighbor(mt::CR<DIRECTION> direction) {

    // find first ortho neighbor
    Command *firstOrthoNeighbor = findEach([&](Command *current)->bool {
      return current != this && !current->pseudo;
    }, direction);

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

  void Command::printHelp() {
    printKeyword(CONSOLE_HINT_1, Console::indents[0]);

    // description
    Console::logItalicString(
      description + Console::getNL(),
      Console::messageColors[CONSOLE_HINT_2]
    );

    printList(CONSOLE_HINT_3, Console::indents[1], false);
  }

  void Command::printKeyword(
    mt::CR<CONSOLE_CODE> consoleCode,
    mt::CR_SZ numberOfIndents
  ) {
    Console::logString(
      std::string(numberOfIndents, ' ') + keyword + " ["
      + Langu::ageParameter::getStringifiedType(stringifiedTypeIndex)
      + ']' + (required.first ? '*' : '\0') + Console::getNL(),
      Console::messageColors[consoleCode]
    );
  }

  void Command::printList(
    mt::CR<CONSOLE_CODE> consoleCode,
    mt::CR_SZ numberOfIndents,
    mt::CR_BOL displayAtLeafWarning
  ) {
    if (hasChildren()) {

      // print children keyword
      getChildren()->head()->forEach([&](mt_ds::LinkedList *current)->bool {

        if (!static_cast<Command*>(current)->pseudo) {
          static_cast<Command*>(current)->printKeyword(consoleCode, numberOfIndents);
        }

        return true;
      });
    }
    // no children warning
    else if (displayAtLeafWarning) {
      Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);
      return;
    }
  }

  void Command::printInterruptionDialoguedResponse() {
    Langu::ageMessage::printResponse(required.first?
      SENTENCE_ARGUMENT_REQUIRED : SENTENCE_INTERRUPTION_DIALOG
    );
  }

  Command *Command::findEach(
    mt::CR<BOOL_CALLBACK> condition,
    mt::CR<DIRECTION> direction
  ) {
    Command *found = nullptr;

    forEach([&](mt_ds::LinkedList *current)->bool {

      if (condition(static_cast<Command*>(current))) {
        found = static_cast<Command*>(current);
        return false;
      }

      return true;
    }, direction);

    return found;
  }

  Command *Command::strictParentHasRequired() {

    Command *found = findEach([&](Command *current)->bool {
      return current->required.first && !current->pseudo;
    });

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

  void Command::makePseudo() {
    if (getParent()) {
      pseudo = true;
      propagation = false;
      required = { false, false };
      static_cast<Command*>(getParent())->pseudosCount++;
    }
  }

  void Command::makeRequired() {
    if (!pseudo) required = { true, true };
  }

  void Command::makeSterilized(mt::CR_BOL becomeLeaf) {

    // delete all descendants
    if (becomeLeaf && getChildren()) {
      getChildren()->annihilate();
    }

    sterilized = true;
  }
}

#endif // __CLI_MENU__COMMAND_CPP__