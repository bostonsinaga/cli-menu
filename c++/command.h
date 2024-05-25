#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include <functional>
#include "util.h"

namespace namespace cli_menu {

  class Command {
  private:
    std::string *name;
    std::string *description;
    VEC_STR *parameters;
    VEC_STR *toggles;
    std::function<VEC_STR, VEC_BOL, VEC_INT> *callback;

  public:
    Command(
      std::string name_in,
      std::string description_in,
      VEC_STR parameters_in,
      VEC_STR toggles_in,
      std::function<VEC_STR, VEC_BOL, VEC_INT> callback_in
    ) {
      name = &name_in;
      description = &description_in;
      parameters = &parameters_in;
      toggles = &toggles_in;
      callback = &callback_in;
    }

    std::string getName() { return *name; }
    VEC_STR *getParameters() { return parameters; }
    VEC_STR *getToggles() { return toggles; }
    std::string getDescription() { return *description; }
    void execute() { callback(); }
  };
}

#endif // __CLI_MENU__COMMAND_H__