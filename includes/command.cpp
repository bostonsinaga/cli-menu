#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {
  //______________|
  // CONSTRUCTORS |
  //______________|

  Command *Command::circularCheckpoint = nullptr;

  bool Command::usingCaseSensitiveName = true,
    Command::usingLowercaseName = false,
    Command::usingUppercaseName = false,
    Command::usingDashesBoundaryLine = false,
    Command::dialogued = false,
    Command::matching = true,
    Command::selecting = false;

  const mt::USI Command::disguiseFlags[disguiseCount] = {
    PROGRAM
  };

  const std::string Command::disguiseNames[disguiseCount][2] = {
    {"Program", "Toggle"}
  };

  Command *Command::lastCom = nullptr;
  mt::VEC_STR Command::directInputs = {};

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL accumulating_in,
    CM_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : TREE(name_in) {
    description = description_in;
    callback = callback_in;
    required = required_in;
    accumulating = accumulating_in;
    propagatingCallback = propagatingCallback_in;
    setParent(parent_in);
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL accumulating_in
  ) : TREE(name_in) {
    description = description_in;
    required = required_in;
    accumulating = accumulating_in;
    setParent(parent_in);
  }

  void Command::resetData(mt::CR_BOL discarded) {
    if (discarded) {
      ResultInputs::popName();
      resultInputsIndex = -1;
      unuseRequired();
    }
  }

  void Command::setChildren(
    CR_VEC_TREE newChildren,
    mt::CR_BOL needEmpty,
    mt::CR_BOL validating
  ) {
    if (isContainer()) {
      TREE::setChildren(
        newChildren,
        needEmpty,
        validating
      );

      for (TREE *node : newChildren) {
        relateToSupporter(node, true);
        addRequiredItems(static_cast<Cm*>(node));
      }
    }
  }

  void Command::addChild(
    TREE *node,
    mt::CR_BOL reconnected
  ) {
    if (isContainer()) {
      TREE::addChild(node, reconnected);
      relateToSupporter(node, true);
      addRequiredItems(static_cast<Cm*>(node));
    }
  }

  // the 'node[index]' is guaranteed to always be exist
  Command::TREE *Command::dismantle(mt::CR_INT index) {
    if (isParent()) {
      TREE *node = TREE::dismantle(index);
      relateToSupporter(node, false);
      reduceRequiredItems(static_cast<Cm*>(node));
      return node;
    }
    return nullptr;
  }

  Command::VEC_TREE Command::releaseChildren() {
    if (isParent()) {
      VEC_TREE released = TREE::releaseChildren();

      for (TREE *node : released) {
        relateToSupporter(node, false);
        reduceRequiredItems(static_cast<Cm*>(node));
      }

      return released;
    }

    return {};
  }

  //__________|
  // ULTIMATE |
  //__________|

  mt::UI Command::getRequiredCount() {
    if (isSupporter()) {
      return ultimate->requiredItems.size();
    }
    return requiredItems.size();
  }

  Color Command::getLevelLabelColor() {
    if (Command::usingDashesBoundaryLine) {
      return Color::MINT;
    }
    return Color(0, 159, 159);
  }

  std::string Command::getLevelLabel() {
    return Color::getString(
      "[" + mt_uti::StrTools::getStringToUppercase(getLevelName())
      + "]", getLevelLabelColor()
    );
  }

  // the 'ultimate' confirmed to exist when this invoked
  std::string Command::getFullNameWithUltimate(
    mt::CR_STR separator,
    mt::CR_BOL startWithSeparator,
    mt::CR_BOL endWithSeparator
  ) {
    return (startWithSeparator ? separator : "")
      + Color::getString(
        ultimate->name, getLevelLabelColor()
      ) + separator + getFullName()
      + (endWithSeparator ? separator : "");
  }

  std::string Command::getLevelName(mt::CR_BOL isVerbose) {
    std::string levelName;

    if (!parent) {
      if (isParent()) levelName = "program";
      else levelName = "input";
    }
    else if (isGroup()) {
      levelName = "group";
    }
    else if (isUltimate()) {
      levelName = "command";
    }
    else if (!isVerbose) {
      levelName = "input";
    }

    if (isVerbose) {
      if (isParent()) levelName += " ";

      if (getInheritanceFlag() == PARAMETER) {
        levelName += "parameter";
      }
      else levelName += "toggle";
    }

    return levelName;
  }

  std::string Command::getChildrenLevelName(mt::CR_BOL onlyRequired) {
    if (isParent()) {

      static const int initAmount = 3;
      Command *loopCom;

      int indexes[] = {0, 0, 0},
        finalAmount = initAmount;

      std::string availableNames[] = {
        "command", "group", "input"
      };

      // name selection
      for (int i = 0; i < children.size(); i++) {
        loopCom = static_cast<Cm*>(children[i]);

        if (!onlyRequired ||
          (onlyRequired && loopCom->required)
        ) {
          if (loopCom->isUltimate()) indexes[0]++;
          else if (loopCom->isGroup()) indexes[1]++;
          else indexes[2]++;
        }
      }

      // plural detection
      for (int i = 0; i < initAmount; i++) {
        if (indexes[i] > 1) {
          availableNames[i] += "s";
        }
        else if (indexes[i] == 0) {
          availableNames[i] = "";
          finalAmount--;
        }
      }

      std::string namesText;

      // return value assignment
      for (int i = 0; i < initAmount; i++) {

        if (!availableNames[i].empty()) {
          namesText += availableNames[i];

          if (i < initAmount - 1) {
            if (finalAmount == initAmount) {

              if (i == initAmount - 2) {
                namesText += ", or ";
              }
              else namesText += ", ";
            }
            else if (finalAmount == 2) {
              namesText += " or ";
            }
          }
        }
      }

      return namesText;
    }

    // called by toddler or orphan
    return "";
  }

  std::string Command::getSentenceSubject(Command *subject) {
    return "the '" + subject->name + "' " + subject->getLevelName();
  }

  // not for program or question in the middle
  mt::USI Command::tryToGoBack() {

    if (parent) {
      if (!isDirectInputsError("go back")) {
        resetData(true);
        return static_cast<Cm*>(parent)->dialog();
      }
    }
    else Message::printDialog(
      Message::ERROR_FLAG,
      "You are now at the root level. No more groups upward."
    );

    return CONTINUE_FLAG;
  }

  // for supporters
  bool Command::doesUltimateAllowEnter(mt::CR_BOL fromChild) {

    if (ultimate) {
      Command *ulCom = static_cast<Cm*>(ultimate);
      const mt::UI reqCt = ulCom->getRequiredCount();

      // all complete
      if (!reqCt) return true;
      else {
        const bool isOneLeft = reqCt == 1;

        Command *oneLeft = isOneLeft ?
          static_cast<Cm*>(ulCom->requiredItems.back()) : nullptr;

        const bool
          isThis = oneLeft == this,
          isOneLeftNotThis = !isThis && isOneLeft;

        Message::printNeatDialog(
          Message::ERROR_FLAG,
          "unable to process before "
          + std::string( isOneLeftNotThis ? "a " : "" )
          + std::string( isThis ? "this " : "required " )
          + ulCom->getChildrenLevelName(true)
          + std::string( isOneLeftNotThis ? (" named '" + oneLeft->name) : " " )
          + std::string( isOneLeftNotThis ? "' is" : (isThis ? "is" : "are") )
          + " provided"
        );

        return false;
      }
    }

    // container
    return true;
  }

  // for an only child
  void Command::printNullptrNeighborError() {
    if (parent) {
      Command* parCom = static_cast<Cm*>(parent);
    }

    Message::printNeatDialog(
      Message::ERROR_FLAG,
      "this " + getLevelName(isSupporter()) + " has no "
      + (parent || children.size() ? "neighbors" : "connections")
    );
  }

  Command *Command::chooseLastCommand(mt::CR_BOL onlyParent) {
    return ultimate ? ultimate : (
      onlyParent && parent ? static_cast<Cm*>(parent) : this
    );
  }

  void Command::addRequiredItems(Command *command) {
    if (command->required) {
      requiredItems.push_back(command);
    }
  }

  void Command::reduceRequiredItems(Command *command) {
    if (command->required) {

      mt_uti::VecTools<TREE*>::cutSingle(
        requiredItems, mt_uti::VecTools<TREE*>::getIndex(
          requiredItems, command
        )
      );
    }
  }

  void Command::registerRequiredItems(Command *command) {
    if (!command->required) {
      command->required = true;
      requiredItems.push_back(command);
    }
  }

  void Command::revokeRequiredItems(Command *command) {

    if (command->required) {
      command->required = false;

      mt_uti::VecTools<TREE*>::cutSingle(
        requiredItems, mt_uti::VecTools<TREE*>::getIndex(
          requiredItems, command
        )
      );
    }
  }

  void Command::useRequired() {
    used = true;

    if (parent) {
      static_cast<Cm*>(parent)->revokeRequiredItems(this);
    }
  }

  void Command::unuseRequired() {
    used = false;

    if (parent) {
      static_cast<Cm*>(parent)->registerRequiredItems(this);
    }
  }

  void Command::useResultInputsIndex() {
    resultInputsIndex = ResultInputs::getLastIndex();
    useRequired();
  }

  void Command::collapseUltimateItems(
    Command *newUltimate,
    VEC_TREE &united
  ) {
    VEC_TREE released = children;
    mt_uti::VecTools<TREE*>::concat(united, released);

    // avoid calling add or reduce '..RequiredItems'
    ultimate = newUltimate;
    children = {};

    for (TREE *node : released) {
      Command *com = static_cast<Cm*>(node);

      // do not call 'resign'
      com->next = nullptr;
      com->previous = nullptr;
      com->head = com;
      com->parent = nullptr;

      com->collapseUltimateItems(newUltimate, united);
    }
  }

  void Command::relateToSupporter(
    TREE *node,
    mt::CR_BOL connected
  ) {
    if (node && isUltimate()) {
      static_cast<Cm*>(node)->ultimate = connected ? this : nullptr;
    }
  }

  void Command::setAsUltimate() {
    VEC_TREE united;

    collapseUltimateItems(this, united);
    children = united;
    cleanDuplicatesInChildren();

    for (int i = 0; i < united.size(); i++) {

      if (i < united.size() - 1) {
        united[i]->connect(united[i+1]);
      }

      united[i]->setParent(this, false);
    }
  }

  void Command::resignFromUltimate() {
    if (isUltimate()) {
      for (TREE *node : children) {
        static_cast<Cm*>(node)->ultimate = nullptr;
      }

      ultimate = nullptr;
      requiredItems = {};
    }
  }

  bool Command::execute() {
    bool called = false;
    ResultInputs::title = name;

    if (callback) {
      callback();
      called = true;
    }

    if (propagatingCallback) {
      if (parent) {
        return static_cast<Cm*>(parent)->execute();
      }
      return true;
    }

    return called;
  }

  bool Command::executeTo(Command *target) {
    if (target) return target->execute();
    return false;
  }

  Command *Command::getUnusedNeighbor(Command *start) {

    if (!next || next == start) {
      return nullptr;
    }
    else if (!static_cast<Cm*>(next)->used) {
      return static_cast<Cm*>(next);
    }

    // pass 'start' to keep remember it
    return static_cast<Cm*>(next)->getUnusedNeighbor(start);
  }

  mt::USI Command::matchTo(Command *target) {
    if (target) {
      return target->match();
    }
    else if (isParent()) return FAILED_FLAG;

    // toddler pointing to its parent
    Command::lastCom = chooseLastCommand(true);

    return COMPLETED_FLAG;
  }

  //________|
  // DIALOG |
  //________|

  bool Command::isDirectInputsError(mt::CR_STR controlName) {

    static Command *curCom = nullptr;
    static std::string text = "";

    if (directInputs.empty()) {
      return false;
    }
    else if (curCom != this) {
      curCom = this;
      text = "";

      std::string strArr[5] = {
        " before ", "", "direct input", "", " processed"
      };

      // singular
      if (directInputs.size() == 1 || (
        directInputs.size() == 2 && DashTest::isSingle(directInputs.back())
      )) {
        strArr[1] = "a ";
        strArr[3] = " is";
      }
      // plural
      else strArr[3] = "s are";

      // strings are united
      for (int i = 0; i < 5; i++) {
        text += strArr[i];
      }

      // final string
      text = "unable to " + controlName + text;
    }

    Message::printNeatDialog(Message::ERROR_FLAG, text);
    return true;
  }

  mt::USI Command::pointToNeighbor(mt::CR_BOL toNext) {

    LINKED_LIST *neighbor = toNext ? next : previous;

    if (neighbor) {
      resetData(true);
      return static_cast<Cm*>(neighbor)->dialog();
    }

    printNullptrNeighborError();
    return CONTINUE_FLAG;
  }

  // has a newline at the end
  std::string Command::getDialogStatusString(
    mt::CR_BOL usingAbbreviations,
    mt::CR_BOL withBrackets,
    mt::CR_BOL forcedToSupporterVersion
  ) {
    std::string status = withBrackets ? " (" : "";
    Color fontColor;

    // dialog
    if (!(isSupporter() || questionedGroup || forcedToSupporterVersion)) {
      status += usingAbbreviations ? "sel" : "selection";
      fontColor = Color::MAGENTA;
    }
    // question
    else {
      // first
      if (getInheritanceFlag() == PARAMETER) {
        status += usingAbbreviations ? "par" : "parameter";
      }
      else status += usingAbbreviations ? "tog" : "toggle";

      status += ", ";

      // second
      if (required) {
        status += std::string(
          usingAbbreviations ? "req" : "required"
        ) + ", ";
      }
      else status += Color::getString(
        usingAbbreviations ? "opt" : "optional",
        Color::MAGENTA
      ) + ", ";

      // third
      std::string remains;

      if (usingAbbreviations) {
        if (!used) return "emp";
        else if (accumulating) remains += "acc";
        else remains += "cor";
      }
      else {
        if (!used) return "empty";
        else if (accumulating) remains += "accumulation";
        else remains += "correction";
      }

      status += Color::getString(remains, Color::MAGENTA);
      fontColor = Color::VIOLET;

      // inverted in 'question' method
      questionedGroup = false;
    }

    return Color::getItalicString(
      status + (withBrackets ? ")\n" : "\n"), fontColor
    );
  }

  mt::USI Command::conversation(mt::CR_BOL dialogOn) {
    std::string bufferStr;
    mt::USI flag;

    while (true) {
      Message::setDialogInput(bufferStr);

      if (!bufferStr.empty()) {

        // copy to secure original input
        std::string controlStr = mt_uti::StrTools::getStringToLowercase(bufferStr);

        // controller detection
        flag = answerControl(controlStr, dialogOn);

        // special handling
        if (flag == PASSED_FLAG) {
          onFreeChangeInputLetterCase(bufferStr);

          if (dialogOn) flag = Command::answerSpecial(bufferStr);
          else flag = answerSpecial(bufferStr);
        }

        // chain call ends
        if (flag != PASSED_FLAG && flag != CONTINUE_FLAG) {
          return flag;
        }
      }
    }

    return CANCELED_FLAG;
  }

  mt::USI Command::answerControl(
    mt::CR_STR controlStr,
    mt::CR_BOL dialogOn
  ) {
    mt::USI flag;

    if (Control::backTest(controlStr)) {

      // stop at the root
      flag = tryToGoBack();

      if (flag != CONTINUE_FLAG) return flag;
      return CONTINUE_FLAG;
    }
    else if (Control::clipboardTest(controlStr)) {

      // hidden paste only available for parameters
      if (dialogOn) {
        Command::clipboardAction();
      }
      else clipboardAction();

      return CONTINUE_FLAG;
    }
    else if (Control::enterTest(controlStr)) {
      bool isContinue = true;

      if (!dialogOn) { // in question

        // requires explicit value
        if (!used && required) {

          Message::printNeatDialog(
            Message::ERROR_FLAG,
            "this " + getLevelName(isSupporter()) + getNeedsString()
          );

          isContinue = false;
        }
        // custom test
        else {
          flag = questionEnterTest();
          if (flag != PASSED_FLAG) return flag;
        }
      }

      // these also in question
      if (isContinue) {

        // pointing to first child
        if (isParent()) {

          return dialogTo(
            static_cast<Cm*>(children.front())
          );
        }
        // directly completed
        else if (doesUltimateAllowEnter()) {

          initDefaultData();
          Command::lastCom = chooseLastCommand();
          return COMPLETED_FLAG;
        }
      }

      return CONTINUE_FLAG;
    }
    else if (Control::helpTest(controlStr)) {
      printHelp();
      return CONTINUE_FLAG;
    }
    else if (Control::listTest(controlStr)) {
      printList();
      return CONTINUE_FLAG;
    }
    else if (Control::modifyTest(controlStr)) {
      if (dialogOn) return question();

      Message::printNeatDialog(
        Message::ERROR_FLAG, "already in insert mode"
      );

      return CONTINUE_FLAG;
    }
    else if (
      Control::nextTest(controlStr) ||
      Control::previousTest(controlStr)
    ) {
      if (dialogOn) {
        flag = pointToNeighbor(
          Control::getSharedFlag() == Control::NEXT
        );

        if (flag != CONTINUE_FLAG) {
          return flag;
        }
      }
      // in question
      else {
        // question in the middle check
        if (!isDirectInputsError("skip")) {
          Command::lastCom = ultimate;

          // container
          if (isContainer()) {

            LINKED_LIST *neighbor = (
              Control::getSharedFlag() == Control::NEXT ?
              next : previous
            );

            if (neighbor) {
              resetData(true);
              return dialogTo(static_cast<Cm*>(neighbor));
            }

            printNullptrNeighborError();
          }
          // supporter
          else return questionTo(
            getUnusedNeighbor(this)
          );
        }
      }

      return CONTINUE_FLAG;
    }
    else if (Control::quitTest(controlStr)) {
      return CANCELED_FLAG;
    }
    else if (Control::resetTest(controlStr)) {
      resetData(true);
      return CONTINUE_FLAG;
    }
    else if (Control::selectTest(controlStr)) {
      if (dialogOn) {
        Message::printNeatDialog(
          Message::ERROR_FLAG, "already in selection mode"
        );
      }
      else { // in question
        if (!isDirectInputsError("select")) {

          if (required) {
            Message::printNeatDialog(
              Message::ERROR_FLAG,
              "unable to select before "
              + std::string(getInheritanceFlag() == TOGGLE ?
                "conditions" : "arguments"
              ) + " are given"
            );
          }
          // the 'selecting' will be an opposite in overridden 'dialog' method
          else {
            if (isSupporter()) {
              Command *parCom = static_cast<Cm*>(parent);
              return parCom->dialog();
            }

            // parent
            return Command::dialog();
          }
        }
      }

      return CONTINUE_FLAG;
    }
    else if (Control::viewTest(controlStr)) {
      viewAction();
      return CONTINUE_FLAG;
    }

    return PASSED_FLAG;
  }

  mt::USI Command::answerSpecial(mt::CR_STR bufferStr) {
    Command *found;
    bool isContinue = false;

    /** Rematch from dialog */

    if (isParent() && directInputs.empty() && (
      DashTest::isDouble(bufferStr) ||
      DashTest::isSingle(bufferStr)
    )) {
      bool recentlySpace = false;
      directInputs.push_back("");

      for (mt::CR_CH ch : bufferStr) {

        if (ch == ' ') {
          if (!recentlySpace) directInputs.push_back("");
          recentlySpace = true;
        }
        else {
          directInputs.back() += ch;
          recentlySpace = false;
        }
      }

      if (lastCom) lastCom->resetBackupData();
      std::reverse(directInputs.begin(), directInputs.end());

      Command::matching = true;
      return matchTo(static_cast<Cm*>(children[0]));
    }

    /** Find developer defined command */

    if (isParent()) {
      found = static_cast<Cm*>(getChild(bufferStr));
    }
    else {
      TREE *usedParent;

      if (ultimate) usedParent = ultimate;
      else if (parent) usedParent = parent;
      else {
        printNullptrNeighborError();
        isContinue = true;
      }

      if (!isContinue) {
        found = static_cast<Cm*>(usedParent->getChild(bufferStr));
      }
    }

    if (!isContinue) {
      if (found) {
        return found->dialog();
      }
      else if (isUltimate()) {
        Message::printNeatDialog(
          Message::ERROR_FLAG, "input not found"
        );
      }
      // toddler
      else if (isToddler()) {
        printNoItems();
      }
      // group
      else Message::printNeatDialog(
        Message::ERROR_FLAG,
        getChildrenLevelName(false) + " not found"
      );
    }

    return PASSED_FLAG;
  }

  mt::USI Command::question() {
    printQuestionBoundaryLine();
    return conversation(false);
  }

  mt::USI Command::questionTo(Command *target) {
    if (target) return target->question();
    return COMPLETED_FLAG;
  }

  mt::USI Command::dialog() {
    printAfterBoundaryLine(getInlineRootNames());

    // inverted in derived method
    Command::selecting = true;

    return conversation(true);
  }

  mt::USI Command::dialogTo(Command *target) {
    if (target) return target->dialog();
    return COMPLETED_FLAG;
  }

  mt::USI Command::askNeighbor() {
    if (!circularCheckpoint && next) {
      circularCheckpoint = this;
    }

    if (next != circularCheckpoint) return matchTo(
      static_cast<Cm*>(next)
    );

    std::string inputLevelName = "input",
      inputName = DashTest::cleanSingle(directInputs.back());

    if (!inputName.empty()) {
      inputLevelName = "parameter";
    }
    else {
      inputName = DashTest::cleanDouble(directInputs.back());

      if (!inputName.empty()) {
        inputLevelName = "toggle";
      }
    }

    Message::printNeatDialog(
      Message::ERROR_FLAG,
      "unknown " + inputLevelName + " named '" + inputName + "'", 1
    );

    // reset values
    circularCheckpoint = nullptr;
    directInputs = {};

    if (isMatchNeedDialog(false)) return dialogTo(
      static_cast<Cm*>(parent)
    );

    return FAILED_FLAG;
  }

  /**
   * This will not be used by program or orphan because there is
   * already a condition for empty children in 'Program::run'.
   */
  bool Command::isMatchNeedDialog(mt::CR_BOL withMessage) {

    Command *parCom = static_cast<Cm*>(parent);
    const mt::UI reqCt = parCom ? parCom->getRequiredCount() : 0;

    if (Command::dialogued && parCom && reqCt) {

      if (withMessage) {
        const bool isOneLeft = reqCt == 1;
        Command *oneLeft = nullptr;

        if (isOneLeft == 1) {
          oneLeft = static_cast<Cm*>(parCom->requiredItems.back());
        }

        // group or toddler
        if (!parCom->ultimate) {

          const std::string errStr = " that need"
            + std::string(isOneLeft ? "s": "")
            + " to be used";

          if (reqCt > 1) Message::printNeatDialog(
            Message::ERROR_FLAG,
            getSentenceSubject(parCom) + " has "
            + parCom->getChildrenLevelName(true)
            + errStr, 1
          );
          // one incomplete
          else Message::printNeatDialog(
            Message::ERROR_FLAG,
            getSentenceSubject(parCom) + " has "
            + "a " + oneLeft->getLevelName()
            + " named '" + oneLeft->name + "'"
            + errStr, 1
          );
        }
        // ultimate or supporter
        else {
          const std::string errStr = "The '" + parCom->name
            + "' command has " + std::string(isOneLeft ? "a " : "")
            + "required input" + std::string(
              isOneLeft ? (" named '" + oneLeft->name + "'.") : "s."
            );

          Message::printDialog(Message::ERROR_FLAG, errStr, 1);
        }
      }

      Command::matching = false;
      return true;
    }

    return false;
  }

  //___________________|
  // ADVANCED MESSAGES |
  //___________________|

  std::string Command::getInlineRootNames(
    mt::CR_STR separator,
    mt::CR_BOL startWithSeparator,
    mt::CR_BOL endWithSeparator
  ) {
    std::string text;
    Command *root = this;

    // looping up to root
    do {
      const bool isFirst = this == root;

      text = root->getFullName(
        true, isFirst, isFirst
      ) + separator + text;

      root = static_cast<Cm*>(root->parent);
    }
    while (root);

    if (startWithSeparator) {
      text = separator + text;
    }

    // remove last separator
    if (!endWithSeparator &&
      !separator.empty() &&
      text.length() >= separator.length()
    ) {
      text.erase(text.length() - separator.length());
    }

    return text;
  }

  bool Command::isTemporaryLetterCaseChange() {
    return !(Command::usingCaseSensitiveName ||
      Command::usingLowercaseName ||
      Command::usingUppercaseName
    );
  }

  void Command::onFreeChangeInputLetterCase(std::string &strIn) {

    if ((Command::matching || Command::selecting) &&
      !Command::usingCaseSensitiveName
    ) {
      if (Command::usingLowercaseName) {
        mt_uti::StrTools::changeStringToLowercase(strIn);
      }
      else mt_uti::StrTools::changeStringToUppercase(strIn);
    }
  }

  void Command::copyMatchName(std::string &hookName) {
    hookName = name;

    if (Command::isTemporaryLetterCaseChange()) {
      mt_uti::StrTools::changeStringToUppercase(hookName);
    }
  }

  void Command::copyMatchInput(
    std::string &hookInput,
    mt::CR_STR oriInput
  ) {
    hookInput = oriInput;
    onFreeChangeInputLetterCase(hookInput);
  }

  void Command::copyMatchStrings(
    std::string &hookName,
    std::string &hookInput,
    mt::CR_STR oriInput
  ) {
    copyMatchName(hookName);
    copyMatchInput(hookInput, oriInput);
  }

  void Command::changeTreeNamesToLowercase() {
    mt_uti::StrTools::changeStringToLowercase(name);

    for (TREE *node : children) {
      static_cast<Cm*>(node)->changeTreeNamesToLowercase();
    }
  }

  void Command::changeTreeNamesToUppercase() {
    mt_uti::StrTools::changeStringToUppercase(name);

    for (TREE *node : children) {
      static_cast<Cm*>(node)->changeTreeNamesToUppercase();
    }
  }

  void Command::printAfterBoundaryLine(std::string comName) {
    comName = "\n>" + comName + ":";

    if (Command::usingDashesBoundaryLine) {
      static bool isInit = true;

      if (!isInit) {
        Message::printBoundaryLine(1, 0);
      }

      isInit = false;

      Message::printString(
        comName, Color::WHITE
      );
    }
    else Message::printString(
      comName, Color::BLACK,
      isContainer() && !questionedGroup ?
        Color::LIGHT_BLUE : Color::LIGHT_GREEN
    );

    // dialog status in brackets
    std::cout << getDialogStatusString(true, true);

    // once at toddler level of 'Toggle'
    if (getInheritanceFlag() == TOGGLE && isToddler()) {
      static bool isInit = true;

      if (isInit && isToddler()) {
        isInit = false;
        Control::printToggleHelp();
      }
    }
    // once on the first call
    else {
      static bool isInit = true;

      if (isInit) {
        isInit = false;
        Control::printParameterHelp();
      }
    }

    // always have a newline
    std::cout << std::endl;
  }

  void Command::printQuestionBoundaryLine() {
    /**
     * The 'questionedGroup' must be true
     * before calling 'printAfterBoundaryLine'.
     */
    if (isContainer()) {
      questionedGroup = true;
      printAfterBoundaryLine(getInlineRootNames());
    }
    else printAfterBoundaryLine(
      getFullNameWithUltimate()
    );
  }

  void Command::printChildrenNamesDescriptions(
    mt::CR_BOL summarized,
    mt::CR_BOL startWithBoundaryLine,
    mt::CR_BOL endWithBoundaryLine
  ) {
    static Command *curCom = nullptr;
    static std::string text = "";
    static bool recentlySummarized = false;

    if (startWithBoundaryLine) {
      Message::printBoundaryLine(1, 1);
    }

    if (curCom != this ||
      recentlySummarized != summarized
    ) {
      curCom = this;
      recentlySummarized = summarized;
      Command *loopCom;

      // current dialog status
      if (!summarized) {
        text = Color::getString("THIS: ", Color::GREEN)
          + getDialogStatusString(false, false, true) + "\n";
      }
      else text = "";

      // save to variable
      for (int i = 0; i < children.size(); i++) {

        loopCom = static_cast<Cm*>(children[i]);
        if (i > 0) text += "\n";

        text += loopCom->getFullName(
          false, true, true, Color::WHITE
        ) + ":" + loopCom->getDialogStatusString(true, true, true);

        // for list control
        if (!summarized) {
          text += "\"" + loopCom->description + "\"\n";
        }
      }
    }

    // the 'text' is reusable within the same instance
    std::cout << text;

    if (endWithBoundaryLine) {
      Message::printBoundaryLine(0, 2);
    }
  }

  void Command::printNoItems() {
    Message::printNeatDialog(
      Message::ERROR_FLAG,
      "this " + getLevelName(isSupporter()) + " does not have any items"
    );
  }

  void Command::printList() {
    if (isParent()) {
      printChildrenNamesDescriptions(true);
    }
    else printNoItems();
  }

  void Command::printHelp() {
    Message::printBoundaryLine(1, 1);

    if (isParent()) {
      std::cout << Color::getString(
        mt_uti::StrTools::getStringToUppercase(name),
        Color::WHITE
      ) << ":\n\n";

      std::cout << description;
      printChildrenNamesDescriptions(false);
    }
    else {
      std::cout << getDialogStatusString(false, false, true) << description;
      Message::printBoundaryLine(1, 2);
    }
  }

  void Command::printError() {
    std::cout << "Command Error..";
  }

  void Command::clipboardAction() {
    Message::printNeatDialog(
      Message::ERROR_FLAG,
      "hidden text pasting is only available for parameters"
    );
  }

  void Command::checkDisguise(
    Command *command,
    mt::CR_STR occurrenceMethodName
  ) {
    int detected = -1;

    for (int i = 0; i < disguiseCount; i++) {
      if (!command->disguised &&
        command->getInheritanceFlag() == disguiseFlags[i]
      ) { break; }
    }

    if (detected >= 0) {
      command->disguised = true;

      Message::printNamed(
        Message::WARNING_FLAG,
        "Cannot add a '" + disguiseNames[detected][0] + "' (name: '" +
        command->name + "'). To keep proceeding, it is now considered as '"
        + disguiseNames[detected][1] + "'.",
        occurrenceMethodName,
        false
      );
    }
  }
}

#endif // __CLI_MENU__COMMAND_CPP__