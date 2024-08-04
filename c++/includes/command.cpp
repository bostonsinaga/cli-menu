#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {

  //____________|
  // Parameters |
  //____________|

  Parameters::Parameters(
    mt::VEC_STR &names_in,
    mt::VEC_INT &types_in,
    mt::VEC_BOL &obligatories_in,
    mt::VEC_STR &descriptions_in
  ) {
    names = names_in;
    types = types_in;
    obligatories = obligatories_in;
    descriptions = descriptions_in;
    balance();
  }

  Parameters::Parameters(
    const mt::VEC_STR &names_in,
    const mt::VEC_INT &types_in,
    const mt::VEC_BOL &obligatories_in,
    const mt::VEC_STR &descriptions_in
  ) {
    names = names_in;
    types = types_in;
    obligatories = obligatories_in;
    descriptions = descriptions_in;
    balance();
  }
  
  void Parameters::balance() {
    std::vector<mt::ULLI> differences = mt_uti::StrTools::getDifferencesToSize({
      types.size(),
      obligatories.size(),
      descriptions.size(),
      arguments.size()
    }, names.size());

    for (int i = 0; i < differences[0]; i++) { types.push_back(INT); }
    for (int i = 0; i < differences[1]; i++) { obligatories.push_back(false); }
    for (int i = 0; i < differences[2]; i++) { descriptions.push_back("No description."); }
    for (int i = 0; i < differences[3]; i++) { arguments.push_back(""); }
  }

  void Parameters::setArguments(mt::VEC_STR &arguments_in) {
    arguments = arguments_in;
    balance();
  }

  void Parameters::setArguments(const mt::VEC_STR &arguments_in) {
    arguments = arguments_in;
    balance();
  }

  int Parameters::amount() { return names.size(); }

  mt::VEC_STR *Parameters::getNames() { return &names; }
  mt::VEC_INT *Parameters::getTypes() { return &types; }
  mt::VEC_BOL *Parameters::getObligatories() { return &obligatories; }
  mt::VEC_STR *Parameters::getDescriptions() { return &descriptions; }
  mt::VEC_STR *Parameters::getArguments() { return &arguments; }

  std::string Parameters::getName(int index) {
    return mt_uti::VecTools::getAt<std::string>(&names, index, "");
  }

  int Parameters::getType(int index) {
    return mt_uti::VecTools::getAt<int>(&types, index, 0);
  }

  bool Parameters::getObligatory(int index) {
    return mt_uti::VecTools::getAt<bool>(&obligatories, index, false);
  }

  std::string Parameters::getDescription(int index) {
    return mt_uti::VecTools::getAt<std::string>(&descriptions, index, "");
  }

  std::string Parameters::getArgument(int index) {
    return mt_uti::VecTools::getAt<std::string>(&arguments, index, "");
  }

  //_________|
  // Toggles |
  //_________|

  Toggles::Toggles(
    mt::VEC_STR &names_in,
    mt::VEC_STR &descriptions_in
  ) {
    names = names_in;
    descriptions = descriptions_in;
    balance();
  }

  Toggles::Toggles(
    const mt::VEC_STR &names_in,
    const mt::VEC_STR &descriptions_in
  ) {
    names = names_in;
    descriptions = descriptions_in;
    balance();
  }

  void Toggles::balance() {
    std::vector<mt::ULLI> differences = mt_uti::StrTools::getDifferencesToSize({
      descriptions.size(), states.size()
    }, names.size());

    for (int i = 0; i < differences[0]; i++) { descriptions.push_back("No description."); }
    for (int i = 0; i < differences[1]; i++) { states.push_back(false); }
  }

  void Toggles::setStates(mt::VEC_BOL &states_in) {
    states = states_in;
    balance();
  }

  void Toggles::setStates(const mt::VEC_BOL &states_in) {
    states = states_in;
    balance();
  }

  int Toggles::amount() { return names.size(); }

  mt::VEC_STR *Toggles::getNames() { return &names; }
  mt::VEC_STR *Toggles::getDescriptions() { return &descriptions; }
  mt::VEC_BOL *Toggles::getStates() { return &states; }

  std::string Toggles::getName(int index) {
    return mt_uti::VecTools::getAt<std::string>(&names, index, "");
  }

  std::string Toggles::getDescription(int index) {
    return mt_uti::VecTools::getAt<std::string>(&descriptions, index, "");
  }

  bool Toggles::getState(int index) {
    return mt_uti::VecTools::getAt<bool>(&states, index, "");
  }

  //_________|
  // Command |
  //_________|

  Command::Command(
    std::string name_in,
    std::string description_in,
    Parameters *parameters_in,
    Toggles *toggles_in,
    CLBK *callback_in
  ) {
    name = name_in;
    description = description_in;
    parameters = parameters_in;
    toggles = toggles_in;
    callback = callback_in;
  }

  std::string Command::getName() { return name; }
  std::string Command::getDescription() { return description; }

  void Command::execute() {
    (*callback)(
      parameters->getArguments(),
      toggles->getStates()
    );
  }

  Parameters *Command::getParameters() { return parameters; }
  Toggles *Command::getToggles() { return toggles; }
}

#endif // __CLI_MENU__COMMAND_CPP__