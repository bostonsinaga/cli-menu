#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {
  //_________|
  // CONTROL |
  //_________|

  const std::string
    Control::CANCEL = ":c",
    Control::ENTER = ":e",
    Control::NEXT = ":n";

  int Control::test(std::string &str) {
    mt_uti::StrTools::changeStringToLowercase(str);

    if (str == Control::CANCEL) return 1;
    else if (str == Control::ENTER) return 2;
    else if (str == Control::NEXT) return 3;
    return 0;
  }

  //________________________|
  // COMMAND - CONSTRUCTORS |
  //________________________|

  void Command::setMetaData(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    Command *holder_in,
    mt::CR_BOL required_in
  ) {
    name = name_in;
    description = description_in;
    required = required_in;
    setHolder(holder_in);
    setBoundaryLine(40);
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in
  ) {
    setMetaData(name_in, description_in, holder_in, required_in);
    callback = callback_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in
  ) {
    setMetaData(name_in, description_in, holder_in, required_in);
    plainCallback = callback_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    Command *holder_in,
    mt::CR_BOL required_in
  ) {
    setMetaData(name_in, description_in, holder_in, required_in);
  }

  Command::~Command() {
    name = "";
    description = "";
    tier = 0;
    items = {};
    requiredItems = {};
    next = nullptr;
    holder = nullptr;
    ultimate = nullptr;
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
      if (com->getName() == name_in) return true;
    }
    return false;
  }

  Command *Command::getItem(mt::CR_INT index) {
    return mt_uti::VecTools<Command*>::getAt(items, index, nullptr);
  }

  Command *Command::getItem(mt::CR_STR name_in) {
    for (Command *com : items) {
      if (com->getName() == name_in) return com;
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

  void Command::setItems(CR_VEC_COM newItems) {
    items = {};

    for (Command *com : newItems) {
      if (com) items.push_back(com);
    }

    cleanDuplicatesInItems();

    for (int i = 0; i < items.size(); i++) {
      items[i]->setHolder(this, false);
      if (i > 0) items[i-1]->next = items[i];
    }
  }

  VEC_COM Command::setItemsRelease(CR_VEC_COM newItems) {
    if (!isSupporter()) {
      VEC_COM oldItems = items;
      setItems(newItems);
      return oldItems;
    }
    return {};
  }

  void Command::setItemsReplace(CR_VEC_COM newItems) {
    if (!isSupporter()) {
      cleanItems();
      setItems(newItems);
    }
  }

  void Command::addItem(Command *command) {

    if (command && !isSupporter()) {
      command->setHolder(this, false);

      if (!items.empty()) {
        items.back()->next = command;
      }

      items.push_back(command);
      cleanDuplicateToLastAdded(command);
      updateRequiredItems(command, true);
    }
  }

  void Command::cleanDuplicatesInItems() {
    std::tuple<VEC_COM, VEC_COM>
      wastedTuple = mt_uti::VecTools<Command*>::cleanDuplicateInside(
        items, false,
        [](Command *rep, Command *com)->bool {
          if (rep->getName() == com->getName()) return true;
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
          if (rep->getName() == com->getName()) return true;
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

  void Command::setHolder(Command *newHolder, bool addBack) {
    if (newHolder) {
      if (holder) holder->releaseItem(this);
      if (addBack) newHolder->addItem(this);
      if (newHolder->isUltimate()) ultimate = newHolder;
      holder = newHolder;
      tier = holder->tier + 1;
    }
  }

  void Command::remove(mt::CR_BOL firstOccurrence) {
    if (firstOccurrence && holder) {
      holder->releaseItem(this);
    }
    cleanItems();
    delete this;
  }

  bool Command::isUltimate() {
    return this == ultimate;
  }

  bool Command::isGroup() {
    return !items.empty() && !ultimate;
  }

  bool Command::isSupporter() {
    if (ultimate) return tier > ultimate->tier;
    return false;
  }

  bool Command::isRequired() {
    return isGroup() || required;
  }

  bool Command::isOptional() {
    return !isRequired();
  }

  mt::VEC_STR Command::getTreeNamesVector(
    mt::CR_BOL fully,
    mt::CR_BOL withThis
  ) {
    mt::VEC_STR names;
    Command *root = this;

    std::function<void()> chooseName = [&]() {
      if (fully) names.push_back(root->getFullName());
      else names.push_back(root->name);
    };

    if (withThis) chooseName();

    while (root->holder) {
      root = root->holder;
      chooseName();
    }

    return names;
  }

  std::string Command::getTreeNames(
    mt::CR_STR separator,
    mt::CR_BOL fully,
    mt::CR_BOL withThis
  ) {
    return mt_uti::StrTools::uniteVector(
      getTreeNamesVector(fully, withThis),
      separator
    );
  }

  //__________|
  // ULTIMATE |
  //__________|

  int Command::getRequiredCount() {
    if (ultimate) return ultimate->requiredItems.size();
    return 0;
  }

  void Command::updateRequiredItems(Command *command, mt::CR_BOL adding) {
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
      for (Command *com : items) {
        com->ultimate = nullptr;
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

  void Command::printHelp(mt::CR_BOL idx) {
    static bool init[] = {true, true};

    static const std::string abbreviations[] = {
      "\033[3m(cancel = :c, enter = :e, next = :n, previous = :p)\033[0m\n",
      "\033[3m(yes = y, no = n, or boolean)\033[0m\n"
    };

    if (init[idx]) {
      init[idx] = false;
      std::cout << abbreviations[idx];
    }

    std::cout << std::endl;
  }

  void Command::printTryAgain(mt::CR_STR about) {
    std::cout << "\n\033[31m> " << about << ". Try Again:\033[0m\n\n";
  }

  // called after ultimate
  mt::USI Command::closedQuestion() {

    bool willBreak;
    std::string buffer;
    printAfterBoundaryLine(getFullName(), true);

    while (std::getline(std::cin, buffer)) {

      mt_uti::StrTools::changeStringToLowercase(buffer);
      willBreak = false;

      if (buffer == "y" || buffer == "yes" ||
        buffer == "1" || buffer == "true"
      ) {
        setData(true);
        willBreak = true;
      }
      else if (buffer == "n" || buffer == "no" ||
        buffer == "0" || buffer == "false" ||
        Control::nextTest(buffer)
      ) {
        setData(false);
        willBreak = true;
      }
      else if (Control::cancelTest(buffer)) {
        return DIALOG_FLAG.CANCELED;
      }
      else if (Control::enterTest(buffer)) {
        if (getRequiredCount() == 0) {
          return DIALOG_FLAG.COMPLETE;
        }
        else Command::printTryAgain("Cannot enter before all required parameters are met");
      }
      else Command::printTryAgain("Only accept boolean values");

      if (willBreak) return dialog();
    }

    return DIALOG_FLAG.CANCELED;
  }

  // called after ultimate
  mt::USI Command::openQuestion() {

    mt::VEC_STR strVec;
    std::string buffer;
    bool inputPassed;

    printAfterBoundaryLine(getFullName(), false);

    while (std::getline(std::cin, buffer)) {
      inputPassed = isOptional() || (isRequired() && !strVec.empty());

      if (Control::cancelTest(buffer)) {
        return DIALOG_FLAG.CANCELED;
      }
      else if (Control::enterTest(buffer)) {
        if (getRequiredCount() == 0 && inputPassed) {
          return DIALOG_FLAG.COMPLETE;
        }
        else Command::printTryAgain("Cannot enter before all required parameters are met");
      }
      else if (Control::nextTest(buffer)) {
        if (inputPassed) {
          setData(mt_uti::StrTools::uniteVector(strVec, "\n"));
          return dialog();
        }
        else Command::printTryAgain("Cannot skip with empty input on required parameter");
      }
      else strVec.push_back(buffer);
    }

    return DIALOG_FLAG.CANCELED;
  }

  // selection of question, group, or ultimate
  mt::USI Command::dialog(mt::CR_BOL doneNullptr) {
    std::string nameTest;

    static std::string treeNames = getTreeNames(" ", true);
    treeNames += " " + getFullName();

    if (isUltimate()) {
      treeNames += "\033[32m (main)\033[0m";
    }

    printAfterBoundaryLine(treeNames, false);

    while (std::getline(std::cin, nameTest)) {

      if (Control::cancelTest(nameTest)) {
        return DIALOG_FLAG.CANCELED;
      }
      else if (Control::enterTest(nameTest)) {
        Command::printTryAgain("Cannot enter before parameters");
        continue;
      }
      else if (Control::nextTest(nameTest)) {
        Command::printTryAgain("Cannot skip before selecting group or command");
        continue;
      }

      Command *found = getItem(nameTest);

      if (found) {
        if (found->isSupporter()) {
          // choose question
          if (found->getInheritanceFlag() == PARAMETER) {
            return found->openQuestion();
          }
          return found->closedQuestion();
        }
        return found->dialog();
      }
      // called from question gives 'true'
      else if (doneNullptr) return DIALOG_FLAG.COMPLETE;
      else Command::printTryAgain("Command not found");
    }

    return DIALOG_FLAG.CANCELED;
  }

  std::string Command::boundaryLine = "";

  void Command::setBoundaryLine(mt::CR_SI size) {
    if (size < 0) boundaryLine = "";
    else boundaryLine = std::string(size, '-');
  }

  void Command::printBoundaryLine() {
    static bool isActive = false;
    if (isActive) std::cout << boundaryLine;
    else isActive = true;
  }

  void Command::printAfterBoundaryLine(
    mt::CR_STR comName,
    mt::CR_BOL idx
  ) {
    printBoundaryLine();
    std::cout << "\n\033[1m>" << comName << ":\033[0m\n";
    Command::printHelp(idx);
  }
}

#endif // __CLI_MENU__COMMAND_CPP__