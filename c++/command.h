#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "util.h"

namespace cli_menu {

  // integer, float, char, string, object
  enum {INT, FLO, CHA, STR, OBJ};

  // callback format ('Parameters' arguments and 'Toggles' states)
  typedef std::function<void(VEC_STR*, VEC_BOO*)> CLBK;

  class Parameters {
  private:
    VEC_STR names;
    VEC_INT types;
    VEC_BOO obligatories;
    VEC_STR descriptions;
    VEC_STR arguments;

    void balance();

  public:
    Parameters(
      VEC_STR &names_in,
      VEC_INT &types_in,
      VEC_BOO &obligatories_in,
      VEC_STR &descriptions_in
    );

    Parameters(
      const VEC_STR &names_in,
      const VEC_INT &types_in,
      const VEC_BOO &obligatories_in,
      const VEC_STR &descriptions_in
    );

    void setArguments(VEC_STR &arguments_in);
    void setArguments(const VEC_STR &arguments_in);
    int amount();

    VEC_STR *getNames();
    VEC_INT *getTypes();
    VEC_BOO *getObligatories();
    VEC_STR *getDescriptions();
    VEC_STR *getArguments();

    std::string getName(int index);
    int getType(int index);
    bool getObligatory(int index);
    std::string getDescription(int index);
    std::string getArgument(int index);
  };

  class Toggles {
  private:
    VEC_STR names;
    VEC_STR descriptions;
    VEC_BOO states;

    void balance();

  public:
    Toggles(
      VEC_STR &names_in,
      VEC_STR &descriptions_in
    );

    Toggles(
      const VEC_STR &names_in,
      const VEC_STR &descriptions_in
    );

    void setStates(VEC_BOO &states_in);
    void setStates(const VEC_BOO &states_in);
    int amount();

    VEC_STR *getNames();
    VEC_STR *getDescriptions();
    VEC_BOO *getStates();

    std::string getName(int index);
    std::string getDescription(int index);
    bool getState(int index);
  };

  class Command {
  private:
    std::string name;
    std::string description;
    Parameters *parameters;
    Toggles *toggles;
    CLBK *callback;

  public:
    Command(
      std::string name_in,
      std::string description_in,
      Parameters *parameters_in,
      Toggles *toggles_in,
      CLBK *callback_in
    );

    std::string getName();
    std::string getDescription();
    void execute();

    Parameters *getParameters();
    Toggles *getToggles();
  };

  typedef std::vector<Command*> VEC_COM;
}

#endif // __CLI_MENU__COMMAND_H__