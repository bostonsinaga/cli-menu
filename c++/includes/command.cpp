#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {
  //______________|
  // CONSTRUCTORS |
  //______________|

  bool Command::usingCaseSensitiveName = true,
    Command::usingLowercaseName = false,
    Command::usingUppercaseName = false,
    Command::usingDashesBoundaryLine = false,
    Command::dialogued = false;

  void Command::setMetaData(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ) {
    name = name_in;
    description = description_in;
    required = required_in;
    accumulating = accumulating_in;
    setHolder(holder_in);
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ) {
    setMetaData(
      name_in, description_in,
      holder_in, required_in,
      accumulating_in
    );

    callback = callback_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ) {
    setMetaData(
      name_in, description_in,
      holder_in, required_in,
      accumulating_in
    );

    plainCallback = callback_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ) {
    setMetaData(
      name_in, description_in,
      holder_in, required_in,
      accumulating_in
    );
  }

  //___________________|
  // PARENT - CHILDREN |
  //___________________|

  bool Command::hasItem(Command *command) {
    for (Command *com : items) {
      if (com == command) return true;
    }
    return false;
  }

  bool Command::hasItem(mt::CR_STR name_in) {
    for (Command *com : items) {
      if (com->name == name_in) return true;
    }
    return false;
  }

  Command *Command::getItem(mt::CR_INT index) {
    return mt_uti::VecTools<Command*>::getAt(items, index, nullptr);
  }

  Command *Command::getItem(mt::CR_STR name_in) {
    bool needToUppercase = Command::isTemporaryLetterCaseChange();

    for (Command *com : items) {
      std::string nm = com->name;

      if (needToUppercase) {
        mt_uti::StrTools::changeStringToUppercase(nm);
      }

      if (nm == name_in) return com;
    }

    return nullptr;
  }

  Command *Command::getRoot() {
    Command *root = this;
    while (root->holder) {
      root = root->holder;
    }
    return root;
  }

  void Command::connectNext(int &index, VEC_COM &vecCom) {
    if (index > 0) {
      vecCom[index - 1]->next = vecCom[index];

      // connect last index to the first
      if (index == vecCom.size() - 1) {
        vecCom[index]->next = vecCom[0];
      }
    }
  }

  void Command::setItems(
    CR_VEC_COM newItems,
    mt::CR_BOL needEmpty,
    mt::CR_BOL validating
  ) {
    int startIdx = 0;

    if (needEmpty) items = {};
    else startIdx = items.size();

    // slower (safe)
    if (validating) {
      for (Command *com : newItems) {
        if (com) items.push_back(com);
      }
      cleanDuplicatesInItems();
    }
    // faster (danger)
    else items = newItems;

    for (int i = startIdx; i < items.size(); i++) {
      items[i]->setHolder(this, false);
      Command::connectNext(i, items);
    }
  }

  VEC_COM Command::setItemsRelease(
    CR_VEC_COM newItems,
    mt::CR_BOL validating
  ) {
    if (!isSupporter()) {
      VEC_COM oldItems = items;
      setItems(newItems, true, validating);
      return oldItems;
    }
    return {};
  }

  void Command::setItemsReplace(
    CR_VEC_COM newItems,
    mt::CR_BOL validating
  ) {
    if (!isSupporter()) {
      cleanItems();
      setItems(newItems, true, validating);
    }
  }

  void Command::addItems(
    CR_VEC_COM newItems,
    mt::CR_BOL validating
  ) {
    if (!isSupporter()) {
      setItems(newItems, false, validating);
    }
  }

  void Command::addItem(
    Command *command,
    mt::CR_BOL reconnected
  ) {
    if (command && !isSupporter()) {

      if (command->getInheritanceFlag() == PROGRAM) {
        command->disguise(true);

        Message::printNamed(
          Message::STATUS::WARNING,
          "Cannot add a 'Program' (name: '" +
          command->name + "'). To keep proceeding, it is now considered as 'Toggle'.",
          "cli_menu::Command::addItem",
          false
        );
      }

      if (reconnected) {
        command->setHolder(this, false);
      }

      if (!items.empty()) {
        items.back()->next = command;
      }

      items.push_back(command);
      cleanDuplicateToLastAdded(command);
      if (isUltimate()) updateRequiredItems(command, true);
    }
  }

  void Command::setHolder(
    Command *newHolder,
    mt::CR_BOL reconnected
  ) {
    if (newHolder) {
      if (holder) {
        holder->releaseItem(this);
      }

      if (reconnected) {
        newHolder->addItem(this, false);
      }

      if (newHolder->isUltimate()) {
        ultimate = newHolder;
      }

      holder = newHolder;
      level = holder->level + 1;
    }
  }

  void Command::cleanDuplicatesInItems() {
    std::tuple<VEC_COM, VEC_COM>
      wastedTuple = mt_uti::VecTools<Command*>::cleanDuplicateInside(
        items, false,
        [](Command *rep, Command *com)->bool {
          if (rep->name == com->name) return true;
          return false;
        }
      );

    // remove duplicates of same name
    for (Command *com : std::get<1>(wastedTuple)) {
      com->remove();
    }
  }

  void Command::cleanDuplicateToLastAdded(Command *command) {
    std::tuple<VEC_COM, VEC_COM>
      wastedTuple = mt_uti::VecTools<Command*>::cleanDuplicateToMember(
        items, command, false,
        [](Command *rep, Command *com)->bool {
          if (rep->name == com->name) return true;
          return false;
        }
      );

    // remove duplicates of same name
    for (Command *com : std::get<1>(wastedTuple)) {
      com->remove();
    }
  }

  void Command::cleanItems() {
    for (Command *com : items) {
      com->remove(false);
    }
  }

  void Command::sewNext(mt::CR_INT index) {
    if (index > 0) {
      // connect between except the index
      if (index + 1 < items.size()) {
        items[index - 1]->next = items[index + 1];
      }
      // connect last index to the first
      else if (index == items.size() - 1) {
        items[index]->next = items[0];
      }
    }
  }

  Command* Command::dismantle(mt::CR_INT index) {
    Command *target = items[index];
    sewNext(index);
    if (isUltimate()) updateRequiredItems(target, false);
    mt_uti::VecTools<Command*>::cutSingle(items, index);
    return target;
  }

  void Command::dismantleRemove(mt::CR_INT index) {
    dismantle(index)->remove(true);
  }

  Command* Command::dismantleRelease(mt::CR_INT index) {
    items[index]->level = 0;
    items[index]->holder = nullptr;
    items[index]->ultimate = nullptr;
    return dismantle(index);
  }

  void Command::removeItem(Command *command) {
    if (!command) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == command) {
        dismantleRemove(i);
        break;
      }
    }
  }

  void Command::removeItem(mt::CR_INT index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
      dismantleRemove(index);
    }
  }

  Command *Command::releaseItem(Command *command) {
    if (!command) return nullptr;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == command) {
        return dismantleRelease(i);
      }
    }

    return nullptr;
  }

  Command *Command::releaseItem(mt::CR_INT index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
      return dismantleRelease(index);
    }
    return nullptr;
  }

  VEC_COM Command::releaseItems() {
    VEC_COM released = items;

    for (Command *com : items) {
      com->level = 0;
      com->holder = nullptr;
      com->ultimate = nullptr;
    }

    items = {};
    requiredItems = {};
    return released;
  }

  void Command::remove(mt::CR_BOL firstOccurrence) {
    if (firstOccurrence && holder) {
      holder->releaseItem(this);
    }
    cleanItems();
    delete this;
  }

  //__________|
  // ULTIMATE |
  //__________|

  std::string Command::getMainLabel() {
    if (isUltimate()) {
      Color fontColor;

      if (Command::usingDashesBoundaryLine) {
        fontColor = Color::MINT;
      }
      else fontColor = Color::TEAL;

      return Color::getString("[MAIN]", fontColor);
    }
    return "";
  }

  // the 'ultimate' confirmed to exist when this invoked
  std::string Command::getFullNameWithUltimate() {
    return " " + ultimate->getFullName() + " " + getFullName();
  }

  void Command::updateRequiredItems(
    Command *command,
    mt::CR_BOL adding
  ) {
    if (command->required) {
      if (adding) {
        requiredItems.push_back(command);
      }
      else mt_uti::VecTools<Command*>::cutSingle(
        requiredItems,
        mt_uti::VecTools<Command*>::getIndex(requiredItems, command)
      );
    }
  }

  void Command::updateRequiredSelf(mt::CR_BOL adding) {
    if (ultimate) {
      ultimate->updateRequiredItems(this, adding);
    }
  }

  void Command::collapseUltimateItems(
    Command *newUltimate,
    VEC_COM &united
  ) {
    ultimate = newUltimate;
    mt_uti::VecTools<Command*>::concat(united, items);

    for (Command *com : items) {
      com->collapseUltimateItems(newUltimate, united);
    }

    items = {};
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
    VEC_COM united;

    collapseUltimateItems(this, united);
    items = united;
    Command::cleanDuplicatesInItems();

    for (int i = 0; i < united.size(); i++) {
      connectNext(i, united);
      updateRequiredItems(united[i], true);
      united[i]->holder = this;
      united[i]->level = level + 1;
    }
  }

  void Command::resignFromUltimate() {
    if (isUltimate()) {
      for (Command *com : items) com->ultimate = nullptr;
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

  Command *Command::getUnusedNext(Command *start) {
    if (!next || next == start) return nullptr;
    else if (!next->used) return next;
    return next->getUnusedNext(start);
  }

  Command *Command::matchTo(
    Command *target,
    mt::VEC_STR &inputs,
    ParamData &paramData
  ) {
    if (target) return target->match(inputs, paramData);
    else if (isGroup()) {
      // this is a 'Program' or without 'Program'
      if (!holder) return this;
      // contained by the 'Program'
      return holder;
    }
    return ultimate;
  }

  //________|
  // DIALOG |
  //________|

  const std::string
    Command::cannotProcessErrorString = "Cannot process until all required parameters are met",
    Command::cannotSkipErrorString = "Cannot skip with empty input on required parameter";

  void Command::printDialogError(mt::CR_STR reason) {
    Message::printString(
      "\n> " + reason + "\n\n", Color::RED
    );
  }

  void Command::printDialogStatus() {
    std::string status = " (";

    if (isGroup()) status += "sel";
    else {
      status += "par, ";

      if (required) status += "req, ";
      else status += "opt, ";

      if (!used) status += "emp";
      else if (accumulating) status += "acc";
      else status += "cor";
    }

    Message::printItalicString(
      status + ")\n", Color::MAGENTA
    );
  }

  /**
   * The 'ultimate' is guaranteed to exist because
   * this method always called at supporter level.
   */
  Command *Command::questionTo(
    Command *target,
    ParamData &paramData
  ) {
    if (target) return target->question(paramData);
    return ultimate;
  }

  Command *Command::dialog(ParamData &paramData) {
    /** Print name list */
    static std::string inlineNames = "";

    if (inlineNames.empty() && holder) {
      inlineNames += holder->getInlineRootNames(" ", true);
    }

    inlineNames += " " + getFullName();
    printAfterBoundaryLine(inlineNames);

    /** Get user input */
    std::string nameTest;

    while (true) {
      Message::setDialogInput(nameTest);

      if (Control::cancelTest(nameTest)) {
        break; // returns nullptr below
      }
      else if (Control::enterTest(nameTest)) {
        Command::printDialogError(
          "Cannot process before parameters"
        );
        continue;
      }
      else if (Control::nextTest(nameTest)) {
        // pointing to neighbor
        if (next) {
          return next->dialog(paramData);
        }
        // redirected to first item
        else if (getNumberOfItems() > 0) {
          if (isUltimate()) {
            return items[0]->question(paramData);
          }
          return items[0]->dialog(paramData);
        }
        // group that has no items
        else {
          Command::printDialogError(
            "The command has only one parameter"
          );
          continue;
        }
      }
      // find developer defined command
      else {
        Command *found;
        Command::onFreeChangeInputLetterCase(nameTest);

        if (isGroup()) found = getItem(nameTest);
        else found = ultimate->getItem(nameTest);

        if (found) {
          if (found->isSupporter()) {
            return found->question(paramData);
          }
          return found->dialog(paramData);
        }
        else if (isUltimate() || isSupporter()) {
          Command::printDialogError("Parameter not found");
        }
        else Command::printDialogError("Group not found");
      }
    }

    return nullptr; // canceled
  }

  Command *Command::dialogTo(
    Command *target,
    ParamData &paramData
  ) {
    if (target) return target->dialog(paramData);
    return nullptr;
  }

  //___________________|
  // ADVANCED MESSAGES |
  //___________________|

  std::string Command::getInlineRootNames(
    mt::CR_STR separator,
    mt::CR_BOL fully
  ) {
    std::string text;
    Command *root = holder;

    static const std::function<void(Command*)>
      add = [&](Command *com) {
        text = (fully ? com->getFullName() : com->name) + separator;
      };

    add(this);

    // looping up to root
    while (root) {
      add(root);
      root = root->holder;
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
    Command *root = holder;
    int tabsCount = 0;

    // looping up to root
    while (root) {
      tabsCount += root->name.length() + separator.length();
      root = root->holder;
    }

    text += name + separator;

    if (columnIndex > 0) {
      text = std::string(tabsCount, ' ') + text;
    }

    // display a neat description
    if (withDescription && items.empty()) {
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
    // has items (recursion)
    else for (int i = 0; i < items.size(); i++) {
      text += items[i]->getBranchLeafString(
        spacesCount, i, withDescription
      );
    }

    // add newline to every index except the last
    if (holder &&
      columnIndex < holder->items.size() - 1
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
    std::string &hookName1, std::string &hookName2,
    mt::CR_STR oriName1, mt::CR_STR oriName2
  ) {
    // copy name
    hookName1 = oriName1;

    if (Command::isTemporaryLetterCaseChange()) {
      mt_uti::StrTools::changeStringToUppercase(hookName1);
    }

    // copy input
    hookName2 = oriName2;
    Command::onFreeChangeInputLetterCase(hookName2);
  }

  void Command::changeTreeNamesToLowercase() {
    mt_uti::StrTools::changeStringToLowercase(name);

    for (Command *com : items) {
      com->changeTreeNamesToLowercase();
    }
  }

  void Command::changeTreeNamesToUppercase() {
    mt_uti::StrTools::changeStringToUppercase(name);

    for (Command *com : items) {
      com->changeTreeNamesToUppercase();
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
      comName, Color::BLACK, Color::WHITE
    );

    // has a newline at the end
    printDialogStatus();

    // once displayed
    if (!ultimate || (isSupporter() && getInheritanceFlag() == PARAMETER)) {
      Control::printHelp(isSupporter());
    }
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
}

#endif // __CLI_MENU__COMMAND_CPP__