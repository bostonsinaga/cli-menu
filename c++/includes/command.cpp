#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {

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

  std::string Command::getName() { return name; }
  std::string Command::getDescription() { return description; }
  Command *getHolder() { return holder; }
  VEC_COM Command::getItems() { return items; }

  Command *Command::getItem(int index) {
    return mt_uti::VecTools<Command*>::getAt(items, index, nullptr);
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
        mt_uti::cutSingle<Command*>(items, i);
        break;
      }
    }
  }

  void Command::removeItem(int index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
      items[i]->remove();
      mt_uti::VecTools<Command*>::cutSingle(items, index);
      break;
    }
  }

  void releaseItem(Command *com) {
    if (!com) return;

    for (int i = 0; i < items.size(); i++) {
      if (items[i] == com) {
        item[i]->holder = nullptr;
        mt_uti::cutSingle<Command*>(items, i);
        break;
      }
    }
  }

  void releaseItem(int index) {
    if (mt_uti::VecTools<Command*>::hasIndex(items, index)) {
      item[i]->holder = nullptr;
      mt_uti::VecTools<Command*>::cutSingle(items, index);
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

  bool Command::matchItems(mt::CR_STR test) {
    for (Command *com : items) {
      if (com->getName() == test) return true;
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

  void Command::pullData(
    mt::CR_VEC_STR &TEXTS,
    mt::CR_VEC_DBL &NUMBERS,
    mt::CR_VEC_BOL &CONDITIONS
  ) {
    for () {

    }
  }

  void Command::execute() {
    if (ultimate) {
      Command *root = ultimate->getHolder();
      mt::CR_VEC_STR TEXTS;
      mt::CR_VEC_DBL NUMBERS;
      mt::CR_VEC_BOL CONDITIONS;

      while (root->getHolder()) {
        root = root->getHolder();
      }

      pullData(TEXTS, NUMBERS, CONDITIONS);
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

  Parameter::~Parameter() {
    argument = "";
    type = false;
  }

  bool Parameter::getType() { return type; }
  std::string Parameter::getArgument() { return argument; }

  std::string Parameter::getStringifiedType() {
    if (type == TEXT) return "TEXT";
    return "NUMBER";
  }

  void Parameter::match(mt::VEC_STR &tests) {

  }

  //_________|
  // Toggles |
  //_________|

  Toggles::Toggles(
    mt::CR_VEC_STR names_in,
    mt::CR_VEC_STR descriptions_in
  ) {
    names = names_in;
    descriptions = descriptions_in;
    balanceSize();
  }

  void Toggles::balanceSize() {

    mt::VEC_ULLI differences = mt_uti::VecTools<mt::ULLI>::getDifferencesToSize({
      descriptions.size(), states.size()
    }, names.size());

    mt_uti::VecTools<std::string>::concat(
      descriptions, std::vector<std::string>(differences[0], "No description.")
    );

    mt_uti::VecTools<bool>::concat(
      states, std::vector<bool>(differences[1], false)
    );
  }

  void Toggles::setStates(mt::CR_VEC_BOL states_in) {
    states = states_in;
    balanceSize();
  }

  int Toggles::amount() { return names.size(); }

  mt::VEC_STR Toggles::getNames() { return names; }
  mt::VEC_STR Toggles::getDescriptions() { return descriptions; }
  mt::VEC_BOL Toggles::getStates() { return states; }

  std::string Toggles::getName(int index) {
    return mt_uti::VecTools<std::string>::getAt(names, index, "");
  }

  std::string Toggles::getDescription(int index) {
    return mt_uti::VecTools<std::string>::getAt(descriptions, index, "");
  }

  bool Toggles::getState(int index) {
    return mt_uti::VecTools<bool>::getAt(states, index, "");
  }
}

#endif // __CLI_MENU__COMMAND_CPP__