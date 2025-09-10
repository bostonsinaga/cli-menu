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

    static void pushWord(Command *node, mt::CR_STR input);
    static void pushNumber(Command *node, mt::CR_LD input);
    static void pushToggle(Command *node, mt::CR_BOL input);
    static void pushUltimate(Command *node, mt::CR_STR input);

    void popWord(Command *node);
    void popNumber(Command *node);
    void popToggle(Command *node);
    void popUltimate(Command *node);

    static void eraseWords(Command *node);
    static void eraseNumbers(Command *node);
    static void eraseToggles(Command *node);
    static void eraseUltimates(Command *node);

    static void clearWords();
    static void clearNumbers();
    static void clearToggles();
    static void clearUltimates();
    static void clearAll();

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

    // display current node arguments
    template <typename T>
    static void printInput(mt::CR_VEC<T> vec, mt::CR_BOL withIndent);

    // display all visited nodes arguments
    static void printInputs(Command *onlyThis); // defined at 'command.cpp'
  };
}

#endif // __CLI_MENU__RESULT_HPP__