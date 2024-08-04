#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {

  //____________|
  // Parameters |
  //____________|

  Parameters::Parameters(
    mt::CR_VEC_STR names_in,
    mt::CR_VEC_INT types_in,
    mt::CR_VEC_BOL obligatories_in,
    mt::CR_VEC_STR descriptions_in
  ) {
    names = names_in;
    types = types_in;
    obligatories = obligatories_in;
    descriptions = descriptions_in;
    balanceSize();
  }

  void Parameters::balanceSize() {

    mt::VEC_ULLI differences = mt_uti::VecTools<mt::ULLI>::getDifferencesToSize({
      types.size(),
      obligatories.size(),
      descriptions.size(),
      arguments.size()
    }, names.size());

    mt_uti::VecTools<int>::concat(
      types, std::vector<int>(differences[0], NUMBER)
    );

    mt_uti::VecTools<bool>::concat(
      obligatories, std::vector<bool>(differences[1], false)
    );

    mt_uti::VecTools<std::string>::concat(
      descriptions, std::vector<std::string>(differences[2], "No description.")
    );

    mt_uti::VecTools<std::string>::concat(
      arguments, std::vector<std::string>(differences[3], "")
    );
  }

  void Parameters::setArguments(mt::CR_VEC_STR arguments_in) {
    arguments = arguments_in;
    balanceSize();
  }

  int Parameters::amount() { return names.size(); }

  mt::VEC_STR Parameters::getNames() { return names; }
  mt::VEC_INT Parameters::getTypes() { return types; }
  mt::VEC_BOL Parameters::getObligatories() { return obligatories; }
  mt::VEC_STR Parameters::getDescriptions() { return descriptions; }
  mt::VEC_STR Parameters::getArguments() { return arguments; }

  std::string Parameters::getName(int index) {
    return mt_uti::VecTools<std::string>::getAt(names, index, "");
  }

  int Parameters::getType(int index) {
    return mt_uti::VecTools<int>::getAt(types, index, NUMBER);
  }

  bool Parameters::getObligatory(int index) {
    return mt_uti::VecTools<bool>::getAt(obligatories, index, false);
  }

  std::string Parameters::getDescription(int index) {
    return mt_uti::VecTools<std::string>::getAt(descriptions, index, "");
  }

  std::string Parameters::getArgument(int index) {
    return mt_uti::VecTools<std::string>::getAt(arguments, index, "");
  }

  mt::VEC_STR Parameters::getStringifiedTypes() {
    mt::VEC_STR retTypes(types.size());

    for (int i = 0; i < retTypes.size(); i++) {
      if (types[i] == NUMBER) retTypes[i] = "NUMBER";
      else retTypes[i] = "TEXT";
    }

    return retTypes;
  }

  std::string Parameters::getStringifiedType(int index) {
    if (mt_uti::VecTools<int>::getAt(types, index, NUMBER) == NUMBER) {
      return "NUMBER";
    }
    return "TEXT";
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

  //_________|
  // Command |
  //_________|

  Command::Command(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    Parameters *parameters_in,
    Toggles *toggles_in,
    const std::shared_ptr<CALLBACK> &callback_in
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