#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle {
  private:
    bool condition;
    ~Toggle() { condition = false; }

    void pullData(
      mt::CR_VEC_STR &TEXTS,
      mt::CR_VEC_DBL &NUMBERS,
      mt::CR_VEC_BOL &CONDITIONS
    );

  public:
    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    bool getCondition();
    bool match(mt::VEC_STR &inputs);
    int getInheritanceFlag() { return TOGGLE; }
  };
}

#define __CLI_MENU__TOGGLE_H__