#ifndef __CLI_MENU__COMMAND_CPP__
#define __CLI_MENU__COMMAND_CPP__

#include "command.h"

namespace cli_menu {

  //____________|
  // Parameters |
  //____________|

  Parameters::Parameters(
    VEC_STR names_in,
    VEC_INT types_in,
    VEC_BOO obligatories_in,
    VEC_STR descriptions_in,
    VEC_STR arguments_in
  ) {
    names = &names_in;
    types = &types_in;
    obligatories = &obligatories_in;
    descriptions = &descriptions_in;
    arguments = &arguments_in;

    std::vector<LLUI> differences = Util::getDifferencesToSize({
      types->size(),
      obligatories->size(),
      descriptions->size(),
      arguments->size()
    }, names->size());

    for (int i = 0; i < differences[0]; i++) { types->push_back(INT); }
    for (int i = 0; i < differences[1]; i++) { obligatories->push_back(false); }
    for (int i = 0; i < differences[2]; i++) { descriptions->push_back("No description."); }
    for (int i = 0; i < differences[3]; i++) { arguments->push_back(""); }
  }

  int Parameters::amount() { return names->size(); }

  VEC_STR *Parameters::getNames() { return names; }
  VEC_INT *Parameters::getTypes() { return types; }
  VEC_BOO *Parameters::getObligatories() { return obligatories; }
  VEC_STR *Parameters::getDescriptions() { return descriptions; }
  VEC_STR *Parameters::getArguments() { return arguments; }

  std::string Parameters::getName(int index) {
    return Util::getIndexOfVector<std::string>(names, index, "");
  }

  int Parameters::getType(int index) {
    return Util::getIndexOfVector<int>(types, index, 0);
  }

  bool Parameters::getObligatory(int index) {
    return Util::getIndexOfVector<bool>(obligatories, index, false);
  }

  std::string Parameters::getDescription(int index) {
    return Util::getIndexOfVector<std::string>(descriptions, index, "");
  }

  std::string Parameters::getArgument(int index) {
    return Util::getIndexOfVector<std::string>(arguments, index, "");
  }

  //_________|
  // Toggles |
  //_________|

  Toggles::Toggles(
    VEC_STR names_in,
    VEC_STR descriptions_in,
    VEC_BOO states_in
  ) {
    names = &names_in;
    descriptions = &descriptions_in;
    states = &states_in;

    std::vector<LLUI> differences = Util::getDifferencesToSize({
      descriptions->size(), states->size()
    }, names->size());

    for (int i = 0; i < differences[0]; i++) { descriptions->push_back("No description."); }
    for (int i = 0; i < differences[1]; i++) { states->push_back(false); }
  }

  int Toggles::amount() { return names->size(); }

  VEC_STR *Toggles::getNames() { return names; }
  VEC_STR *Toggles::getDescriptions() { return descriptions; }
  VEC_BOO *Toggles::getStates() { return states; }

  std::string Toggles::getName(int index) {
    return Util::getIndexOfVector<std::string>(names, index, "");
  }

  std::string Toggles::getDescription(int index) {
    return Util::getIndexOfVector<std::string>(descriptions, index, "");
  }

  bool Toggles::getState(int index) {
    return Util::getIndexOfVector<bool>(states, index, "");
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
    name = &name_in;
    description = &description_in;
    parameters = parameters_in;
    toggles = toggles_in;
    callback = callback_in;
  }

  std::string Command::getName() { return *name; }
  std::string Command::getDescription() { return *description; }

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