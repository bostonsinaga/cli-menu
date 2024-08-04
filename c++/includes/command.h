#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include <string>
#include <vector>
#include <functional>
#include "mini-tools.h"

namespace cli_menu {

  // callback format ('Parameters' arguments and 'Toggles' states)
  typedef std::function<void(mt::VEC_STR*, mt::VEC_BOL*)> CLBK;

  class Parameters {
  private:
    mt::VEC_STR names;
    mt::VEC_INT types;
    mt::VEC_BOL obligatories;
    mt::VEC_STR descriptions;
    mt::VEC_STR arguments;

    void balance();

  public:
    Parameters(
      mt::VEC_STR &names_in,
      mt::VEC_INT &types_in,
      mt::VEC_BOL &obligatories_in,
      mt::VEC_STR &descriptions_in
    );

    Parameters(
      const mt::VEC_STR &names_in,
      const mt::VEC_INT &types_in,
      const mt::VEC_BOL &obligatories_in,
      const mt::VEC_STR &descriptions_in
    );

    void setArguments(mt::VEC_STR &arguments_in);
    void setArguments(const mt::VEC_STR &arguments_in);
    int amount();

    mt::VEC_STR *getNames();
    mt::VEC_INT *getTypes();
    mt::VEC_BOL *getObligatories();
    mt::VEC_STR *getDescriptions();
    mt::VEC_STR *getArguments();

    std::string getName(int index);
    int getType(int index);
    bool getObligatory(int index);
    std::string getDescription(int index);
    std::string getArgument(int index);
  };

  class Toggles {
  private:
    mt::VEC_STR names;
    mt::VEC_STR descriptions;
    mt::VEC_BOL states;

    void balance();

  public:
    Toggles(
      mt::VEC_STR &names_in,
      mt::VEC_STR &descriptions_in
    );

    Toggles(
      const mt::VEC_STR &names_in,
      const mt::VEC_STR &descriptions_in
    );

    void setStates(mt::VEC_BOL &states_in);
    void setStates(const mt::VEC_BOL &states_in);
    int amount();

    mt::VEC_STR *getNames();
    mt::VEC_STR *getDescriptions();
    mt::VEC_BOL *getStates();

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