#ifndef __CLI_MENU__RESULT_HPP__
#define __CLI_MENU__RESULT_HPP__

#include "language.hpp"

namespace cli_menu {

  class Command;

  template <typename T>
  using COMUNORMAP = mt::UNORMAP<Command*, T>;

  class Result {
  private:
    template <typename T>
    static std::string stringifiedVectorMember(mt::CR<T> vecmem);

    template <typename T>
    static void printType(
      mt::CR_STR stringifiedType,
      COMUNORMAP<mt::VEC<T>> &unormap
    );

    /** Access the data via 'Command' address */

    // parameters arguments
    inline static COMUNORMAP<mt::VEC_STR> words;
    inline static COMUNORMAP<mt::VEC_LD> numbers;
    inline static COMUNORMAP<mt::VEC_BOL> toggles;

    // better set inside the 'Command' callback
    inline static COMUNORMAP<mt::VEC_STR> ultimates;

    friend class Word;
    friend class Number;
    friend class Toggle;

  public:
    /** Setters */

    static void addWord(Command *node, mt::CR_STR input);
    static void addNumber(Command *node, mt::CR_LD input);
    static void addToggle(Command *node, mt::CR_BOL input);
    static void addUltimate(Command *node, mt::CR_STR input);

    static void removeWords(Command *node);
    static void removeNumbers(Command *node);
    static void removeToggles(Command *node);
    static void removeUltimate(Command *node);

    /** Getters */

    // 'std::unordered_map' existence checkers
    static bool hasWords(Command *node);
    static bool hasNumbers(Command *node);
    static bool hasToggles(Command *node);
    static bool hasUltimates(Command *node);

    // return 0 for unknown id
    static size_t numberOfWords(Command *node);
    static size_t numberOfNumbers(Command *node);
    static size_t numberOfToggles(Command *node);
    static size_t numberOfUltimates(Command *node);

    // get with index
    static std::string getWordAt(Command *node, mt::CR_SZ index);
    static mt::LD getNumberAt(Command *node, mt::CR_SZ index);
    static bool getToggleAt(Command *node, mt::CR_SZ index);
    static std::string getUltimateAt(Command *node, mt::CR_SZ index);

    // get at front
    static std::string getFirstWord(Command *node);
    static mt::LD getFirstNumber(Command *node);
    static bool getFirstToggle(Command *node);
    static std::string getFirstUltimate(Command *node);

    // get at back
    static std::string getLastWord(Command *node);
    static mt::LD getLastNumber(Command *node);
    static bool getLastToggle(Command *node);
    static std::string getLastUltimate(Command *node);

    // concatenate string vector into a string
    static std::string concatUltimates(
      Command *node,
      mt::CR_STR separator = "\n"
    );

    // display all arguments (defined at 'command.cpp')
    static void printInputs();
  };
}

#endif // __CLI_MENU__RESULT_HPP__