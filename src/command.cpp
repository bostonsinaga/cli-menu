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
    processCallback = callback_in;
  }

  Command::Command(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) {
    keyword = keyword_in;
    description = description_in;
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
    Command *firstNeighbor = nullptr,
      *firstChild = nullptr;

    Command::matching = true;

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

        // pseudo-child callbacks and program end
        if (firstChild && firstChild->pseudo) {
          firstChild->triggerCallbacks();
          return COMMAND_ENDED;
        }
        /**
         * The match will be paused until arguments are given from the dialog.
         * The 'Command::raws' is not 'pop_back()'.
         */
        else if (
          required.first && Command::globalDialogued && localDialogued &&
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
    if (required.first && Command::globalDialogued && localDialogued) {
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

  COMMAND_CODE Command::dialog() {
    static bool hinted = false;
    std::string input;
    Command::matching = false;

    // outline or fill style
    Console::logStylishHeader(
      generateSequentialRootNames(),
      editing
    );

    // hints for controllers
    if (!hinted) {
      hinted = true;
      Control::printAbbreviations();
      Control::printBooleanAvailableValues();
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
        COMMAND_CODE code = enter();
        if (code != COMMAND_ONGOING) return code;
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
        COMMAND_CODE code = goToNeighbor(mt_ds::GeneralTree::RIGHT);
        if (code != COMMAND_ONGOING) return code;
      }
      // PREVIOUS
      else if (Control::previousTest(input)) {
        COMMAND_CODE code = goToNeighbor(mt_ds::GeneralTree::LEFT);
        if (code != COMMAND_ONGOING) return code;
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
          COMMAND_CODE code = goDown(input);
          if (code != COMMAND_ONGOING) return code;
        }
      }
    }

    return COMMAND_TERMINATED;
  }

  COMMAND_CODE Command::enter() {

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
        return COMMAND_ONGOING;
      }
    }

    // no required nodes (done)
    return igniteCallbacks();
  }

  bool Command::triggerCallbacks() {
    bool anyInput = false,
      isProcess = false,
      anyOutput = false;

    for (mt::PAIR2<COMMAND_CALLBACK, Command*> &cbnode : inputCallbacks) {
      if (cbnode.first && cbnode.first(cbnode.second)) {
        anyInput = true;
      }
    }

    if ((anyInput || inputCallbacks.empty()) &&
      processCallback && processCallback(this)
    ) {
      isProcess = true;

      for (mt::PAIR2<COMMAND_CALLBACK, Command*> &cbnode : outputCallbacks) {
        if (cbnode.first && cbnode.first(cbnode.second)) {
          anyOutput = true;
        }
      }
    }

    return (isProcess || !processCallback) &&
      (anyOutput || outputCallbacks.empty());
  }

  COMMAND_CODE Command::igniteCallbacks() {

    if (Command::globalPropagation && localPropagation) {
      COMMAND_CODE propagatingCode;

      bubble([&](mt_ds::LinkedList *node)->bool {

        if (static_cast<Command*>(node)->triggerCallbacks()) {
          propagatingCode = COMMAND_SUCCEEDED;
          return true;
        }

        propagatingCode = COMMAND_FAILED;
        return false;
      });

      return propagatingCode;
    }
    else { // not propagated
      if (triggerCallbacks()) {
        return COMMAND_SUCCEEDED;
      }

      return COMMAND_FAILED;
    }
  }

  void Command::pushInputCallbacks(
    COMMAND_CALLBACK callback_in,
    Command *node
  ) {
    if (processCallback && node) {
      inputCallbacks.push_back({callback_in, node});
    }
  }

  void Command::pushOutputCallbacks(
    COMMAND_CALLBACK callback_in,
    Command *node
  ) {
    if (processCallback && node) {
      outputCallbacks.push_back({callback_in, node});
    }
  }

  void Command::popInputCallbacks() {
    if (!inputCallbacks.empty()) {
      inputCallbacks.pop_back();
    }
  }

  void Command::popOutputCallbacks() {
    if (!outputCallbacks.empty()) {
      outputCallbacks.pop_back();
    }
  }

  void Command::clearInputCallbacks() {
    inputCallbacks.clear();
  }

  void Command::clearOutputCallbacks() {
    outputCallbacks.clear();
  }

  // always in selection mode
  COMMAND_CODE Command::goDown(mt::CR_STR input) {
    bool spaceFound = false;
    mt::VEC_STR additionalRaws = {""};

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
        COMMAND_CODE code = firstSelected->match();
        if (code != COMMAND_ONGOING) return code;
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
    mt_uti::VecTools<std::string>::eraseIntervalStable(
      Command::raws,
      {
        Command::raws.size() - additionalRaws.size(),
        Command::raws.size() - 1
      }
    );

    return COMMAND_ONGOING;
  }

  COMMAND_CODE Command::goToNeighbor(
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

    return COMMAND_ONGOING;
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

  void Command::printHelp() {
    // keyword
    Console::logString(
      keyword + " ["
      + Langu::ageCommand::getStringifiedType(stringifiedTypeIndex) + "]\n",
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
    if (hasChildren()) {
      resetPointer();

      getChildren()->iterate(
        mt_ds::GeneralTree::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          if (!static_cast<Command*>(node)->pseudo) {
            Console::logString(
              (withHelp ? "  " : "") + static_cast<Command*>(node)->keyword + " ["
              + Langu::ageCommand::getStringifiedType(
                static_cast<Command*>(node)->stringifiedTypeIndex
              ) + "]\n",
              Console::messageColors[withHelp ? CONSOLE_HINT_3 : CONSOLE_HINT_2]
            );
          }

          return true;
        }
      );
    }
    // print error
    else if (!withHelp) {
      Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);
      return;
    }

    // additional newline only in dialog
    if (!Command::matching) {
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