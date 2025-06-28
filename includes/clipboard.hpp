#ifndef __CLI_MENU__UTILS_HPP__
#define __CLI_MENU__UTILS_HPP__

#include "console.hpp"

namespace cli_menu {

  /** ONLY FOR STRING COPY! */
  class Clipboard {
  private:
    static void pasteText(std::string &dataRef);
    static void pasteNumbers(mt::VEC_LD &numbersRef);
    static void pasteConditions(mt::VEC_BOL &conditionsRef);

    Clipboard() = delete;
    friend class Command;
  };

  class HyphensDetector {
  private:
    VEC_STR singleKeywords, doubleKeywords;
    HyphensDetector() = delete;

    /**
     * The 'raws' is expected to be obtained
     * from the main function parameters.
     */
    HyphensDetector(CR_VEC_STR raws);

    friend class Command;
  };
}

#endif // __CLI_MENU__UTILS_HPP__