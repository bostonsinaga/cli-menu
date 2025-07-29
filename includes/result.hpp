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

    // 'Command' callback generated value
    inline static mt::STRUNORMAP_STR ultimate;

    // return these when getters are given an unknown keyword
    inline static mt::VEC_STR wordsGarbage;
    inline static mt::VEC_LD numbersGarbage;
    inline static mt::VEC_BOL togglesGarbage;
    inline static std::string ultimateGarbage;

    // 'std::unordered_map' existence checkers
    static bool hasWords(mt::CR_STR keyword);
    static bool hasNumbers(mt::CR_STR keyword);
    static bool hasToggles(mt::CR_STR keyword);

    friend class Word;
    friend class Number;
    friend class Toggle;

  public:
    static void addWords(mt::CR_STR keyword, mt::CR_STR input);
    static void addNumbers(mt::CR_STR keyword, mt::CR_LD input);
    static void addToggles(mt::CR_STR keyword, mt::CR_BOL input);

    static void removeWords(mt::CR_STR keyword);
    static void removeNumbers(mt::CR_STR keyword);
    static void removeToggles(mt::CR_STR keyword);

    /** Direct access to 'std::unordered_map' values */

    static mt::VEC_STR &getWords(mt::CR_STR keyword);
    static mt::VEC_LD &getNumbers(mt::CR_STR keyword);
    static mt::VEC_BOL &getToggles(mt::CR_STR keyword);

    static std::string &getUltimate(mt::CR_STR keyword);

    // display all arguments
    static void printInputs();
  };
}

#include "result.tpp"
#endif // __CLI_MENU__RESULT_HPP__