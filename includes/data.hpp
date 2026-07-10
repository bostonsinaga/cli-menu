#ifndef __CLI_MENU__DATA_HPP__
#define __CLI_MENU__DATA_HPP__

#include "language.hpp"

namespace cli_menu {

  template <class T>
  class Data {
  public:
    mt::VEC<T> values;
    T latest();

    // stringified boolean is based on 'mt_uti::Booleanizer'
    virtual mt::VEC_STR stringify(mt::CR_STR separator = "\n");

    virtual void print(
      mt::CR<CONSOLE_CODE> consoleCode,
      mt::CR_SZ numberOfIndents
    );
  };
}

#include "data.tpp"
#endif // __CLI_MENU__DATA_HPP__