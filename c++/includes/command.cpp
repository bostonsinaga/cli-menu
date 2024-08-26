#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL required_in
  ) {
    name = name_in;
    description = description_in;
    callback = callback_in;
    required = required_in;
  }

  Command::~Command() {
    name = "";
    description = "";
    tier = 0;
    callback.reset();
  }

  Command *Command::getItem(mt::CR_INT index) {
    return mt_uti::VecTools<Command*>::getAt(items, index, nullptr);
  }

  Command *Command::getRoot() {
    Command *root = this;
    while (root->holder) {
      root = root->holder;
    }
    return root;
  }

  void Command::setItems(CR_VEC_COM newItems) {
    items = newItems;
    for (Command *com : items) {
      if (com) com->setHolder(this, false);
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
      items.push_back(command);
    }
  }

  void Command::cleanItems() {
    for (Command *com : items) {
      if (com) com->remove();
    }
  }

  void Command::removeItem(Command *command) {
    if (!command) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == command) {
        items[i]->remove();
        mt_uti::VecTools<Command*>::cutSingle(items, i);
        break;
      }
    }
  }

  void Command::removeItem(int index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
      items[index]->remove();
      mt_uti::VecTools<Command*>::cutSingle(items, index);
    }
  }

  void Command::releaseItem(Command *command) {
    if (!command) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == command) {
        items[i]->holder = nullptr;
        mt_uti::VecTools<Command*>::cutSingle(items, i);
        tier = 0;
        break;
      }
    }
  }

  void Command::releaseItem(int index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
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

  void Command::remove() {
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

  bool Command::isClassifier() {
    if (ultimate) return tier < ultimate->tier;
    return false;
  }

  void Command::spreadUltimateDown(Command *newUltimate) {
    ultimate = newUltimate;
    for (Command *com : items) {
      com->spreadUltimateDown(newUltimate);
    }
  }

  void Command::setAsUltimate() {
    ultimate = this;
    spreadUltimateDown(this);
  }

  void Command::run(ParamData &paramData) {
    if (callback) {
      (*callback)(
        paramData.texts,
        paramData.numbers,
        paramData.conditions
      );
    }
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

  bool Command::cleanCapturedPositionalInputs(mt::VEC_STR &inputs) {
    if (this == ultimate) {
      mt::VEC_INT usedIndexes;

      for (int i = 0; inputs.size(); i++) {
        if (inputs[i].length() == 0) {
          usedIndexes.push_back(i);
        }
      }

      mt_uti::VecTools<std::string>::cutIndexes(
        inputs, usedIndexes
      );

      return true;
    }
    return false;
  }
}

#endif // __CLI_MENU__COMMAND_CPP__