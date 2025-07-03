#ifndef __CLI_MENU__RESULT_HPP__
#define __CLI_MENU__RESULT_HPP__

namespace cli_menu {

  class Result {
  private:
    template <typename T>
    concept LIMITED_TYPE =
      std::is_same_v<T, std::string> ||
      std::is_same_v<T, mt::LD> ||
      std::is_same_v<T, bool>;

    template <LIMITED_TYPE T>
    static std::string stringifiedVectorMember(T &vecmem);

    template <LIMITED_TYPE T>
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