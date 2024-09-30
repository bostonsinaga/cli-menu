#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {
  //______________|
  // CONSTRUCTORS |
  //______________|

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

  Command::~Command() {
    name = "";
    description = "";
    tier = 0;
    items = {};
    requiredItems = {};
    holder = nullptr;
    next = nullptr;
    ultimate = nullptr;
    accumulating = false;
    required = false;
    used = false;
    callback.reset();
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
    for (Command *com : items) {
      if (com->name == name_in) return com;
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

      if (i > 0) {
        items[i-1]->next = items[i];

        // connect last index to the first
        if (i == items.size() - 1) {
          items[i]->next = items[0];
        }
      }
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

        Message::print(
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
      updateRequiredItems(command, true);
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
      tier = holder->tier + 1;
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
    if (index > 0 &&
      items.size() > index + 1
    ) {
      items[index - 1]->next = items[index + 1];
    }
  }

  Command* Command::dismantle(mt::CR_INT index) {
    Command *target = items[index];
    sewNext(index);
    updateRequiredItems(target, false);
    mt_uti::VecTools<Command*>::cutSingle(items, index);
    return target;
  }

  void Command::dismantleRemove(mt::CR_INT index) {
    dismantle(index)->remove(true);
  }

  Command* Command::dismantleRelease(mt::CR_INT index) {
    items[index]->tier = 0;
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
      com->tier = 0;
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
      return Message::customColored(
        Message::COLOR::GREEN, "(main)"
      );
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
    if (isUltimate() && command->isRequired()) {
      if (adding) {
        requiredItems.push_back(command);
      }
      else {
        mt_uti::VecTools<Command*>::cutSingle(
          requiredItems,
          mt_uti::VecTools<Command*>::getIndex(requiredItems, command)
        );
      }
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
    Command *last = nullptr;

    collapseUltimateItems(this, united);
    items = united;
    Command::cleanDuplicatesInItems();

    for (Command *com : united) {
      if (last) last->next = com;
      updateRequiredItems(com, true);
      com->holder = this;
      com->tier = tier + 1;
      last = com;
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

  void Command::deepPull(
    ParamData &paramData,
    mt::VEC_UI &usedIndexes
  ) {
    if (!usedIndexes.empty()) {
      const int index = usedIndexes[0];
      usedIndexes.erase(usedIndexes.begin());
      items[index]->pullData(paramData, usedIndexes);
    }
  }

  void Command::pullData(
    ParamData &paramData,
    mt::VEC_UI &usedIndexes
  ) {
    deepPull(paramData, usedIndexes);
  }

  //________|
  // DIALOG |
  //________|

  void Command::printDialogError(
    mt::CR_STR reason,
    mt::CR_STR suggestion
  ) {
    std::cerr << Message::customColored(
      Message::COLOR::RED,
      "\n> " + reason + ". " + suggestion + ":\n\n"
    );
  }

  /**
   * The 'command' confirmed to exist when this invoked.
   * This always invoked in supporter level.
   */
  mt::USI Command::chooseQuestion(Command *command) {
    if (command->next->used &&
      command->ultimate->getRequiredCount() == 0
    ) {
      return DIALOG::COMPLETE;
    }
    else if (command->getInheritanceFlag() == PARAMETER) {
      return command->openQuestion();
    }
    // PROGRAM or TOGGLE
    return command->closedQuestion();
  }

  // called after ultimate
  mt::USI Command::closedQuestion() {
    std::string buffer;

    Command::printAfterBoundaryLine(
      getFullNameWithUltimate(),
      false
    );

    while (std::getline(std::cin, buffer)) {
      mt_uti::StrTools::changeStringToLowercase(buffer);

      if (buffer == "y" || buffer == "yes" ||
        buffer == "1" || buffer == "true"
      ) {
        setData(true);
        Command::chooseQuestion(next);
      }
      else if (buffer == "n" || buffer == "no" ||
        buffer == "0" || buffer == "false" ||
        Control::nextTest(buffer)
      ) {
        setData(false);
        Command::chooseQuestion(next);
      }
      else if (Control::cancelTest(buffer)) {
        return DIALOG::CANCELED;
      }
      else if (Control::enterTest(buffer)) {
        if (getRequiredCount() == 0) {
          return DIALOG::COMPLETE;
        }
        else Command::printDialogError(
          "Cannot enter before all required parameters are met"
        );
      }
      else if (Control::selectTest(buffer)) {
        return dialog();
      }
      else Command::printDialogError(
        "Only accept boolean values"
      );
    }

    return DIALOG::CANCELED;
  }

  // called after ultimate
  mt::USI Command::openQuestion() {

    mt::VEC_STR strVec;
    std::string buffer;
    bool inputPassed;

    Command::printAfterBoundaryLine(
      getFullNameWithUltimate(),
      true
    );

    while (std::getline(std::cin, buffer)) {
      inputPassed = isOptional() || (isRequired() && !strVec.empty());

      if (Control::cancelTest(buffer)) {
        return DIALOG::CANCELED;
      }
      else if (Control::enterTest(buffer)) {
        if (getRequiredCount() == 0 && inputPassed) {
          return DIALOG::COMPLETE;
        }
        else Command::printDialogError(
          "Cannot enter before all required parameters are met"
        );
      }
      else if (Control::nextTest(buffer)) {
        if (inputPassed) {
          setData(mt_uti::StrTools::uniteVector(strVec, "\n"));
          return Command::chooseQuestion(next);
        }
        else Command::printDialogError(
          "Cannot skip with empty input on required parameter"
        );
      }
      else if (Control::selectTest(buffer)) {
        return dialog();
      }
      else strVec.push_back(buffer);
    }

    return DIALOG::CANCELED;
  }

  // available to all tiers (command selection)
  mt::USI Command::dialog() {

    std::string nameTest;
    static std::string inlineNames = "";

    if (inlineNames.empty() && holder) {
      inlineNames += holder->getInlineRootNames(" ", true);
    }

    inlineNames += " " + getFullName();
    Command::printAfterBoundaryLine(inlineNames, true);

    while (std::getline(std::cin, nameTest)) {

      if (Control::cancelTest(nameTest)) {
        return DIALOG::CANCELED;
      }
      else if (Control::enterTest(nameTest)) {
        Command::printDialogError(
          "Cannot enter before parameters"
        );
        continue;
      }
      else if (Control::nextTest(nameTest)) {
        if (next) {
          return next->dialog();
        }
        else if (getNumberOfItems() > 0) {
          return items[0]->dialog();
        }
        else { // group that has no items
          Command::printDialogError(
            "This is a dead end",
            "Please go back to the previous level or enter with this"
          );
          continue;
        }
      }

      Command *found;
      if (isGroup()) found = getItem(nameTest);
      else found = ultimate->getItem(nameTest);

      if (found) {
        if (found->isSupporter()) {
          return Command::chooseQuestion(found);
        }
        return found->dialog();
      }
      else Command::printDialogError("Command not found");
    }

    return DIALOG::CANCELED;
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

  void Command::printAfterBoundaryLine(
    mt::CR_STR comName,
    mt::CR_BOL isOpen
  ) {
    static bool init[] = {true, true};

    if (!(init[0] && init[1])) {
      Message::printBoundaryLine();
    }

    // bold font style
    std::cout << "\n\033[1m>" << comName << ":\033[0m\n";

    // display only once
    if (init[isOpen]) {
      init[isOpen] = false;

      // italic font style
      if (isOpen) {
        std::cout
          << "\033[3m(.cancel = :c, "
          << ".enter = :e, "
          << ".next = :n, "
          << ".previous = :p, "
          << ".select = :s)\033[0m\n";
      }
      // italic font style
      else std::cout
        << "\033[3m(yes = y, "
        << "no = n, "
        << "or boolean)\033[0m\n"; 
    }

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