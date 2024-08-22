#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"
#include "dash-test.h"

namespace cli_menu {
  using namespace mt_uti;

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    const std::shared_ptr<CALLBACK> &callback_in
  ) {
    name = name_in;
    description = description_in;
    callback = callback_in;
  }

  Command::~Command() {
    name = "";
    description = "";
    tier = 0;
    callback.reset();
  }

  Command *Command::getItem(CR_INT index) {
    return VecTools<Command*>::getAt(items, index, nullptr);
  }

  Command *Command::getProgram() {
    Command *program = this;
    while (program->holder) {
      program = program->holder;
    }
    return program;
  }

  Command *Command::getRoot() {
    Command *root = this;
    while (root->holder != Command::program) {
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

  void Command::addItem(Command *com) {
    if (com) {
      com->setHolder(this, false);
      items.push_back(com);
    }
  }

  void Command::cleanItems() {
    for (Command *com : items) {
      if (com) com->remove();
    }
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

  bool Command::isRequired() {
    return tier <= Command::ultimate->tier;
  }

  Command *Command::program = nullptr;
  Command *Command::ultimate = nullptr;

  void Command::setProgram(Command *newProgram) {
    program = newProgram->getProgram();
  }

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
      if (com) com->pullData(TEXTS, NUMBERS, CONDITIONS);
    }
  }

  void Command::execute() {
    if (ultimate) {
      if (!program) Command::setProgram(ultimate);

      mt::CR_VEC_STR TEXTS;
      mt::CR_VEC_DBL NUMBERS;
      mt::CR_VEC_BOL CONDITIONS;

      ultimate->getRoot()->pullData(TEXTS, NUMBERS, CONDITIONS);
      (*ultimate->callback)(TEXTS, NUMBERS, CONDITIONS);
    }
    else {
      if (!program) {
        Command::setProgram(ultimate);
      }

      ultimate = getRoot();
      Command::execute();
    }
  }
}

#endif // __CLI_MENU__COMMAND_CPP__