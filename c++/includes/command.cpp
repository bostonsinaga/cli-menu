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
    CR_SP_CALLBACK callback_in
  ): TREE(name_in) {
    description = description_in;
    callback = callback_in;
    required = required_in;
    setParent(parent_in);
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    CR_SP_PLAIN_CALLBACK callback_in
  ): TREE(name_in) {
    description = description_in;
    plainCallback = callback_in;
    required = required_in;
    setParent(parent_in);
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in
  ): TREE(name_in) {
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
        relateToSupporter(node, true);
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
      relateToSupporter(node, true);
      updateRequiredItems(static_cast<Cm*>(node), true);
    }
  }

  // the 'node[index]' is guaranteed to always be exist
  Command::TREE *Command::dismantle(mt::CR_INT index) {
    if (isParent()) {
      TREE *node = TREE::dismantle(index);
      relateToSupporter(node, false);
      updateRequiredItems(static_cast<Cm*>(node), false);
      return node;
    }
    return nullptr;
  }

  Command::VEC_TREE Command::releaseChildren() {
    if (isParent()) {
      VEC_TREE released = TREE::releaseChildren();

      for (TREE *node : released) {
        relateToSupporter(node, false);
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
    if (isSupporter()) {
      return ultimate->requiredItems.size();
    }
    return requiredItems.size();
  }

  Color Command::getMainLabelColor() {
    if (Command::usingDashesBoundaryLine) {
      return Color::MINT;
    }
    return Color::TEAL;
  }

  std::string Command::getMainLabel() {
    if (isUltimate()) {
      return Color::getString(
        "[MAIN]", getMainLabelColor()
      );
    }
    return "";
  }

  // the 'ultimate' confirmed to exist when this invoked
  std::string Command::getFullNameWithUltimate(
    mt::CR_STR separator,
    mt::CR_BOL startWithSeparator,
    mt::CR_BOL endWithSeparator
  ) {
    return (startWithSeparator ? separator : "")
      + Color::getString(
        ultimate->name, getMainLabelColor()
      ) + separator + getFullName()
      + (endWithSeparator ? separator : "");
  }

  std::string Command::getLevelName(
    mt::CR_BOL toEndUser,
    mt::CR_BOL isFirstCapitalLetter
  ) {
    std::string levelName;

    if (!parent) {
      levelName = "program";
    }
    else if (isGroup()) {
      levelName = "group";
    }
    else if (isUltimate()) {
      levelName = toEndUser ? "command" : "ultimate";
    }
    else if (isSupporter()) {
      levelName = toEndUser ? (
        getInheritanceFlag() == PARAMETER ? "parameter" : "toggle"
      ) : "supporter";
    }
    else levelName = toEndUser ? "command" : "toddler";

    // change first character to uppercase
    if (isFirstCapitalLetter) {
      levelName[0] = char(std::toupper(levelName[0]));
    }

    return levelName;
  }

  std::string Command::getChildrenLevelName(
    mt::CR_BOL toEndUser,
    mt::CR_BOL onlyRequired,
    mt::CR_BOL isFirstCapitalLetter
  ) {
    static const mt::USI maxCount = 4;
    mt::VEC_USI indexes;
    std::string availableNames[maxCount], levelNames[maxCount];
    bool pluralConditions[] = {false, false, false, false};
    mt::USI availableCount = 0;

    if (isGroup()) {
      levelNames[0] = "command";
      levelNames[1] = "group";
      levelNames[2] = "toddler";
      levelNames[3] = "ultimate";

      // capture level occurrences
      for (TREE *node : children) {
        Command *curCom = static_cast<Cm*>(node);

        if (!onlyRequired ||
          (onlyRequired && curCom->required)
        ) {
          if (curCom->ultimate) {
            indexes.push_back(toEndUser ? 0 : 3);
          }
          else if (curCom->getNumberOfChildren() == 0) {
            indexes.push_back(toEndUser ? 0 : 2);
          }
          else indexes.push_back(1);
        }
      }
    }
    // ultimate does not need to count with loop
    else if (isUltimate()) {
      if (toEndUser) {
        levelNames[0] = "command";
        levelNames[1] = "parameter";
        levelNames[2] = "toggle";

        // capture supporter occurrences
        for (TREE *node : children) {
          Command *curCom = static_cast<Cm*>(node);

          if (!onlyRequired ||
            (onlyRequired && curCom->required)
          ) {
            switch (curCom->getInheritanceFlag()) {
              case PARAMETER: {
                indexes.push_back(1);
              break;}
              case TOGGLE: {
                indexes.push_back(2);
              break;}
              default: {
                indexes.push_back(0);
              }
            }
          }
        }
      }
      else {
        availableCount = 1;
        availableNames[0] = "supporter";
        pluralConditions[0] = getRequiredCount() > 1;
      }
    }
    // toddler or supporter
    else if (parent) {
      return static_cast<Cm*>(parent)->getChildrenLevelName(
        toEndUser,
        onlyRequired,
        isFirstCapitalLetter
      );
    }
    // program or orphan
    else {
      availableCount = 1;
      availableNames[0] = toEndUser ? "command": "toddler";
    }

    mt::USI pluralCount = 0;
    mt::USI occurrences[] = {0, 0, 0, 0};

    // set names and plural conditions
    for (mt::USI &i : indexes) {
      if (occurrences[i] < 2) {

        if (occurrences[i] < 1) {
          availableNames[availableCount] = levelNames[i];
          availableCount++;
        }
        else {
          pluralCount++;
          pluralConditions[availableCount - 1] = true;
        }

        // possible values: 0, 1, 2
        occurrences[i]++;
      }
      else if (pluralCount == maxCount) break;
    }

    // the product
    std::string renderedName;
    bool isLast;

    // concatenate string vector into a sentence
    for (int i = 0; i < availableCount; i++) {

      if (availableNames[i].empty()) break;
      else {
        isLast = i == availableCount - 1;

        if (isLast && availableCount > 1) {
          renderedName += "or ";
        }

        renderedName += availableNames[i];

        if (pluralConditions[i]) {
          renderedName += 's';
        }

        if (!isLast) {
          renderedName += availableCount > 2 ? ", " : " ";
        }
      }
    }

    // change first character to uppercase
    if (isFirstCapitalLetter) {
      renderedName[0] = char(std::toupper(renderedName[0]));
    }

    return renderedName;
  }

  // for supporters
  bool Command::doUltimateAllowEnter(mt::CR_BOL fromChild) {

    if (ultimate) {
      Command *ulCom = static_cast<Cm*>(ultimate);
      const mt::UI reqCt = ulCom->getRequiredCount();

      // all complete
      if (!reqCt) return true;
      else {
        // error strings
        const std::string
          errStr_1 = "Cannot process before ",
          errStr_2 = errStr_1
            + std::string(reqCt == 1 ? "a " : "")
            + "required ";

        Command *oneLeft = nullptr;

        if (reqCt == 1) {
          oneLeft = static_cast<Cm*>(ulCom->requiredItems.back());
        }

        // some incomplete
        if (reqCt > 1) Message::printDialogError(
          errStr_2 + ulCom->getChildrenLevelName() + "."
        );
        // self incomplete
        else if (oneLeft == this) Message::printDialogError(
          errStr_1 + "this " + getLevelName() + " is " + std::string(
            getInheritanceFlag() == PARAMETER ? "filled in." : "specified."
          )
        );
        // one incomplete
        else Message::printDialogError(
          errStr_2 + oneLeft->getLevelName()
          + " named '" + oneLeft->name + "'."
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
    Message::printDialogError(
      "Cannot skip this " + getLevelName() + " with empty "
      + (getInheritanceFlag() == PARAMETER ? "argument" : "condition") + "."
    );
  }

  // for single toddler
  void Command::printSingleNextError() {
    if (parent) {
      Command* parCom = static_cast<Cm*>(parent);

      Message::printDialogError(
        "The '" + parCom->name + "' "
        + parCom->getLevelName() + " has only one "
        + parCom->getChildrenLevelName() + "."
      );
    }
    else printOrphanError();
  }

  // for program or orphan (rarely used)
  void Command::printOrphanError() {
    Message::printDialogError(
      "This " + std::string(getInheritanceFlag() == PROGRAM ? "program" : "command")
      + " has no connections."
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

  void Command::relateToSupporter(
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

  bool Command::run() {
    if (plainCallback) {
      (*plainCallback)();
      return true;
    }
    return false;
  }

  bool Command::run(ParamData &paramData) {
    if (callback) {
      (*callback)(
        paramData.texts,
        paramData.numbers,
        paramData.conditions
      );
      return true;
    }
    else if (run()) return true;
    return false;
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
    return static_cast<Cm*>(next)->getUnusedNeighbor(start);
  }

  mt::USI Command::matchTo(
    Command *target,
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) {
      return target->match(inputs, paramData, lastCom);
    }
    else if (isParent()) return FLAG::FAILED;

    // toddler pointing to its parent
    *lastCom = chooseLastCommand(true);

    return FLAG::COMPLETED;
  }

  //________|
  // DIALOG |
  //________|

  void Command::printDialogStatus() {
    std::string status = " (";
    Color fontColor;

    if (!(isSupporter() || questionedGroup)) {
      status += "sel"; // 1st
      fontColor = Color::MAGENTA;
    }
    // supporter
    else {
      status += "par, "; // 1st

      if (required) status += "req, "; // 2nd
      else status += Color::getString(
        "opt", Color::MAGENTA
      ) + ", ";

      status += getFillingStatusString(); // 3rd
      fontColor = Color::VIOLET;

      // group returns to call 'dialog' method after 'question'
      questionedGroup = false;
    }

    Message::printItalicString(
      status + ")\n", fontColor
    );
  }

  std::string Command::getFillingStatusString() {
    if (!used) return "emp";
    return Color::getString("cor", Color::MAGENTA);
  }

  mt::USI Command::questionTo(
    Command *target,
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) return target->question(
      inputs, paramData, lastCom
    );
    return FLAG::COMPLETED;
  }

  mt::USI Command::dialog(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string nameTest;
    printAfterBoundaryLine(getInlineRootNames());

    while (true) {
      Message::setDialogInput(nameTest);

      // copy to secure original input
      std::string controlStr = mt_uti::StrTools::getStringToLowercase(nameTest);

      if (Control::cancelTest(controlStr)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(controlStr)) {
        // pointing to first child
        if (isParent()) {
          return dialogTo(
            static_cast<Cm*>(children.front()), inputs, paramData, lastCom
          );
        }
        // directly completed
        else if (doUltimateAllowEnter()) {
          *lastCom = chooseLastCommand();

          if (getInheritanceFlag() == TOGGLE) {
            setData(paramData, false);
          }

          return FLAG::COMPLETED;
        }
      }
      else if (Control::nextTest(controlStr)) {
        // pointing to neighbor
        if (next) {
          return static_cast<Cm*>(next)->dialog(
            inputs, paramData, lastCom
          );
        }
        // redirected to first child
        else if (isParent()) {
          return static_cast<Cm*>(children.front())->dialog(
            inputs, paramData, lastCom
          );
        }
        else printSingleNextError();
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
          return found->dialog(inputs, paramData, lastCom);
        }
        else if (isUltimate()) {
          Message::printDialogError("Parameter not found.");
        }
        else if (children.empty()) {
          Message::printDialogError("This command does not have any parameters.");
        }
        else Message::printDialogError(
          getChildrenLevelName(true, true, true) + " not found."
        );
      }
    }

    return FLAG::CANCELED;
  }

  mt::USI Command::dialogTo(
    Command *target,
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) return target->dialog(
      inputs, paramData, lastCom
    );
    return FLAG::COMPLETED;
  }

  mt::USI Command::askNeighbor(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (!circularCheckpoint) {
      circularCheckpoint = this;
    }

    if (next != circularCheckpoint) {
      return matchTo(
        static_cast<Cm*>(next), inputs, paramData, lastCom
      );
    }

    // reset value
    circularCheckpoint = nullptr;

    if (isMatchNeedDialog()) return dialogTo(
      static_cast<Cm*>(parent), inputs, paramData, lastCom
    );

    return FLAG::FAILED;
  }

  /**
   * This will not be used by program or orphan because there is
   * already a condition for empty children in 'Program::run'.
   */
  bool Command::isMatchNeedDialog() {
    Command *parCom = static_cast<Cm*>(parent);
    const mt::UI reqCt = parCom ? parCom->getRequiredCount() : 0;

    if (Command::dialogued && parCom && reqCt) {
      Command *oneLeft = nullptr;

      if (reqCt == 1) {
        oneLeft = static_cast<Cm*>(parCom->requiredItems.back());
      }

      // group or toddler
      if (!parCom->ultimate) {

        const std::string
          firstErrStr = "The '" + parCom->name + "' group has ",
          lastErrStr = " that need"
            + std::string(reqCt == 1 ? "s": "")
            + " to be used.";

        if (reqCt > 1) Message::printDialogError(
          firstErrStr + parCom->getChildrenLevelName()
          + lastErrStr, 1
        );
        // one incomplete
        else Message::printDialogError(
          firstErrStr + "a " + oneLeft->getLevelName()
          + " named '" + oneLeft->name + "'"
          + lastErrStr, 1
        );

        return true;
      }
      // ultimate or supporter
      else {
        const std::string
          errStr = "The '" + parCom->name
            + "' command has " + (reqCt == 1 ? "a " : "")
            + "required ";

        if (reqCt > 1) Message::printDialogError(
          errStr + getChildrenLevelName() + ".", 1
        );
        // one missed
        else Message::printDialogError(
          errStr + oneLeft->getLevelName()
          + " named '" + oneLeft->name + "'.", 1
        );

        return true;
      }
      return false;
    }
    return false;
  }

  //___________________|
  // ADVANCED MESSAGES |
  //___________________|

  std::string Command::getInlineRootNames(
    mt::CR_STR separator,
    mt::CR_BOL fully,
    mt::CR_BOL startWithSeparator,
    mt::CR_BOL endWithSeparator
  ) {
    std::string text;
    Command *root = this;

    // looping up to root
    do {
      text = (fully ? root->getFullName() : root->name)
        + separator + text;

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

  std::string Command::getBranchLeafString(
    mt::CR_INT spacesCount,
    mt::CR_INT columnIndex,
    mt::CR_BOL withDescription
  ) {
    static std::string separator = std::string(spacesCount, ' ');

    std::string text;
    TREE *root = parent;
    int tabsCount = 0;

    // looping up to root
    while (root) {
      tabsCount += root->getName().length() + separator.length();
      root = root->getParent();
    }

    text += name + separator;

    if (columnIndex > 0) {
      text = std::string(tabsCount, ' ') + text;
    }

    // display a neat description
    if (withDescription && children.empty()) {
      mt::VEC_STR lines {""};

      // detect newline characters
      for (char &ch : description) {
        lines.back().push_back(ch);
        if (ch == '\n') lines.push_back("");
      }

      // get rid empty strings created due to detected newlines
      for (int i = 0; i < lines.size(); i++) {
        if (lines[i].empty()) {
          mini_tools::utils::VecTools<std::string>::cutSingle(lines, i);
          i--;
        }
      }

      // push 'lines' vector to 'text' string
      for (int i = 0; i < lines.size(); i++) {

        if (i > 0 || columnIndex > 0) {
          lines[i] = std::string(
            tabsCount + name.length() + separator.length(), ' '
          ) + lines[i];
        }

        text += lines[i];
      }
    }
    // has children (recursion)
    else for (int i = 0; i < getNumberOfChildren(); i++) {
      text += static_cast<Cm*>(children[i])->getBranchLeafString(
        spacesCount, i, withDescription
      );
    }

    // add newline to every index except the last
    if (parent &&
      columnIndex < parent->getNumberOfChildren() - 1
    ) { text += "\n"; }

    return text;
  }

  std::string Command::getBranchLeafString(
    int spacesCount,
    mt::CR_BOL withDescription
  ) {
    if (spacesCount < 1) spacesCount = 1;
    return getBranchLeafString(spacesCount, 0, withDescription);
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
        Message::printBoundaryLine();
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

    // has a newline at the end
    printDialogStatus();

    // once at toddler level of 'Toggle'
    if (getInheritanceFlag() == TOGGLE &&
      isToddler()
    ) {
      static bool isInit = true;

      if (isInit && isSupporter()) {
        isInit = false;

        Message::printItalicString(
          "  yes = y, no = n, or boolean\n"
        );
      }
    }
    // once on the first call
    else {
      static bool isInit = true;

      if (isInit) {
        isInit = false;

        Message::printItalicString(
          std::string("  .back     = :b, .cancel = :c,\n") +
          std::string("  .enter    = :e, .help   = :h,\n") +
          std::string("  .list     = :l, .next   = :n,\n") +
          std::string("  .previous = :p, .select = :s\n")
        );
      }
    }

    // always have a newline
    std::cout << std::endl;
  }

  void Command::printHelp() {
    std::cout << mt_uti::StrTools::getStringToUppercase(name) << ':';
    Message::printBoundaryLine();
    std::cout << description;

    if (isParent()) {
      Message::printBoundaryLine();
      std::cout << getBranchLeafString(1, !isUltimate());
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