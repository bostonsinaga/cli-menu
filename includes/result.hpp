#ifndef __CLI_MENU__RESULT_HPP__
#define __CLI_MENU__RESULT_HPP__

#include "console.hpp"

namespace cli_menu {

  class Result {
  private:
    template <typename T>
    static std::string stringifiedVectorMember(T &vecmem);

    template <typename T>
    static void printType(
      mt::CR_STR stringifiedType,
      mt::STRUNORMAP<mt::VEC<T>> &unormap
    );

  public:
    inline static mt::STRUNORMAP<std::string> words;
    inline static mt::STRUNORMAP<mt::LD> numbers;
    inline static mt::STRUNORMAP<bool> toggles;

    static void addWord(mt::CR_STR keyword, mt::CR_STR input);
    static void addNumber(mt::CR_STR keyword, mt::CR_STR input);
    static void addToggle(mt::CR_STR keyword, mt::CR_STR input);

    static void removeWord(mt::CR_STR keyword);
    static void removeNumber(mt::CR_STR keyword);
    static void removeToggle(mt::CR_STR keyword);

    static void printInputs();
  };
}

#include "result.tpp"
#endif // __CLI_MENU__RESULT_HPP__