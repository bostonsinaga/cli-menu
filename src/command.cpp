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

    // strings that are not detected as keyword will become this arguments
    mt::VEC_STR argvraws;

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

        // push arguments to 'Data::unormap::comvec'
        if (!argvraws.empty()) strargv(argvraws);
        Command::raws.pop_back();

        // pseudo-child callbacks and program ends at initial match
        if (firstChild && firstChild->pseudo) {
          firstChild->triggerCallbacks();
          return setStatus(COMMAND_PSEUDO);
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
            {keyword}
          );

          Command::interruptionDialogued = true;
          return dialog();
        }
        else { // go to other node

          // the required boolean automatically has value 'true'
          if (required.first && stringifiedTypeIndex == STRINGIFIED_TYPE_INPUT_BOOLEAN) {
            strargv({"1"});
          }

          if (firstChild) return firstChild->match();
          return firstNeighbor->match();
        }
      }
      else { // push to arguments buffer
        argvraws.push_back(Command::raws.back());
        Command::raws.pop_back();
      }
    }

    // push arguments to 'Data::unormap::comvec'
    if (!argvraws.empty()) strargv(argvraws);

    // uncompleted required this
    if (required.first) {

      Langu::ageMessage::printTemplateResponse(
        SENTENCE_PARAMETER_REQUIRED,
        {keyword}
      );

      if (dialogued) return dialog();
      return setStatus(COMMAND_ERROR);
    }

    Command *firstRequiredNeighbor = strictParentHasRequired();

    // uncompleted required neighbors with strict parent
    if (firstRequiredNeighbor) {
      if (dialogued) {
        if (Command::onceDialogued) remember();
        return firstRequiredNeighbor->dialog();
      }
      else return setStatus(COMMAND_ERROR);
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
          {firstRequiredChild->keyword}
        );

        if (dialogued) {
          if (Command::onceDialogued) remember();
          return firstRequiredChild->dialog();
        }
        else return setStatus(COMMAND_ERROR);
      }
    }

    // completed required neighbors / non-strict parent
    return igniteCallbacks();
  }

  Command *Command::dialog() {
    Command::onceDialogued = true;
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
        Control::printAbbreviations(true, IndentBranched());
        Control::printBooleanAvailableValues(true, IndentBranched());
      }
      // LIST CHILDREN
      else if (Control::childrenListTest(rawstr)) {
        printList(CONSOLE_HINT_2, IndentSticked(), true);
      }
      // CLEAR TERMINAL SCREEN
      else if (Control::clearScreenTest(rawstr)) {
        Console::clearScreen();
        Console::logStylishHeader(seqNames, editing);
      }
      // NEXT NEIGHBOR
      else if (Control::neighborNextTest(rawstr)) {
        Command *lastCom = goToNeighbor(RIGHT);
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
      }
      // PREVIOUS NEIGHBOR
      else if (Control::neighborPreviousTest(rawstr)) {
        Command *lastCom = goToNeighbor(LEFT);
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
      }
      // ENTER CHILDREN
      else if (Control::childrenEnterTest(rawstr)) {        
        Command *lastCom = enter();
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
      }
      // EXECUTE CALLBACKS
      else if (Control::childrenExecuteTest(rawstr)) {        
        Command *lastCom = execute();
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
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
      // VIEW THIS INPUT
      else if (Control::viewInputThisTest(rawstr)) {
        printInput(true);
      }
      // VIEW CHILDREN INPUTS
      else if (Control::viewInputChildrenTest(rawstr)) {
        printChildrenInputs();
      }
      // VIEW THIS OUTPUT
      else if (Control::viewOutputThisTest(rawstr)) {
        printOutput(true);
      }
      // VIEW CHILDREN OUTPUTS
      else if (Control::viewOutputChildrenTest(rawstr)) {
        printChildrenOutputs();
      }
      // RESET THIS INPUT
      else if (Control::resetInputThisTest(rawstr)) {
        resetInput(true);
      }
      // RESET DESCENDANT INPUTS
      else if (Control::resetInputDescendantsTest(rawstr)) {
        resetDescendantInputs();
      }
      // RESET THIS OUTPUT
      else if (Control::resetOutputThisTest(rawstr)) {
        resetOutput(true);
      }
      // RESET DESCENDANT OUTPUTS
      else if (Control::resetOutputDescendantsTest(rawstr)) {
        resetDescendantOutputs();
      }
      // RESET THIS DATA
      else if (Control::resetDataThisTest(rawstr)) {
        resetInput(true);
        resetOutput(true);
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
      // NODE LEVEL UNDO
      else if (Control::nodeLevelUndoTest(rawstr)) {
        Command *lastCom = unredo(UNREDO_BACKWARD);
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
      }
      // NODE LEVEL REDO
      else if (Control::nodeLevelRedoTest(rawstr)) {
        Command *lastCom = unredo(UNREDO_FORWARD);
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
      }
      // BACK TO PARENT
      else if (Control::parentBackTest(rawstr)) {
        Command *lastCom = backTo(getParent());
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
      }
      // BACK TO ROOT
      else if (Control::rootBackTest(rawstr)) {
        mt_ds::GeneralTree *root = getRoot();
        Command *lastCom = backTo(root == this ? nullptr : root);
        if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
      }
      // EXIT PROGRAM
      else if (Control::programQuitTest(rawstr)) {
        break;
      }
      // WILD VALUE
      else {
        if (editing) strargv({rawstr});
        else { // selection (match in dialog)
          Command *lastCom = goDown(rawstr);
          if (lastCom->statusCode != COMMAND_ONGOING) return lastCom;
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
      else {
        remember();
        return static_cast<Command*>(topCommand)->dialog();
      }
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
      remember();

      // find ortho child
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
          {firstRequiredChild->keyword}
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

  Command *Command::setStatus(mt::CR<COMMAND_CODE> code) {
    statusCode = code;
    return this;
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
    mt::VEC_STR additionalRaws[2] = {{""}, {}};
    Command::phaseCode = COMMAND_PHASE_MATCH_IN_DIALOG;

    // split 'raw' into the string vector using spaces as delimiters
    for (mt::CR_CH ch : raw) {

      if (mt_uti::StrTool::isWhitespace(ch)) {
        if (!spaceFound) {
          spaceFound = true;
          additionalRaws[0].push_back("");
        }
      }
      else {
        spaceFound = false;
        additionalRaws[0].back() += ch;
      }
    }

    // clean 'additionalRaws' from spaces or empty string
    for (mt::CR_STR str : additionalRaws[0]) {
      if (!mt_uti::StrTool::isWhitespaces(str) && str != "") {
        additionalRaws[1].push_back(str);
      }
    }

    // user presses enter without any effect
    if (additionalRaws[1].empty()) {
      return setStatus(COMMAND_ONGOING);
    }

    // reverse additional raws order
    std::reverse(additionalRaws[1].begin(), additionalRaws[1].end());

    // insert to back of main raws
    mt_uti::VecTool<std::string>::concatCopy(
      Command::raws, additionalRaws[1]
    );

    // find first child by keyword possibility at back of string vector
    if (getChildren()) {
      Command *firstSelected = static_cast<Command*>(getChildren())
      ->findEach([](Command *current)->bool {
        return current->testHyphens(Command::raws.back());
      });

      // match in dialog
      if (firstSelected) {

        // pop back detected keyword
        Command::raws.pop_back();
        additionalRaws[1].pop_back();

        // still on the current node
        if (firstSelected->pseudo) {
          firstSelected->igniteCallbacks();
        }
        // move to child
        else if (!strictParentHasRequired()) {
          Command *lastCom = firstSelected->match();

          /**
          * Exclude the return of 'COMMAND_PSEUDO' to prevent the program
          * from terminating when selecting a node followed by its pseudo-child.
          */
          if (lastCom->statusCode != COMMAND_ONGOING &&
            lastCom->statusCode != COMMAND_PSEUDO
          ) {
            remember();
            return lastCom;
          }
        }
      }
      else {
        // child not found
        if (hasChildren()) {
          Langu::ageMessage::printTemplateResponse(
            SENTENCE_KEYWORD_NOT_FOUND,
            {Command::raws.back()}
          );
        }
        else { // this is a leaf
          Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);
        }
      }
    }
    // absolutely no children
    else Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);

    // remove the recently added strings
    if (!additionalRaws[1].empty()) {
      int rawsSizeDiff = additionalRaws[1].size() - Command::raws.size();

      mt_uti::VecTool<std::string>::eraseIntervalStable(
        Command::raws,
        {
          Command::raws.size() - additionalRaws[1].size() + rawsSizeDiff,
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
      else {
        remember();
        return static_cast<Command*>(firstOrthoNeighbor)->dialog();
      }
    }
    // has no neighbors
    else Langu::ageMessage::printResponse(SENTENCE_PARAMETER_ALONE);

    return setStatus(COMMAND_ONGOING);
  }

  Command *Command::unredo(mt::CR<UNREDO_DIRECTION> dir) {

    // pop the top one and add this in the opposite direction
    if (!Command::unredos[dir].empty()) {
      Command *top = Command::unredos[dir].top();
      Command::unredos[dir].pop();
      Command::unredos[!dir].push(this);
      return top->dialog();
    }

    Langu::ageMessage::printResponse(SENTENCE_COMMAND_UNREDO_NO_NEW_VISITS);
    return setStatus(COMMAND_ONGOING);
  }

  void Command::printHelp() {
    printKeyword(CONSOLE_HINT_1, IndentSticked());

    // description
    Console::logItalicString(
      description + '\n',
      Console::messageColors[CONSOLE_HINT_2]
    );

    printList(CONSOLE_HINT_3, IndentBranched(), false);
  }

  void Command::printKeyword(
    mt::CR<CONSOLE_CODE> consoleCode,
    CR_Indent indent
  ) {
    Console::logString(
      indent.get() + keyword + " ["
      + Langu::ageParameter::getStringifiedType(stringifiedTypeIndex)
      + ']' + (required.first ? '*' : '\0') + '\n',
      Console::messageColors[consoleCode]
    );
  }

  void Command::printList(
    mt::CR<CONSOLE_CODE> consoleCode,
    CR_Indent indent,
    mt::CR_BOL displayAtLeafWarning
  ) {
    if (hasChildren()) {

      // print children keyword
      getChildren()->head()->forEach([&](mt_ds::LinkedList *current)->bool {

        if (!static_cast<Command*>(current)->pseudo) {
          static_cast<Command*>(current)->printKeyword(consoleCode, indent);
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

  void Command::makeSterilized(mt::CR_BOL becomeLeaf) {

    // delete all descendants
    if (becomeLeaf && getChildren()) {
      getChildren()->annihilate();
    }

    sterilized = true;
  }
}

#endif // __CLI_MENU__COMMAND_CPP__