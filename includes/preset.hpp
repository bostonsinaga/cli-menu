#ifndef __CLI_MENU__PRESET_HPP__
#define __CLI_MENU__PRESET_HPP__

#include "type.hpp"

namespace cli_menu {

  class Preset final {
  private:
    // set command to pseudo, sterilized, and has no data
    static void hide(Command *command);

  public:
    // these are owned by all commands by default
    static void applyHelp(Parameter *owner);
    static void applyList(Parameter *owner);

    class File final {
    private:
      // get multiple filenames from wildcard pattern
      static void completePathWildcards(Command *command);

      /**
       * Write output string into a text file.
       * Used as part of the 'applyTextOut..' callback.
       */
      static COMMAND_CALLBACK_CODE useTextOut(
        Command *owner,
        std::string filename
      );

    public:
      /**
       * Read text file with custom handler.
       */
      static void applyCustomIn(
        Parameter *owner,
        mt::CR_BOL isRequired,
        mt::CR<CODE_CALLBACK> callback
      );

      /**
       * Text file reader preset (can read multiple files).
       * Accept wildcards to get multiple files.
       * 
       * If 'isRequired' is false and filename is not provided,
       * the callback will return true without displaying error message.
       */
      static void applyTextIn(
        Parameter *owner,
        mt::CR_BOL isRequired
      );

      /**
       * Write text file with custom handler.
       */
      static void applyCustomOut(
        Parameter *owner,
        mt::CR_BOL isRequired,
        mt::CR<CODE_CALLBACK> callback
      );

      /**
       * Text file writer preset.
       * 
       * Only write to the last filename of 'PRESET_OUT' argument.
       * If it not provided ('isRequired' is false), the last argument of 'PRESET_IN' will be used.
       * If still not provided, the program keyword will be used as the filename
       * with 'fileOutDefaultExtension' as its extension.
       */
      static void applyTextOutFallback(
        Parameter *owner,
        mt::CR_BOL isRequired
      );

      /**
       * Text file writer preset.
       * 
       * If filename is not provided, the callback will return true
       * without creating file and without displaying error message.
       * 
       * The node will not be required.
       */
      static void applyTextOutOptional(Parameter *owner);
    };

    /**
     * For displaying color presets.
     * Show their names and RGB values.
     */
    class ColorSet final {
    public:
      inline static int spacing = 1;

    private:
      static Color highlights[COLOR_TOTAL];

      static mt::PAIR2<std::string, size_t> stringify(
        const COLOR_CODE &code,
        CR_CLR foreground,
        CR_CLR background
      );

      static void print();

      inline static std::string getSpacing() {
        return std::string(spacing, ' ');
      }

    public:
      // this owned by a 'Program' by default
      static void apply(Parameter *owner);
    };
  };
}

#endif // __CLI_MENU__PRESET_HPP__