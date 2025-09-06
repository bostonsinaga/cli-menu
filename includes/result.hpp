#ifndef __CLI_MENU__RESULT_HPP__
#define __CLI_MENU__RESULT_HPP__

#include "language.hpp"

namespace cli_menu {

  class Result {
  private:
    template <typename T>
    static std::string stringifiedVectorMember(mt::CR<T> vecmem);

    template <typename T>
    static void printType(
      mt::CR_STR stringifiedType,
      mt::STRUNORMAP<mt::VEC<T>> &unormap
    );

    /** Access the data via 'Command' keyword */

    // parameters arguments
    inline static mt::STRUNORMAP<mt::VEC_STR> words;
    inline static mt::STRUNORMAP<mt::VEC_LD> numbers;
    inline static mt::STRUNORMAP<mt::VEC_BOL> toggles;

    // better set inside the 'Command' callback
    inline static mt::STRUNORMAP<mt::VEC_STR> ultimates;

    friend class Word;
    friend class Number;
    friend class Toggle;

  public:
    /** Setters */

    static void addWord(mt::CR_STR keyword, mt::CR_STR input);
    static void addNumber(mt::CR_STR keyword, mt::CR_LD input);
    static void addToggle(mt::CR_STR keyword, mt::CR_BOL input);
    static void addUltimate(mt::CR_STR keyword, mt::CR_STR input);

    static void removeWords(mt::CR_STR keyword);
    static void removeNumbers(mt::CR_STR keyword);
    static void removeToggles(mt::CR_STR keyword);
    static void removeUltimate(mt::CR_STR keyword);

    /** Getters */

    // 'std::unordered_map' existence checkers
    static bool hasWords(mt::CR_STR keyword);
    static bool hasNumbers(mt::CR_STR keyword);
    static bool hasToggles(mt::CR_STR keyword);
    static bool hasUltimates(mt::CR_STR keyword);

    // return 0 for unknown keyword
    static size_t numberOfWords(mt::CR_STR keyword);
    static size_t numberOfNumbers(mt::CR_STR keyword);
    static size_t numberOfToggles(mt::CR_STR keyword);
    static size_t numberOfUltimates(mt::CR_STR keyword);

    // get with index
    static std::string getWordAt(mt::CR_STR keyword, mt::CR_SZ index);
    static mt::LD getNumberAt(mt::CR_STR keyword, mt::CR_SZ index);
    static bool getToggleAt(mt::CR_STR keyword, mt::CR_SZ index);
    static std::string getUltimateAt(mt::CR_STR keyword, mt::CR_SZ index);

    // get at front
    static std::string getFirstWord(mt::CR_STR keyword);
    static mt::LD getFirstNumber(mt::CR_STR keyword);
    static bool getFirstToggle(mt::CR_STR keyword);
    static std::string getFirstUltimate(mt::CR_STR keyword);

    // get at back
    static std::string getLastWord(mt::CR_STR keyword);
    static mt::LD getLastNumber(mt::CR_STR keyword);
    static bool getLastToggle(mt::CR_STR keyword);
    static std::string getLastUltimate(mt::CR_STR keyword);

    // concatenate string vector into a string
    static std::string concatUltimates(
      mt::CR_STR keyword,
      mt::CR_STR separator = "\n"
    );

    // display all arguments
    static void printInputs();
  };
}

#include "result.tpp"
#endif // __CLI_MENU__RESULT_HPP__