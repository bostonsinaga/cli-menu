#ifndef __CLI_MENU__CONSOLE_HPP__
#define __CLI_MENU__CONSOLE_HPP__

#include "color.hpp"

namespace cli_menu {

  // console color code
  enum CONLOR_CODE {
    CONLOR_ERROR, CONLOR_WARNING, CONLOR_HINT,
    CONLOR_TITLE, CONLOR_DESCRIPTION, CONLOR_LIST,
    CONLOR_HIGHLIGHT, CONLOR_CORRECT, CONLOR_CANCEL
  };

  /**
   * Display styled text to terminal.
   */
  class Console {
  private:
    static constexpr int totalStatus = 9;

  public:
    Console() = delete;

    /** Global Indentation Control */

    class Indent {
    public:
      inline virtual std::string get() const { return ""; }
      inline virtual void set(mt::CR_SZ number_in) {}
    };

    class IndentSticked : public Indent {
    private:
      inline static size_t number = 0;

    public:
      inline void set(mt::CR_SZ number_in) override {
        IndentSticked::number = number_in;
      }

      inline std::string get() const override {
        return std::string(IndentSticked::number, ' ');
      }
    };

    class IndentBranched : public Indent {
    private:
      inline static size_t number = 2;

    public:
      inline void set(mt::CR_SZ number_in) override {
        IndentBranched::number = number_in;
      }

      inline std::string get() const override {
        return std::string(IndentBranched::number, ' ');
      }
    };

    /** VARIABLES */

    inline static bool outlineStyle = true;
    inline static char boundaryCharacter = '-';
    inline static size_t boundaryLineLength = 45;
    inline static std::string listPointStyle = ">";
    static Color messageColors[totalStatus];

    static Color
      boundaryModifyColor,
      boundarySelectionColor,
      boxModifyColors[2],
      boxSelectionColors[2];

    /** NORMAL */

    static void logString(
      mt::CR_STR text,
      mt::CR<Color> foreground
    );

    static void logString(
      mt::CR_STR text,
      mt::CR<Color> foreground,
      mt::CR<Color> background
    );

    /** ITALIC */

    static void logItalicString(mt::CR_STR text);

    static void logItalicString(
      mt::CR_STR text,
      mt::CR<Color> foreground
    );

    static void logItalicString(
      mt::CR_STR text,
      mt::CR<Color> foreground,
      mt::CR<Color> background
    );

    /** UNDERLINE */

    static void logUnderlineString(mt::CR_STR text);

    static void logUnderlineString(
      mt::CR_STR text,
      mt::CR<Color> foreground
    );

    static void logUnderlineString(
      mt::CR_STR text,
      mt::CR<Color> foreground,
      mt::CR<Color> background
    );

    /**
     * SPECIALS
     * The 'editing' parameter, defined in 'Command::editing',
     * is used to switch between editing and selecting modes.
     * In this class, it serves to distinguish the theme.
     */

    static mt::CR<Color> chooseBoundaryColor(mt::CR_BOL editing);
    static void logBoundaryLine(mt::CR_BOL editing);

    static void logStylishHeader(
      mt::CR_STR title,
      mt::CR_BOL editing
    );

    /**
     * Call 'logString' with 'listPointStyle' at the beginning,
     * colored with the predefined 'messageColors'.
     */
    static void logResponse(
      const CONLOR_CODE &code,
      mt::CR_STR reason
    );

    // clear the terminal screen
    static void clearScreen() {
      std::cout << "\033[2J\033[3J\033[H" << std::flush;
    }

    struct LimitedText {
      inline static bool wrapping = true;
      inline static size_t maxCharacters = 45;

      inline static std::string
        tailDots = "...",
        remainingBrackets[2] {"[+", "]"};

      /**
       * Limit the text to 'maxCharacters'.
       * Ended with 'tailDots' and 'remainingBrackets'.
       */
      static std::string trim(
        mt::CR_STR text,
        mt::CR_BOL withTailDots = true,
        mt::CR_BOL withRemainingBrackets = true
      );

      // drop part of the text to a newline after exceeding the 'maxCharacters'
      static std::string wrap(mt::CR_STR text);
    };
  };
}

#endif // __CLI_MENU__CONSOLE_HPP__