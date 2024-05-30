#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include <functional>
#include "util.h"

namespace namespace cli_menu {

  typedef std::vector<Command*> VEC_COM;

  // char, string, integer, float, object
  enum {CHA, STR, INT, FLO, OBJ};

  class Parameters {
  private:
    VEC_STR *names;
    VEC_INT *types;
    VEC_BOO *obligatories;
    VEC_STR *descriptions;

  public:
    Parameters(
      VEC_STR names_in,
      VEC_INT types_in,
      VEC_BOO obligatories_in,
      VEC_STR descriptions_in
    ) {
      names = &names_in;
      types = &types_in;
      obligatories = &obligatories_in;
      descriptions = &descriptions_in;
    }

    int amount() { return names->size(); }

    VEC_STR *getNames() { return names; }
    VEC_INT *getTypes() { return types; }
    VEC_BOO *getObligatories() { return obligatories; }
    VEC_STR *getDescriptions() { return descriptions; }

    std::string getName(int index) {
      return Util::getIndexOfVector<VEC_STR>(names, index, "");
    }

    int getType(int index) {
      return Util::getIndexOfVector<VEC_INT>(types, index, "");
    }

    bool getObligatory(int index) {
      return Util::getIndexOfVector<VEC_BOO>(obligatories, index, false);
    }

    std::string getDescription(int index) {
      return Util::getIndexOfVector<VEC_STR>(descriptions, index, "");
    }
  };

  class Toggles {
  private:
    VEC_STR *names;
    VEC_STR *descriptions;

  public:
    Toggles(
      VEC_STR names_in,
      VEC_STR descriptions_in
    ) {
      names = &names_in;
      descriptions = &descriptions_in;
    }

    int amount() { return names->size(); }

    VEC_STR *getNames() { return names; }
    VEC_STR *getDescriptions() { return descriptions; }

    std::string getName(int index) {
      return Util::getIndexOfVector<VEC_STR>(names, index, "");
    }

    std::string getDescription(int index) {
      return Util::getIndexOfVector<VEC_STR>(descriptions, index, "");
    }
  };

  class Command {
  private:
    std::string *name;
    std::string *description;
    Parameters *parameters;
    Toggles *toggles;
    std::function<VEC_STR, VEC_DOU, VEC_BOO> *callback;

  public:
    Command(
      std::string name_in,
      std::string description_in,
      Parameters *parameters_in,
      Toggles *toggles_in,
      std::function<VEC_STR, VEC_DOU, VEC_BOO> callback_in
    ) {
      name = &name_in;
      description = &description_in;
      parameters = parameters_in;
      toggles = toggles_in;
      callback = &callback_in;
    }

    std::string getName() { return *name; }
    std::string getDescription() { return *description; }
    void execute() { callback(); }

    Parameters *getParameters() { return parameters; }
    Toggles *getToggles() { return toggles; }
  };
}

#endif // __CLI_MENU__COMMAND_H__