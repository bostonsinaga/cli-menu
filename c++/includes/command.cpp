#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {
  using namespace mt_uti;

  //___________|
  // DASH TEST |
  //___________|

  bool DashTest::isSingle(mt::CR_STR str) {
    if (str.length() > 1 && str[0] == '-') {
      return true;
    }
    return false;
  }

  bool DashTest::isDouble(mt::CR_STR str) {
    if (str.length() > 2 &&
      str[0] == '-' && str[1] == '-'
    ) { return true; }
    return false;
  }

  bool DashTest::cleanSingle(std::string &str) {
    if (isSingle(str)) {
      str = str.substr(1);
      return true;
    }
    return false;
  }

  bool DashTest::cleanDouble(std::string &str) {
    if (isDouble(str)) {
      str = str.substr(2);
      return true;
    }
    return false;
  }

  //_________|
  // COMMAND |
  //_________|

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    const std::shared_ptr<CALLBACK> &callback_in
  ) {
    name = name_in;
    description = description_in;
    callback = callback_in;
    if (!Command::ultimate) Command::setUltimate(this);
  }

  Command::~Command() {
    name = "";
    description = "";
    tier = 0;
    callback.reset();
  }

  Command *Command::getItem(int index) {
    return VecTools<Command*>::getAt(items, index, nullptr);
  }

  Command *Command::getRoot() {
    Command *root = this;
    while (root->holder) root = root->holder;
    return root;
  }

  void Command::setItems(CR_VEC_COM newItems) {
    items = newItems;
    for (Command *com : items) com->setHolder(this, false);
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

  void Command::addItem(Command *com) {
    if (com) {
      com->setHolder(this, false);
      items.push_back(com);
    }
  }

  void Command::cleanItems() {
    for (Command *com : items) com->remove();
  }

  void Command::removeItem(Command *com) {
    if (!com) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == com) {
        items[i]->remove();
        VecTools<Command*>::cutSingle(items, i);
        break;
      }
    }
  }

  void Command::removeItem(int index) {
    if (VecTools<Command*>::hasIndex(items, index)) {
      items[i]->remove();
      VecTools<Command*>::cutSingle(items, index);
      break;
    }
  }

  void releaseItem(Command *com) {
    if (!com) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == com) {
        item[i]->holder = nullptr;
        VecTools<Command*>::cutSingle(items, i);
        break;
      }
    }
  }

  void releaseItem(int index) {
    if (VecTools<Command*>::hasIndex(items, index)) {
      item[i]->holder = nullptr;
      VecTools<Command*>::cutSingle(items, index);
      break;
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

  void Command::remove() {
    cleanItems();
    delete this;
  }

  Command *Command::ultimate = nullptr;

  void Command::setUltimate(Command *newUltimate) {
    ultimate = newUltimate;
  }

  bool Command::::cleanCapturedPositionalInputs(mt::VEC_STR &inputs) {
    if (this == Command::ultimate) {
      mt::VEC_INT usedIndexes;

      for (int i = 0; inputs.size(); i++) {
        if (inputs[i].length() == 0) {
          usedIndexes.push_back(i);
        }
      }

      VecTools<std::string>::cutIndexes(
        inputs, usedIndexes
      );

      return true;
    }
    return false;
  }

  void Command::deepPull(
    mt::CR_VEC_STR &TEXTS,
    mt::CR_VEC2_DBL &NUMBERS,
    mt::CR_VEC_BOL &CONDITIONS
  ) {
    for (Command *com : items) {
      com->pullData(TEXTS, NUMBERS, CONDITIONS);
    }
  }

  void Command::execute() {
    if (ultimate) {
      mt::CR_VEC_STR TEXTS;
      mt::CR_VEC_DBL NUMBERS;
      mt::CR_VEC_BOL CONDITIONS;

      ultimate->getRoot()->pullData(TEXTS, NUMBERS, CONDITIONS);
      (*ultimate->callback)(TEXTS, NUMBERS, CONDITIONS);
    }
  }

  //___________|
  // PARAMETER |
  //___________|

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    const std::shared_ptr<CALLBACK> &callback_in
  ):
  Command::Command(name_in, description_in, callback_in) {
    type = type_in;
  }

  bool Parameter::getType() { return type; }
  std::string Parameter::getArgument() { return argument; }

  std::string Parameter::getStringifiedType() {
    if (type == TEXT) return "TEXT";
    return "NUMBER";
  }

  void Parameter::pullData(
    mt::CR_VEC_STR &TEXTS,
    mt::CR_VEC2_DBL &NUMBERS,
    mt::CR_VEC_BOL &CONDITIONS
  ) {
    if (type == TEXT) {
      TEXTS.push_back(argument);
    }
    else {
      NUMBERS.push_back(
        Reader::parseNumbers<double>(argument)
      );
    }

    CONDITIONS.push_back(false);
    deepPull(TEXTS, NUMBERS, CONDITIONS);
  }

  bool Parameter::match(mt::VEC_STR &inputs) {

    bool incomplete = false;
    int begin = 0, end = inputs.size() - 1;

    if (tier <= Command::ultimate->tier) {
      begin = tier;
      end = tier + 1;
      if (begin >= inputs.size()) return false;
      else if (end >= inputs.size()) incomplete = true;
    }

    for (int i = begin; i < end; i++) {

      int j = i + 1;
      std::string testName = inputs[i];

      if (DashTest::cleanSingle(testName) &&
        testName == name
      ) {
        inputs[i] = "";

        if (!incomplete) {
          argument = inputs[j];
          inputs[j] = "";
        }

        if (!Command::cleanCapturedPositionalInputs(inputs)) {
          if (tier > Command::ultimate->tier) {
            VecTools<std::string>::cutInterval(inputs, i, j);
          }
        }

        return !incomplete;
      }
    }
    return false;
  }

  //________|
  // TOGGLE |
  //________|

  Toggle::Toggle(
    mt::CR_VEC_STR names_in,
    mt::CR_VEC_STR descriptions_in,
    const std::shared_ptr<CALLBACK> &callback_in
  ):
  Command(name_in, description_in, callback_in) {}

  void Toggle::pullData(
    mt::CR_VEC_STR &TEXTS,
    mt::CR_VEC2_DBL &NUMBERS,
    mt::CR_VEC_BOL &CONDITIONS
  ) {
    CONDITIONS.push_back(condition);
    TEXTS.push_back("");
    NUMBERS.push_back({});
    deepPull(TEXTS, NUMBERS, CONDITIONS);
  }

  bool Toggle::getCondition() { return condition; }

  bool Toggle::match(mt::VEC_STR &inputs) {
    int begin = 0, end = inputs.size();

    if (tier <= Command::ultimate->tier) {
      begin = tier;
      end = tier + 1;
      if (begin >= inputs.size()) return false;
    }

    for (int i = begin; i < end; i++) {
      std::string testName = inputs[i];

      if (DashTest::cleanDouble(testName) &&
        testName == name
      ) {
        condition = true;
        inputs[i] = "";

        if (!Command::cleanCapturedPositionalInputs(inputs)) {
          if (tier > Command::ultimate->tier) {
            VecTools<std::string>::cutSingle(inputs, i);
          }
        }

        return true;
      }
    }
    return false;
  }
}

#endif // __CLI_MENU__COMMAND_CPP__