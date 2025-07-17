#ifndef __CLI_MENU__RESULT_HPP__
#define __CLI_MENU__RESULT_HPP__

#include "console.hpp"

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

  public:
    inline static mt::STRUNORMAP<mt::VEC_STR> words;
    inline static mt::STRUNORMAP<mt::VEC_LD> numbers;
    inline static mt::STRUNORMAP<mt::VEC_BOL> toggles;

    static void addWord(mt::CR_STR keyword, mt::CR_STR input);
    static void addNumber(mt::CR_STR keyword, mt::CR_LD input);
    static void addToggle(mt::CR_STR keyword, mt::CR_BOL input);

    static void removeWord(mt::CR_STR keyword);
    static void removeNumber(mt::CR_STR keyword);
    static void removeToggle(mt::CR_STR keyword);

    /**
     * The 'editing' parameter, defined in 'Command::editing',
     * is used to switch between editing and selecting modes.
     * In this class, it serves to distinguish the theme.
     */
    static void printInputs(mt::CR_BOL editing);
  };
}

#include "result.tpp"
#endif // __CLI_MENU__RESULT_HPP__