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
    CR_SP_CALLBACK callback_in,
    Command *parent_in,
    mt::CR_BOL required_in
  ): TREE(name_in, parent_in) {
    description = description_in;
    callback = callback_in;
    required = required_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    Command *parent_in,
    mt::CR_BOL required_in
  ): TREE(name_in, parent_in) {
    description = description_in;
    plainCallback = callback_in;
    required = required_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    Command *parent_in,
    mt::CR_BOL required_in
  ): TREE(name_in, parent_in) {
    description = description_in;
    required = required_in;
  }

  //__________|
  // ULTIMATE |
  //__________|

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
  std::string Command::getFullNameWithUltimate() {
    return " " + Color::getString(
      ultimate->name, getMainLabelColor()
    ) + " " + getFullName();
  }

  void Command::updateRequiredItems(
    Command *command,
    mt::CR_BOL adding
  ) {
    if (command->required) {
      if (adding) {
        requiredItems.push_back(command);
      }
      else mt_uti::VecTools<TREE*>::cutSingle(
        requiredItems,
        mt_uti::VecTools<TREE*>::getIndex(
          requiredItems, command
        )
      );
    }
  }

  void Command::updateRequiredSelf(mt::CR_BOL adding) {
    if (!used) {
      // occurs once (safeguard)
      used = true;

      if (ultimate) {
        ultimate->updateRequiredItems(this, adding);
      }
    }
  }

  void Command::collapseUltimateItems(
    Command *newUltimate,
    VEC_TREE &united
  ) {
    ultimate = newUltimate;
    VEC_TREE released = releaseChildren();
    mt_uti::VecTools<TREE*>::concat(united, released);

    for (TREE *node : released) {
      static_cast<Cm*>(node)->collapseUltimateItems(newUltimate, united);
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
    else if (isGroup()) return FLAG::ERROR;
    return FLAG::COMPLETED;
  }

  //________|
  // DIALOG |
  //________|

  const std::string
    Command::error_string_enter = "Cannot process until all required parameters are met",
    Command::error_string_next = "Cannot skip with empty input on required parameter";

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
    }

    Message::printItalicString(
      status + ")\n", fontColor
    );
  }

  std::string Command::getFillingStatusString() {
    if (!used) return "emp";
    return "cor";
  }

  /**
   * The 'ultimate' is guaranteed to exist because
   * this method always called at supporter level.
   */
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

    printAfterBoundaryLine(
      getAccumulatedInlineRootNames(" ", true)
    );

    while (true) {
      Message::setDialogInput(nameTest);

      // copy to secure original input
      std::string controlStr = mt_uti::StrTools::getStringToLowercase(nameTest);

      if (Control::cancelTest(controlStr)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(controlStr)) {
        Message::printDialogError(
          "Cannot process before parameters"
        );
      }
      else if (Control::nextTest(controlStr)) {
        // pointing to neighbor
        if (next) {
          return static_cast<Cm*>(next)->dialog(paramData, lastCom);
        }
        // redirected to first child
        else if (getNumberOfChildren() > 0) {
          if (isUltimate()) {
            return static_cast<Cm*>(children[0])->question(paramData, lastCom);
          }
          return static_cast<Cm*>(children[0])->dialog(paramData, lastCom);
        }
        // group that has no children
        else Message::printDialogError(
          "The command has only one parameter"
        );
      }
      // find developer defined command
      else {
        Command *found;
        Command::onFreeChangeInputLetterCase(nameTest);

        if (isGroup()) found = static_cast<Cm*>(getChild(nameTest));
        else found = static_cast<Cm*>(ultimate->getChild(nameTest));

        if (found) {
          if (found->isSupporter()) {
            return found->question(paramData, lastCom);
          }
          return found->dialog(paramData, lastCom);
        }
        else if (isUltimate() || isSupporter()) {
          Message::printDialogError("Parameter not found");
        }
        else Message::printDialogError("Group not found");
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

  bool Command::isMatchNeedDialog() {
    return Command::dialogued &&
    (isGroup() || (ultimate && getRequiredCount() != 0));
  }

  //___________________|
  // ADVANCED MESSAGES |
  //___________________|

  std::string Command::getInlineRootNames(
    mt::CR_STR separator,
    mt::CR_BOL fully
  ) {
    std::string text;
    Command *root = static_cast<Cm*>(parent);

    static const std::function<void(Command*)>
      add = [&](Command *com) {
        text = (fully ? com->getFullName() : com->name) + separator;
      };

    add(this);

    // looping up to root
    while (root) {
      add(root);
      root = static_cast<Cm*>(root->parent);
    }

    return text;
  }

  std::string Command::getAccumulatedInlineRootNames(
    mt::CR_STR separator,
    mt::CR_BOL fully
  ) {
    static std::string inlineNames = "";

    if (inlineNames.empty() && parent) {
      inlineNames += " " + static_cast<Cm*>(parent)
        ->getInlineRootNames(separator, fully);
    }
    else inlineNames += " ";

    inlineNames += getFullName();
    return inlineNames;
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
      isGroup() ? Color(223, 223, 223) : Color::WHITE
    );

    // has a newline at the end
    printDialogStatus();

    /** Once displayed */

    // group or supporter parameter
    if (!ultimate || (isSupporter() && getInheritanceFlag() == PARAMETER)) {
      Control::printHelp(isSupporter());
    }
    // supporter toggle
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

    if (isGroup()) {
      Message::printBoundaryLine();
      std::cout << getBranchLeafString(1, !isUltimate());
    }
  }

  void Command::printError() {
    std::cerr << "Command Error..";
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