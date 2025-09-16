#ifndef __CLI_MENU__PRESET_HPP__
#define __CLI_MENU__PRESET_HPP__

#include "type.hpp"

namespace cli_menu {

  class Preset final {
  private:
    // get multiple filenames from wildcard pattern
    static void completeFileInWildcards(Command *node);

    /**
     * Write 'Result::outputs' to file.
     * Part of 'applyFileOut' preset callback.
     */
    static bool setFileOut(
      Command *node,
      std::string &filename
    );

  public:
    /** Read/write text file with custom handler */

    static void applyFileIn(
      Creator *owner,
      mt::CR_BOL isRequired,
      COMMAND_CALLBACK customCallback
    );

    static void applyFileOut(
      Creator *owner,
      mt::CR_BOL isRequired,
      COMMAND_CALLBACK customCallback
    );

    /**
     * Text file reader preset (can read multiple files).
     * Accept wildcards to get multiple files.
     * 
     * If 'isRequired' is false and filename is not provided,
     * the callback will return true without displaying error message.
     */
    static void applyFileIn(
      Creator *owner,
      mt::CR_BOL isRequired
    );

    /**
     * Text file writer preset.
     * 
     * Only write to the last filename of 'PRESET_OUT' argument.
     * If it not provided ('isRequired' is false), the last argument of 'PRESET_IN' will be used.
     * If still not provided, the program keyword will be used as the filename
     * with 'fileOutDefaultExtension' as its extension.
     * 
     * If 'isOptional' is true and filename is not provided,
     * the callback will return true without creating file
     * and without displaying error message.
     */
    static void applyFileOut(
      Creator *owner,
      mt::CR_BOL isRequired,
      mt::CR_BOL isOptional
    );

    // these are owned by all commands by default
    static void applyHelp(Creator *owner);
    static void applyList(Creator *owner);
  };
}

#endif // __CLI_MENU__PRESET_HPP__