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
      levelName = toEndUser ? "parameter" : "supporter";
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
    mt::VEC_STR levelNames;
    mt::VEC_BOL pluralConditions;

    if (isGroup()) {
      int anyGroup = 0,
        anyToddler = 0,
        anyUltimate = 0;

      // count level occurrences for conditions below
      for (TREE *node : children) {
        Command *curCom = static_cast<Cm*>(node);

        if (!onlyRequired ||
          (onlyRequired && curCom->required)
        ) {
          if (curCom->ultimate) {
            anyUltimate++;
          }
          else if (curCom->getNumberOfChildren() == 0) {
            anyToddler++;
          }
          else anyGroup++;
        }
      }

      /** Determining Name and Plural */

      if (anyGroup && anyToddler && anyUltimate) {
        levelNames = toEndUser ?
          mt::VEC_STR{"group", "command"} :
          mt::VEC_STR{"group", "toddler", "ultimate"};

        pluralConditions = toEndUser ?
          mt::VEC_BOL{anyGroup > 1, anyToddler > 1 || anyUltimate > 1} :
          mt::VEC_BOL{anyGroup > 1, anyToddler > 1, anyUltimate > 1};
      }
      else if (anyGroup && anyToddler) {
        levelNames = toEndUser ?
          mt::VEC_STR{"group", "command"} :
          mt::VEC_STR{"group", "toddler"};

        pluralConditions = mt::VEC_BOL{anyGroup > 1, anyToddler > 1};
      }
      else if (anyToddler && anyUltimate) {
        levelNames = toEndUser ?
          mt::VEC_STR{"command"} :
          mt::VEC_STR{"toddler", "ultimate"};

        pluralConditions = toEndUser ?
          mt::VEC_BOL{anyToddler > 1 || anyUltimate > 1} :
          mt::VEC_BOL{anyToddler > 1, anyUltimate > 1};
      }
      else if (anyGroup && anyUltimate) {
        levelNames = toEndUser ?
          mt::VEC_STR{"group", "command"} :
          mt::VEC_STR{"group", "ultimate"};

        pluralConditions = mt::VEC_BOL{anyGroup > 1, anyUltimate > 1};
      }
      else if (anyGroup) {
        levelNames = mt::VEC_STR{"group"};
        pluralConditions = mt::VEC_BOL{anyGroup > 1};
      }
      else if (anyToddler) {
        levelNames = toEndUser ?
          mt::VEC_STR{"command"} :
          mt::VEC_STR{"toddler"};

        pluralConditions = mt::VEC_BOL{anyToddler > 1};
      }
      else if (anyUltimate) {
        levelNames = toEndUser ?
          mt::VEC_STR{"command"} :
          mt::VEC_STR{"ultimate"};

        pluralConditions = mt::VEC_BOL{anyUltimate > 1};
      }
    }
    // ultimate does not need to count with loop
    else if (isUltimate()) {
      levelNames = toEndUser ?
        mt::VEC_STR{"parameter"} :
        mt::VEC_STR{"supporter"};

      pluralConditions = mt::VEC_BOL{children.size() > 1};
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
      levelNames = toEndUser ?
        mt::VEC_STR{"command"} :
        mt::VEC_STR{"toddler"};

      pluralConditions = mt::VEC_BOL{false};
    }

    std::string renderedName;
    bool isLast;

    // concatenate string vector into a sentence
    for (int i = 0; i < levelNames.size(); i++) {

      isLast = i == levelNames.size() - 1;

      if (isLast && levelNames.size() > 1) {
        renderedName += "or ";
      }

      renderedName += levelNames[i];

      if (pluralConditions[i]) {
        renderedName += 's';
      }

      if (!isLast) {
        renderedName += levelNames.size() > 2 ? ", " : " ";
      }
    }

    // change first character to uppercase
    if (isFirstCapitalLetter) {
      renderedName[0] = char(std::toupper(renderedName[0]));
    }

    return renderedName;
  }

  // invoked in 'question' when 'requiredItems' is not empty
  void Command::printEnterError() {
    if (ultimate) {
      if (getRequiredCount() > 1) {
        Message::printDialogError(
          "Cannot process until all required parameters are met."
        );
      }
      // 'requiredItems' remaining 1
      else Message::printDialogError(
        "Cannot be processed. This command still has a parameter named '"
        + requiredItems.back()->getName()
        + "' that must be filled in."
      );
    }
    // group or toddler
    else {
      const std::string
        levelName = getChildrenLevelName(),
        errStr[2] = {
          "Cannot be processed. This group still has ",
          " that must be used."
        };

      if (getRequiredCount() > 1) {
        Message::printDialogError(
          errStr[0] + levelName + errStr[1]
        );
      }
      // 'requiredItems' remaining 1
      else Message::printDialogError(
        errStr[0] + "a " + levelName + " named '"
        + requiredItems.back()->getName()
        + "'" + errStr[1]
      );
    }
  }

  // invoked in 'question' when 'requiredItems' is not empty
  void Command::printNextError() {
    if (isSupporter()) {
      // there is the word 'input' because this could be 'Toggle'
      Message::printDialogError(
        "Cannot skip with empty input on required parameter."
      );
    }
    // group, ultimate, or toddler
    else Message::printDialogError(
      "Cannot skip this " + getLevelName() + " with empty argument."
    );
  }

  void Command::printOrphanError() {
    Message::printDialogError(
      "This " + std::string(getInheritanceFlag() == PROGRAM ? "program" : "command")
      + " has no connections."
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
      else if (!requiredItems.empty()) {
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
    VEC_TREE released = releaseChildren();
    mt_uti::VecTools<TREE*>::concat(united, released);
    ultimate = newUltimate;

    for (TREE *node : released) {
      node->resign();
      static_cast<Cm*>(node)->collapseUltimateItems(newUltimate, united);
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
    else if (isParent()) return FLAG::ERROR;

    // supporter points its parent
    *lastCom = ultimate;

    return FLAG::COMPLETED;
  }

  //________|
  // DIALOG |
  //________|

  void Command::printDialogStatus() {
    std::string status = " (";
    Color fontColor;

    if (!isGroupNeedQuestion()) {
      status += "sel"; // 1st
      fontColor = Color::MAGENTA;
    }
    // supporter
    else {
      status += "par, "; // 1st

      if (required) status += "req, "; // 2nd
      else status += "opt, ";

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
    return "cor";
  }

  mt::USI Command::questionTo(
    Command *target,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) return target->question(paramData, lastCom);
    return FLAG::COMPLETED;
  }

  mt::USI Command::dialog(
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
        if (onEnter(paramData, lastCom)) {
          return FLAG::COMPLETED;
        }
      }
      else if (Control::nextTest(controlStr)) {
        // pointing to neighbor
        if (next) {
          return static_cast<Cm*>(next)->dialog(paramData, lastCom);
        }
        // redirected to first child
        else if (getNumberOfChildren() > 0) {
          return static_cast<Cm*>(children[0])->dialog(paramData, lastCom);
        }
        // toddler
        else if (parent) {
          Message::printDialogError(
            "The '" + parent->getName()
            + "' group has only one "
            + getChildrenLevelName() + "."
          );
        }
        // program or orphan (rarely occurs)
        else printOrphanError();
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
          // program or orphan
          else {
            printOrphanError();
            continue;
          }

          found = static_cast<Cm*>(usedParent->getChild(nameTest));
        }

        if (found) {
          return found->dialog(paramData, lastCom);
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
    ParamData &paramData,
    Command **lastCom
  ) {
    if (target) return target->dialog(paramData, lastCom);
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
      return matchTo(static_cast<Cm*>(next), inputs, paramData, lastCom);
    }

    circularCheckpoint = nullptr;
    return FLAG::ERROR;
  }

  /**
   * This will not be used by program or orphan because there is
   * already a condition for empty children in 'Program::run'.
   */
  bool Command::isMatchNeedDialog() {
    Command *parCom = static_cast<Cm*>(parent);

    if (Command::dialogued && parCom &&
      parCom->getRequiredCount()
    ) {
      // group or toddler
      if (!parCom->ultimate) {

        Message::printDialogError(
          "The '" + parCom->name + "' "
          + parCom->getLevelName() + " has "
          + parCom->getChildrenLevelName()
          + " to be used.", 1
        );

        return true;
      }
      // ultimate or supporter
      else {
        Message::printDialogError(
          "The '" + parCom->name
          + "' command has required "
          + getChildrenLevelName()
          + " to be filled in.", 1
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
    return !Command::usingCaseSensitiveName &&
      !Command::usingLowercaseName && !Command::usingUppercaseName;
  }

  void Command::onFreeChangeInputLetterCase(std::string &strIn) {
    if (!Command::usingCaseSensitiveName) {
      if (Command::usingLowercaseName) {
        mt_uti::StrTools::changeStringToLowercase(strIn);
      }
      else mt_uti::StrTools::changeStringToUppercase(strIn);
    }
  }

  void Command::copyMatchNames(
    std::string &hookName, std::string &hookInput,
    mt::CR_STR oriName, mt::CR_STR oriInput
  ) {
    hookName = oriName;

    if (Command::isTemporaryLetterCaseChange()) {
      mt_uti::StrTools::changeStringToUppercase(hookName);
    }

    hookInput = oriInput;
    Command::onFreeChangeInputLetterCase(hookInput);
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

    /** Once displayed */

    // container or 'Parameter' supporter
    if (!(isSupporter() && getInheritanceFlag() == TOGGLE)) {
      Control::printHelp(isSupporter());
    }
    // 'Toggle' supporter
    else {
      static bool isClosedInit = true;

      if (isClosedInit && isSupporter()) {
        isClosedInit = false;

        Message::printItalicString(
          "  yes = y, no = n, or boolean\n"
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