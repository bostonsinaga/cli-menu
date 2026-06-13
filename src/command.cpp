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

    if (dialogued) printWelcome();

    /**
     * The string vector will 'pop_back()'
     * until it is empty to stop this loop.
     */
    while (!Command::raws.empty()) {

      // find first child of the same keyword with 'raws.back()'
      if (getChildren()) {
        firstChild = static_cast<Command*>(getChildren())
        ->findEach([](Command *command)->bool {
          return command->testHyphens(Command::raws.back());
        });
      }

      // find first neighbor of the same keyword with 'raws.back()'
      if (!firstChild) {
        firstNeighbor = findEach([&](Command *command)->bool {
          return command != this && command->testHyphens(Command::raws.back());
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
          std::cout << std::endl;

          Langu::ageMessage::printTemplateResponse(
            SENTENCE_PARAMETER_REQUIRED,
            keyword
          );

          Command::interruptionDialogued = true;
          return dialog();
        }
        else { // go to other command

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

    // uncompleted required this
    if (required.first) {
      std::cout << std::endl;

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
      std::cout << std::endl;

      if (dialogued) {
        return firstRequiredNeighbor->dialog();
      }

      return setStatus(COMMAND_ERROR);
    }
    // parent may not be strict, but at least one required child must be completed
    else if (hasChildren()) {

      // find first required child
      Command *firstRequiredChild = static_cast<Command*>(getChildren()->head())
      ->findEach([](Command *command)->bool {
        return command->required.first;
      });

      // go to first required child
      if (firstRequiredChild) {
        std::cout << std::endl;

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
        printList(CONSOLE_HINT_2, 0, true);
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
        // cannot repeat
        if (editing) {
          Langu::ageMessage::printResponse(SENTENCE_MODE_ALREADY_EDITING);
        }
        else { // switch to editing mode
          setMode(false);
          return dialog();
        }
      }
      // SELECT COMMAND
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
      // VIEW THIS INPUT
      else if (Control::viewInputThisTest(rawstr)) {
        Data::Input::print(this);
      }
      // VIEW ALL INPUTS
      else if (Control::viewInputAllTest(rawstr)) {
        Data::Input::printAll();
      }
      // VIEW THIS OUTPUT
      else if (Control::viewOutputThisTest(rawstr)) {
        Data::Output::print(this);
      }
      // VIEW ALL OUTPUTS
      else if (Control::viewOutputAllTest(rawstr)) {
        Data::Output::printAll();
      }
      // RESET THIS & DESCENDANT INPUTS
      else if (Control::resetInputThisTest(rawstr)) {
        resetDescendants(
          [](Command *command)->bool { return command->resetInputUnormap(); },
          {
            SENTENCE_RESET_INPUT_THIS,
            SENTENCE_RESET_INPUT_DESCENDANTS,
            SENTENCE_EMPTY_INPUT_THIS
          }
        );
      }
      // RESET ALL INPUTS
      else if (Control::resetInputAllTest(rawstr)) {
        if (Data::Input::clearAll()) {
          Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_ALL);
        }
        else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_ALL);
      }
      // RESET THIS & DESCENDANT OUTPUTS
      else if (Control::resetOutputThisTest(rawstr)) {
        resetDescendants(
          [](Command *command)->bool { return command->resetOutput(); },
          {
            SENTENCE_RESET_OUTPUT_THIS,
            SENTENCE_RESET_OUTPUT_DESCENDANTS,
            SENTENCE_EMPTY_OUTPUT_THIS
          }
        );
      }
      // RESET ALL OUTPUTS
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
        // push argument to 'Data::Input'
        if (editing) {
          pushInputUnormap(rawstr);
        }
        // selection (match in dialog)
        else {
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
      ->findEach([](Command *command)->bool {
        return !command->pseudo;
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
      static_cast<Command*>(getChildren()->head())->findEach([](Command *command)->bool {
        return command->required.first;
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
    mt::CR<CONDITION_CALLBACK> asWhatCallback
  ) {
    COMMAND_CALLBACK_CODE callbackCode;
    bool anyError = false, anyCanceled = false;

    if (hasChildren()) {
      getChildren()->forEach(
        [&](mt_ds::LinkedList *node)->bool {

          if (asWhatCallback(static_cast<Command*>(node))) {
            callbackCode = static_cast<Command*>(node)->callback(static_cast<Command*>(node));

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
        [](Command *command)->bool { return command->asInput; }
      );

      // process
      COMMAND_CALLBACK_CODE processCallbackCode = COMMAND_CALLBACK_DONE;
      if (callback) processCallbackCode = callback(this);

      // output
      COMMAND_CALLBACK_CODE outputCallbackCode = forEachInOutCallbacks(
        [](Command *command)->bool { return command->asOutput; }
      );

      if (outputCallbackCode != COMMAND_CALLBACK_DONE) {
        return outputCallbackCode;
      }
      else if (processCallbackCode != COMMAND_CALLBACK_DONE) {
        return processCallbackCode;
      }
      else if (inputCallbackCode != COMMAND_CALLBACK_DONE) {
        return inputCallbackCode;
      }
    }

    return COMMAND_CALLBACK_DONE;
  }

  Command *Command::igniteCallbacks() {
    if (propagation) {
      COMMAND_CODE propagatingCode;
      COMMAND_CALLBACK_CODE callbackCode;

      bubble([&](mt_ds::LinkedList *node)->bool {
        callbackCode = static_cast<Command*>(node)->triggerCallbacks();

        if (callbackCode == COMMAND_CALLBACK_ERROR) {
          propagatingCode = COMMAND_ERROR;
          return false;
        }
        else if (callbackCode == COMMAND_CALLBACK_CANCELED) {
          propagatingCode = COMMAND_CANCELED;
          return false;
        }
        else propagatingCode = COMMAND_DONE;

        return static_cast<Command*>(node)->propagation;
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
    Command *firstSelected = getChildren() ? static_cast<Command*>(getChildren())
    ->findEach([](Command *command)->bool {
      return command->testHyphens(Command::raws.back());
    }) : nullptr;

    // match in dialog
    if (firstSelected) {
      // still on the current command
      if (firstSelected->pseudo) {
        firstSelected->igniteCallbacks();
      }
      // move to child
      else if (!strictParentHasRequired()) {
        Command *lastCom = firstSelected->match();

        /**
         * Exclude the return of 'COMMAND_PSEUDO_SILENT' to prevent the program
         * from terminating when selecting a command followed by its pseudo-child.
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

  Command *Command::goToNeighbor(mt::CR<DIRECTION> direction) {

    // find first ortho neighbor
    Command *firstOrthoNeighbor = findEach([&](Command *command)->bool {
      return command != this && !command->pseudo;
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

  void Command::printWelcome() {
    static bool displayed = false;

    if (!displayed) {
      Command *root = static_cast<Command*>(getRoot());
      displayed = true;
      std::cout << std::endl;

      // keyword
      Console::logString(
        Langu::ageMessage::getWelcomeToString() +
        mt_uti::StrTool::copyStringToUppercase(root->keyword) + '\n',
        Console::messageColors[CONSOLE_HINT_1]
      );

      // description
      Console::logItalicString(
        root->description + '\n',
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
    mt::CR<CONSOLE_CODE> consoleCode,
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
    mt::CR<CONSOLE_CODE> consoleCode,
    mt::CR_SZ numberOfIndents,
    mt::CR_BOL displayAtLeafWarning
  ) {
    if (hasChildren()) {

      // print children keyword
      getChildren()->head()->forEach([&](mt_ds::LinkedList *node)->bool {

        if (!static_cast<Command*>(node)->pseudo) {
          static_cast<Command*>(node)->printKeyword(consoleCode, numberOfIndents);
        }

        return true;
      });
    }
    // no children warning
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

  Command *Command::findEach(
    mt::CR<CONDITION_CALLBACK> condition,
    mt::CR<DIRECTION> direction
  ) {
    Command *found = nullptr;

    forEach([&](mt_ds::LinkedList *node)->bool {

      if (condition(static_cast<Command*>(node))) {
        found = static_cast<Command*>(node);
        return false;
      }

      return true;
    }, direction);

    return found;
  }

  Command *Command::strictParentHasRequired() {

    Command *found = findEach([&](Command *command)->bool {
      return command->required.first && !command->pseudo;
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

  void Command::resetDescendants(
    mt::CR<CONDITION_CALLBACK> resetMethod,
    mt::CR_ARR<SENTENCE_CODE, 3> sentenceCodes
  ) {
    bool hasResetThis = resetMethod(this);
    bool hasResetDescendants = false;

    // reset down to the leaves
    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *node)->bool {
          hasResetDescendants = resetMethod(static_cast<Command*>(node));
          return true;
        }
      );
    }

    // print messages
    enum { _sentence_this, _sentence_descendants, _sentence_empty };

    if (hasResetThis) {
      Langu::ageMessage::printResponse(sentenceCodes[_sentence_this]);
    }
    else if (hasResetDescendants) {
      Langu::ageMessage::printResponse(sentenceCodes[_sentence_descendants]);
    }
    else Langu::ageMessage::printResponse(sentenceCodes[_sentence_empty]);
  }

  bool Command::resetOutput() {

    if (Data::Output::has(this)) {
      Data::Output::erase(this);
      return true;
    }

    return false;
  }

  /** Belows are declared in 'data.hpp' */

  void Data::Input::print(Command *command) {
    if (command && !command->printInput()) {
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

  void Data::Output::print(Command *command) {
    if (Output::numberOf(command)) {
      Data::printVector<std::string>(texts[command], false);
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