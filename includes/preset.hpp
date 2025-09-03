#ifndef __CLI_MENU__PRESET_HPP__
#define __CLI_MENU__PRESET_HPP__

#include "type.hpp"

namespace cli_menu {

  class Preset final {
  public:
    /** Read/write text file with custom handler */

    static void applyFileIn(
      Creator *owner,
      COMMAND_CALLBACK customCallback
    );

    static void applyFileOut(
      Creator *owner,
      COMMAND_CALLBACK customCallback
    );

    /** Read/write text file with default handler */

    // can read multiple files
    static void applyFileIn(Creator *owner);

    // only write to the last filename
    static void applyFileOut(Creator *owner);

    // call the 2 methods above at once
    static void applyFile(Creator *owner);

    // these are owned by all commands by default
    static void applyHelp(Creator *owner);
    static void applyList(Creator *owner);
  };
}

#endif // __CLI_MENU__PRESET_HPP__