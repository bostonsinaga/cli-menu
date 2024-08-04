#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include <memory>
#include "mini-tools.h"

namespace cli_menu {

  // callback format ('Parameters' arguments and 'Toggles' states)
  typedef std::function<void(mt::CR_VEC_STR, mt::CR_VEC_BOL)> CALLBACK;

  class Parameters {
  private:
    mt::VEC_STR names;
    mt::VEC_INT types;
    mt::VEC_BOL obligatories;
    mt::VEC_STR descriptions;
    mt::VEC_STR arguments;
    void balanceSize();

    ~Parameters() {
      names = {};
      types = {};
      obligatories = {};
      descriptions = {};
      arguments = {};
    }

  public:
    enum {TEXT, NUMBER};

    Parameters(
      mt::CR_VEC_STR names_in,
      mt::CR_VEC_INT types_in,
      mt::CR_VEC_BOL obligatories_in,
      mt::CR_VEC_STR descriptions_in
    );

    void remove() { delete this; }

    void setArguments(mt::CR_VEC_STR arguments_in);
    int amount();

    mt::VEC_STR getNames();
    mt::VEC_INT getTypes();
    mt::VEC_BOL getObligatories();
    mt::VEC_STR getDescriptions();
    mt::VEC_STR getArguments();

    std::string getName(int index);
    int getType(int index);
    bool getObligatory(int index);
    std::string getDescription(int index);
    std::string getArgument(int index);

    mt::VEC_STR getStringifiedTypes();
    std::string getStringifiedType(int index);
  };

  class Toggles {
  private:
    mt::VEC_STR names;
    mt::VEC_STR descriptions;
    mt::VEC_BOL states;
    void balanceSize();

    ~Toggles() {
      names = {};
      descriptions = {};
      states = {};
    }

  public:
    Toggles(
      mt::CR_VEC_STR names_in,
      mt::CR_VEC_STR descriptions_in
    );

    void remove() { delete this; }

    void setStates(mt::CR_VEC_BOL states_in);
    int amount();

    mt::VEC_STR getNames();
    mt::VEC_STR getDescriptions();
    mt::VEC_BOL getStates();

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
    std::shared_ptr<CALLBACK> callback;

    ~Command() {
      parameters->remove();
      toggles->remove();
      name = "";
      description = "";
      callback.reset();
    }

  public:
    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      Parameters *parameters_in,
      Toggles *toggles_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    void remove() { delete this; }

    std::string getName();
    std::string getDescription();
    void execute();

    Parameters *getParameters();
    Toggles *getToggles();
  };

  typedef std::vector<Command*> VEC_COM;
}

#endif // __CLI_MENU__COMMAND_H__