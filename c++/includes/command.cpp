#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {

  void Command::setMetaData(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in
  ) {
    name = name_in;
    description = description_in;
    required = required_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL required_in
  ) {
    setMetaData(name_in, description_in, required_in);
    callback = callback_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    mt::CR_BOL required_in
  ) {
    setMetaData(name_in, description_in, required_in);
    plainCallback = callback_in;
  }

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in
  ) {
    setMetaData(name_in, description_in, required_in);
  }

  Command::~Command() {
    name = "";
    description = "";
    tier = 0;
    items = {};
    next = nullptr;
    holder = nullptr;
    ultimate = nullptr;
    callback.reset();
  }

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

    for (int i = 0; i < items.size(); i++) {
      items[i]->setHolder(this, false);
      if (i > 0) items[i-1]->next = items[i];
    }
  }

  VEC_COM Command::setItemsRelease(CR_VEC_COM newItems) {
    VEC_COM oldItems = items;
    setItems(newItems);
    return oldItems;
  }

  void Command::setItemsReplace(CR_VEC_COM newItems) {
    cleanItems();
    setItems(newItems);
  }

  void Command::addItem(Command *command) {
    if (command) {
      command->setHolder(this, false);
      Command *last = items.back();
      items.push_back(command);
      last->next = command;
    }
  }

  void Command::cleanItems() {
    for (Command *com : items) {
      com->remove(false);
    }
  }

  void Command::sewNext(CR_INT index) {
    if (index > 0 &&
      items.size() > index + 1
    ) {
      items[index - 1]->next = items[index + 1];
    }
  }

  void Command::removeItem(Command *command) {
    if (!command) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == command) {
        sewNext(i);
        items[i]->remove();
        mt_uti::VecTools<Command*>::cutSingle(items, i);
        break;
      }
    }
  }

  void Command::removeItem(int index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
      sewNext(index);
      items[index]->remove();
      mt_uti::VecTools<Command*>::cutSingle(items, index);
    }
  }

  void Command::releaseItem(Command *command) {
    if (!command) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == command) {
        sewNext(i);
        items[i]->holder = nullptr;
        mt_uti::VecTools<Command*>::cutSingle(items, i);
        tier = 0;
        break;
      }
    }
  }

  void Command::releaseItem(int index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
      sewNext(index);
      items[index]->holder = nullptr;
      mt_uti::VecTools<Command*>::cutSingle(items, index);
      tier = 0;
    }
  }

  void Command::setHolder(Command *newHolder, bool addBack) {
    if (newHolder) {
      if (holder) holder->releaseItem(this);
      if (addBack) newHolder->addItem(this);
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

  bool Command::isRequired() {
    if (ultimate) return tier <= ultimate->tier || required;
    return false;
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

  bool Command::isOptional() {
    return !isRequired();
  }

  void Command::spreadUltimateDown(Command *newUltimate) {
    ultimate = newUltimate;
    for (Command *com : items) {
      com->spreadUltimateDown(newUltimate);
    }
  }

  VEC_STR Command::getTreeNamesVector(
    mt::CR_BOL fully,
    mt::CR_BOL withThis
  ) {
    VEC_STR names;
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

  void Command::setCallback(CR_SP_CALLBACK callback_in) {
    callback.reset();
    callback = callback_in;
  }

  void Command::setCallback(CR_SP_PLAIN_CALLBACK callback_in) {
    plainCallback.reset();
    plainCallback = callback_in;
  }

  void Command::setAsUltimate() {
    ultimate = this;
    spreadUltimateDown(this);
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

  void Command::printHelp(bool isClosed) {
    if (isClosed) {
      static bool isInit = true;

      if (isInit) {
        std::cout << "(cancel = ':q', next = ':w', enter = ':e')\n";
        isInit = false;
      }
    }
    else {
      static bool isInit = true;

      if (isInit) {
        std::cout << "(Yes = 'Y', No = 'N')\n";
        isInit = false;
      }
    }

    std::cout << std::endl;
  }

  // with and after ultimate (supporters)
  mt::USI Command::chooseQuestion(Command *com) {
    if (com) {
      if (com->getInheritanceFlag() == PARAMETER) {
        return com->openQuestion();
      }
      return com->closedQuestion();
    }
    return DIALOG_FLAG.COMPLETE;
  }

  // with and after ultimate (supporters)
  mt::USI Command::closedQuestion() {
    std::string buffer;

    std::cout << "\n\n" << getFullName() << ":\n";
    Command::printHelp(true);

    while (std::getline(std::cin, buffer)) {

      StrTools::changeStringToLowercase(buffer);
      bool willBreak = false;

      if (buffer == "y" || "yes") {
        setData(true);
        willBreak = true;
      }
      else if (buffer == "n" || "no") {
        setData(false);
        willBreak = true;
      }

      if (willBreak) return Command::chooseQuestion(next);
    }
  }

  // with and after ultimate (supporters)
  mt::USI Toggle::openQuestion() {
    mt::VEC_STR strVec;
    std::string buffer;

    std::cout << "\n\n" << getFullName() << ":\n";
    Command::printHelp(false);

    while (std::getline(std::cin, buffer)) {
      bool isEnter = isOptional() && buffer == ":e";

      if (buffer == ":q") return DIALOG_FLAG.CANCELED;
      else if (buffer == ":w") {
        setData(mt_uti::StrTools::uniteVector(strVec, "\n"));
        return Command::chooseQuestion(next);
      }
      else if (isEnter) {
        return DIALOG_FLAG.COMPLETE
      }

      strVec.push_back(buffer);
    }
  }

  // before ultimate (groups)
  mt::USI Command::select() {
    std::string nameTest;
    static std::string treeNames = getTreeNames(" ", true);

    treeNames += getFullName() + " ";
    std::cout << "\n\n" << treeNames << ":\n\n";

    while (std::getline(std::cin, nameTest)) {

      if (nameTest == ":q") return DIALOG_FLAG.CANCELED;
      else if (nameTest == ":w") {
        std::cout << "Cannot use ':w' before ultimate command. Try Again:\n\n";
      }
      else if (nameTest == ":e") {
        std::cout << "Cannot use ':e' before optional argument. Try Again:\n\n";
      }

      Command *next = getItem(nameTest);

      if (next) {
        if (next->isUltimate() || next->isSupporter()) {
          return Command::chooseQuestion(next);
        }
        return next->select();
      }
      else {
        std::cout << "\n\nCommand not found. Try Again:\n";
        Command::printHelp(false);
      }
    }
  }

  mt::USI Command::dialog() {
    if (isGroup() || isUltimate()) {
      return select();
    }
    return Command::chooseQuestion(this);
  }
}

#endif // __CLI_MENU__COMMAND_CPP__