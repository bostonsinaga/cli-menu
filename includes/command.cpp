#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {
  //______________|
  // CONSTRUCTORS |
  //______________|

  Command *Command::circularCheckpoint = nullptr;

  const Command::FullNameProfile
    Command::defaultFullNameProfile = {true, true, true},
    Command::helpFullNameProfile = {false, true, true, Color::WHITE};

  bool Command::usingCaseSensitiveName = true,
    Command::usingLowercaseName = false,
    Command::usingUppercaseName = false,
    Command::usingDashesBoundaryLine = false,
    Command::dialogued = false;

  const mt::USI Command::disguiseFlags[disguiseCount] = {
    PROGRAM
  };

  const std::string Command::disguiseNames[disguiseCount][2] = {
    {"Program", "Toggle"}
  };

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : TREE(name_in) {
    description = description_in;
    callback = callback_in;
    required = required_in;
    propagatingCallback = propagatingCallback_in;
    setParent(parent_in);
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : TREE(name_in) {
    description = description_in;
    plainCallback = callback_in;
    required = required_in;
    propagatingCallback = propagatingCallback_in;
    setParent(parent_in);
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in
  ) : TREE(name_in) {
    description = description_in;
    required = required_in;
    setParent(parent_in);
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
        relateToDependence(node, true);
        updateRequiredItems(static_cast<Cm*>(node), true);
      }
    }
  }

  void Command::addChild(
    TREE *node,
    mt::CR_BOL reconnected
  ) {
    if (isContainer()) {
      TREE::addChild(node, reconnected);
      relateToDependence(node, true);
      updateRequiredItems(static_cast<Cm*>(node), true);
    }
  }

  // the 'node[index]' is guaranteed to always be exist
  Command::TREE *Command::dismantle(mt::CR_INT index) {
    if (isParent()) {
      TREE *node = TREE::dismantle(index);
      relateToDependence(node, false);
      updateRequiredItems(static_cast<Cm*>(node), false);
      return node;
    }
    return nullptr;
  }

  Command::VEC_TREE Command::releaseChildren() {
    if (isParent()) {
      VEC_TREE released = TREE::releaseChildren();

      for (TREE *node : released) {
        relateToDependence(node, false);
        updateRequiredItems(static_cast<Cm*>(node), false);
      }

      return released;
    }

    return {};
  }

  //__________|
  // ULTIMATE |
  //__________|

  mt::UI Command::getRequiredCount() {
    if (isDependence()) {
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
    CR_FullNameProfile fullNameProfile,
    mt::CR_BOL startWithSeparator,
    mt::CR_BOL endWithSeparator
  ) {
    return (startWithSeparator ? separator : "")
      + Color::getString(
        ultimate->name, getLevelLabelColor()
      ) + separator + getFullName(fullNameProfile)
      + (endWithSeparator ? separator : "");
  }

  std::string Command::getLevelName() {
    std::string levelName;

    if (!parent) {
      levelName = "program";
    }
    else if (isGroup()) {
      levelName = "group";
    }
    else if (isUltimate()) {
      levelName = "command";
    }
    else levelName = "input";

    return levelName;
  }

  std::string Command::getChildrenLevelName(mt::CR_BOL onlyRequired) {

    // parent only
    if (!children.empty()) {

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
          if (loopCom->isGroup()) indexes[0]++;
          else if (loopCom->isUltimate()) indexes[1]++;
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

      std::string result;

      // return value assignment
      for (int i = 0; i < initAmount; i++) {

        if (!availableNames[i].empty()) {
          result += availableNames[i];

          if (i < initAmount - 1) {
            if (finalAmount == initAmount) {

              if (i == initAmount - 2) {
                result += ", or ";
              }
              else result += ", ";
            }
            else if (finalAmount == 2) {
              result += " or ";
            }
          }
        }
      }

      return result;
    }

    // called by toddler or orphan
    return "";
  }

  // not for program or question in the middle
  mt::USI Command::isItPossibleToGoBack(
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (parent) {
      if (!printDirectInputsError(directInputs, "go back")) {
        used = false;
        required = true;

        // program
        if (!parent->getParent()) {
          return static_cast<Cm*>(parent)->dialog(
            directInputs, paramData, lastCom
          );
        }

        return static_cast<Cm*>(parent)->question(
          directInputs, paramData, lastCom
        );
      }
    }
    else Message::printDialogError(
      "You are now at the root level. No more groups upward."
    );

    return FLAG::ERROR;
  }

  // for dependences
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

        Message::printNeatDialogError(
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

    // toddler
    return true;
  }

  // called in 'question' when 'requiredItems' contains
  void Command::printRequiredNextError() {
    // for any level
    Message::printNeatDialogError(
      "cannot skip this " + getLevelName() + " with empty "
      + (getInheritanceFlag() == PARAMETER ? "argument" : "condition")
    );
  }

  // for single toddler
  void Command::printNullptrNextError() {
    if (parent) {
      Command* parCom = static_cast<Cm*>(parent);

      Message::printNeatDialogError(
        "the '" + parCom->name + "' "
        + parCom->getLevelName() + " has only one "
        + parCom->getChildrenLevelName(false)
      );
    }
    else printOrphanError();
  }

  // for program or orphan (rarely used)
  void Command::printOrphanError() {
    Message::printNeatDialogError(
      "this input has no connections"
    );
  }

  Command *Command::chooseLastCommand(mt::CR_BOL onlyParent) {
    return ultimate ? ultimate : (
      onlyParent && parent ? static_cast<Cm*>(parent) : this
    );
  }

  void Command::updateRequiredItems(
    Command *command,
    mt::CR_BOL adding
  ) {
    if (command->required) {
      int reqIdx = mt_uti::VecTools<TREE*>::getIndex(
        requiredItems, command
      );

      // 'reqIdx' as duplication guard
      if (adding && reqIdx == -1) {
        requiredItems.push_back(command);
      }
      // 'reqIdx' always positive
      else if (!adding && !requiredItems.empty()) {
        command->required = false;

        mt_uti::VecTools<TREE*>::cutSingle(
          requiredItems, reqIdx
        );
      }
    }
  }

  void Command::updateRequiredUsed(mt::CR_BOL adding) {
    if (!used) {
      used = true;

      if (parent) {
        static_cast<Cm*>(parent)->updateRequiredItems(this, adding);
      }
    }
  }

  void Command::collapseUltimateItems(
    Command *newUltimate,
    VEC_TREE &united
  ) {
    VEC_TREE released = children;
    mt_uti::VecTools<TREE*>::concat(united, released);

    // avoid calling 'updateRequiredItems'
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

  void Command::relateToDependence(
    TREE *node,
    mt::CR_BOL connected
  ) {
    if (node && isUltimate()) {
      static_cast<Cm*>(node)->ultimate = connected ? this : nullptr;
    }
  }

  void Command::setCallback(CR_SP_CALLBACK callback_in) {
    callback.reset();
    callback = callback_in;
  }

  void Command::setCallback(CR_SP_PLAIN_CALLBACK callback_in) {
    plainCallback.reset();
    plainCallback = callback_in;
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

      updateRequiredItems(static_cast<Cm*>(united[i]), true);
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

  bool Command::run(ParamData &paramData) {
    bool called = false;

    if (callback) {

      (*callback)(
        paramData.texts,
        paramData.numbers,
        paramData.conditions
      );

      called = true;
    }
    else if (plainCallback) {
      (*plainCallback)();
      called = true;
    }

    if (propagatingCallback) {
      if (parent) {
        return static_cast<Cm*>(parent)->run(paramData);
      }
      return true;
    }

    return called;
  }

  bool Command::runTo(
    Command *target,
    ParamData &paramData
  ) {
    if (target) return target->run(paramData);
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

  mt::USI Command::matchTo(
    Command *target,
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) {
      return target->match(directInputs, paramData, lastCom);
    }
    else if (isParent()) return FLAG::FAILED;

    // toddler pointing to its parent
    *lastCom = chooseLastCommand(true);

    return FLAG::COMPLETED;
  }

  //________|
  // DIALOG |
  //________|

  bool Command::printDirectInputsError(
    mt::VEC_STR &directInputs,
    mt::CR_STR controlName
  ) {
    static Command *curCom = nullptr;
    static std::string text = "";

    if (directInputs.empty()) return false;
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

    Message::printNeatDialogError(text);
    return true;
  }

  mt::USI Command::tryToSkipWithSelection(
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom,
    mt::CR_STR additionalMessage
  ) {
    if (!printDirectInputsError(directInputs, "select")) {

      if (required) {
        Message::printNeatDialogError(
          "unable to select before " + additionalMessage
        );

        return FLAG::ERROR;
      }
      // the 'selecting' will be an opposite in overridden 'dialog' method
      else {
        if (isToddler()) {
          Command *parCom = static_cast<Cm*>(parent);
          parCom->selecting = true;

          return parCom->dialog(
            directInputs, paramData, lastCom
          );
        }

        // parent
        selecting = true;
        return dialog(directInputs, paramData, lastCom);
      }
    }

    return FLAG::ERROR;
  }

  // has a newline at the end
  std::string Command::getDialogStatusString(
    mt::CR_BOL usingAbbreviations,
    mt::CR_BOL withBrackets,
    mt::CR_BOL forcedToDependenceVersion
  ) {
    std::string status = withBrackets ? " (" : "";
    Color fontColor;

    // independence or parent
    if (!(isDependence() || questionedGroup || forcedToDependenceVersion)) {
      status += usingAbbreviations ? "sel" : "selection";
      fontColor = Color::MAGENTA;
    }
    // dependence
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
      status += getFillingStatusString(usingAbbreviations);
      fontColor = Color::VIOLET;

      // group returns to call 'dialog' method after 'question'
      questionedGroup = false;
    }

    return Color::getItalicString(
      status + (withBrackets ? ")\n" : "\n"), fontColor
    );
  }

  void Command::printDialogStatus(
    mt::CR_BOL usingAbbreviations,
    mt::CR_BOL withBrackets,
    mt::CR_BOL forcedToDependenceVersion
  ) {
    static Command *curCom = nullptr;
    static std::string text = "";

    if (curCom != this) {
      curCom = this;

      text = getDialogStatusString(
        usingAbbreviations,
        withBrackets,
        forcedToDependenceVersion
      );
    }

    // the 'text' is reusable within the same instance
    std::cout << text;
  }

  std::string Command::getHelpDialogStatusString() {
    return Color::getString("THIS: ", Color::GREEN)
      + getDialogStatusString(false, false, true) + "\n";
  }

  void Command::printHelpDialogStatus() {
    static Command *curCom = nullptr;
    static std::string text = "";

    if (curCom != this) {
      curCom = this;
      text = getHelpDialogStatusString();
    }

    std::cout << text;
  }

  std::string Command::getFillingStatusString(
    mt::CR_BOL usingAbbreviations
  ) {
    if (!used) {
      return usingAbbreviations ? "emp" : "empty";
    }

    return Color::getString(
      usingAbbreviations ? "cor" : "correction", Color::MAGENTA
    );
  }

  mt::USI Command::questionTo(
    Command *target,
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) return target->question(
      directInputs, paramData, lastCom
    );
    return FLAG::COMPLETED;
  }

  mt::USI Command::dialog(
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string nameTest;
    printAfterBoundaryLine(getInlineRootNames());

    while (true) {
      Message::setDialogInput(nameTest);

      // copy to secure original input
      std::string controlStr = mt_uti::StrTools::getStringToLowercase(nameTest);

      if (Control::backTest(controlStr)) {

        const mt::USI isItPossibleToGoBackFlag = isItPossibleToGoBack(
          directInputs, paramData, lastCom
        );

        if (isItPossibleToGoBackFlag != FLAG::ERROR) {
          return isItPossibleToGoBackFlag;
        }
      }
      else if (Control::cancelTest(controlStr)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(controlStr)) {
        // pointing to first child
        if (isParent()) {
          return dialogTo(
            static_cast<Cm*>(children.front()), directInputs, paramData, lastCom
          );
        }
        // directly completed
        else if (doesUltimateAllowEnter()) {
          *lastCom = chooseLastCommand();

          if (getInheritanceFlag() == TOGGLE) {
            setData(paramData, false);
          }

          return FLAG::COMPLETED;
        }
      }
      else if (Control::helpTest(controlStr)) {
        printHelp();
      }
      else if (Control::listTest(controlStr)) {
        printChildrenNamesDescriptions(true);
      }
      else if (Control::nextTest(controlStr)) {
        // pointing to neighbor
        if (next) {
          return static_cast<Cm*>(next)->dialog(
            directInputs, paramData, lastCom
          );
        }
        // redirected to first child
        else if (isParent()) {
          return static_cast<Cm*>(children.front())->dialog(
            directInputs, paramData, lastCom
          );
        }
        else printNullptrNextError();
      }
      else if (Control::selectTest(controlStr)) {
        Message::printNeatDialogError(
          "already in selection mode"
        );
      }
      // find developer defined command
      else {
        Command *found;
        Command::onFreeChangeInputLetterCase(nameTest);

        if (isParent()) {
          found = static_cast<Cm*>(getChild(nameTest));
        }
        else {
          TREE *usedParent;

          if (ultimate) usedParent = ultimate;
          else if (parent) usedParent = parent;
          else {
            printOrphanError();
            continue;
          }

          found = static_cast<Cm*>(usedParent->getChild(nameTest));
        }

        if (found) {
          return found->dialog(directInputs, paramData, lastCom);
        }
        else if (isUltimate()) {
          Message::printNeatDialogError("input not found");
        }
        // toddler
        else if (children.empty()) {
          Message::printNeatDialogError(
            "this " + getLevelName() + " does not have any items"
          );
        }
        // group
        else Message::printNeatDialogError(
          getChildrenLevelName(false) + " not found"
        );
      }
    }

    return FLAG::CANCELED;
  }

  mt::USI Command::dialogTo(
    Command *target,
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) return target->dialog(
      directInputs, paramData, lastCom
    );
    return FLAG::COMPLETED;
  }

  mt::USI Command::askNeighbor(
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (!circularCheckpoint && next) {
      circularCheckpoint = this;
    }

    if (next != circularCheckpoint) return matchTo(
      static_cast<Cm*>(next), directInputs, paramData, lastCom
    );

    std::string inputLevelName = "input";

    if (DashTest::isSingle(directInputs.back())) {
      inputLevelName = "toggle";
    }
    else if (DashTest::isDouble(directInputs.back())) {
      inputLevelName = "parameter";
    }

    Message::printNeatDialogError(
      "unknown " + inputLevelName + " named '" + name + "'", 1
    );

    // reset values
    circularCheckpoint = nullptr;
    directInputs = {};

    if (isMatchNeedDialog(false)) return dialogTo(
      static_cast<Cm*>(parent), directInputs, paramData, lastCom
    );

    return FLAG::FAILED;
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

          const std::string firstErrStr = "The '"
            + parCom->name + "' "
            + parCom->getLevelName() + " has ";

          const std::string lastErrStr = " that need"
            + std::string(isOneLeft ? "s": "")
            + " to be used.";

          if (reqCt > 1) Message::printDialogError(
            firstErrStr + parCom->getChildrenLevelName(true)
            + lastErrStr, 1
          );
          // one incomplete
          else Message::printDialogError(
            firstErrStr + "a " + oneLeft->getLevelName()
            + " named '" + oneLeft->name + "'"
            + lastErrStr, 1
          );
        }
        // ultimate or dependence
        else {
          const std::string errStr = "The '" + parCom->name
            + "' command has " + std::string(isOneLeft ? "a " : "")
            + "required input" + std::string(
              isOneLeft ? (" named '" + oneLeft->name + "'.") : "s."
            );

          Message::printDialogError(errStr, 1);
        }
      }

      return true;
    }

    return false;
  }

  //___________________|
  // ADVANCED MESSAGES |
  //___________________|

  std::string Command::getInlineRootNames(
    mt::CR_STR separator,
    FullNameProfile fullNameProfile,
    int listFormat,
    mt::CR_BOL startWithSeparator,
    mt::CR_BOL endWithSeparator
  ) {
    std::string text;
    Command *root = this;

    // looping up to root
    do {
      text = root->getFullName(fullNameProfile) + separator + text;
      root = static_cast<Cm*>(root->parent);

      if (listFormat == INLINE_NAMES_LAST) {
        fullNameProfile.useInputType = false;
        fullNameProfile.useLevelName = false;
        listFormat = INLINE_NAMES_NORMAL;
      }
    }
    while (root && listFormat != INLINE_NAMES_SELF);

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
    if (!Command::usingCaseSensitiveName) {
      if (Command::usingLowercaseName) {
        mt_uti::StrTools::changeStringToLowercase(strIn);
      }
      else mt_uti::StrTools::changeStringToUppercase(strIn);
    }
  }

  void Command::copyMatchName(
    std::string &hookName,
    mt::CR_STR oriName
  ) {
    hookName = oriName;

    if (Command::isTemporaryLetterCaseChange()) {
      mt_uti::StrTools::changeStringToUppercase(hookName);
    }
  }

  void Command::copyMatchInput(
    std::string &hookInput,
    mt::CR_STR oriInput
  ) {
    hookInput = oriInput;
    Command::onFreeChangeInputLetterCase(hookInput);
  }

  void Command::copyMatchStrings(
    std::string &hookName, std::string &hookInput,
    mt::CR_STR oriName, mt::CR_STR oriInput
  ) {
    copyMatchName(hookName, oriName);
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
        Color(223, 223, 255) :  // light blue
        Color(223, 255, 223)    // light green
    );

    // dialog status in brackets
    printDialogStatus(true, true);

    // once at toddler level of 'Toggle'
    if (getInheritanceFlag() == TOGGLE &&
      isToddler()
    ) {
      static bool isInit = true;

      if (isInit && isDependence()) {
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
        text = getHelpDialogStatusString();
      }
      else text = "";

      // save to variable
      for (int i = 0; i < children.size(); i++) {

        loopCom = static_cast<Cm*>(children[i]);
        if (i > 0) text += "\n";

        text += loopCom->getInlineRootNames(
          "", helpFullNameProfile, INLINE_NAMES_SELF, false
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
      printHelpDialogStatus();
      std::cout << description;
      Message::printBoundaryLine(1, 2);
    }
  }

  void Command::printError() {
    std::cout << "Command Error..";
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
        Message::STATUS::WARNING,
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