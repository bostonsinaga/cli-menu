#ifndef __CLI_MENU__DATA_HPP__
#define __CLI_MENU__DATA_HPP__

#include "language.hpp"

namespace cli_menu {

  class Command;

  template <typename T>
  concept WORD_TYPE = std::is_same_v<T, std::string>;

  template <typename T>
  concept NUMBER_TYPE = std::is_same_v<T, mt::LD>;

  template <typename T>
  concept BOOLEAN_TYPE = std::is_same_v<T, bool>;

  template <typename T>
  concept PRIMITIVE_TYPE = WORD_TYPE<T> || NUMBER_TYPE<T> || BOOLEAN_TYPE<T>;

  template <PRIMITIVE_TYPE T>
  using COMUNORMAP = mt::UNORMAP<Command*, mt::VEC<T>>;

  /**
   * Access the vector via 'Command' address.
   */
  class Data final {
  private:
    template <PRIMITIVE_TYPE T>
    static std::string stringifiedVectorMember(mt::CR<T> vecmem);

    // display single vector
    template <PRIMITIVE_TYPE T>
    static void printVector(mt::CR_VEC<T> vec, mt::CR_BOL withIndent);

    // display multiple vectors with stringified type as title on top
    template <PRIMITIVE_TYPE T>
    static void printType(
      mt::CR_STR stringifiedType,
      COMUNORMAP<T> &unormap
    );

  public:
    // clear both input and output
    static bool clearAll();

    /**
     * This class only accepts strings, numbers, and booleans.
     */
    class Input final {
    private:
      // parameters arguments
      inline static COMUNORMAP<std::string> words;
      inline static COMUNORMAP<mt::LD> numbers;
      inline static COMUNORMAP<bool> booleans;

      // friend classes
      friend class Word;
      friend class Number;
      friend class Boolean;

    public:
      /** SETTERS */

      static void pushWord(Command *node, mt::CR_STR input);
      static void pushNumber(Command *node, mt::CR_LD input);
      static void pushBoolean(Command *node, mt::CR_BOL input);

      static void popWord(Command *node);
      static void popNumber(Command *node);
      static void popBoolean(Command *node);

      static void eraseWords(Command *node);
      static void eraseNumbers(Command *node);
      static void eraseBooleans(Command *node);

      // return false (empty) or true (cleared)
      static bool clearWords();
      static bool clearNumbers();
      static bool clearBooleans();
      static bool clearAll();

      /** GETTERS */

      // 'std::unordered_map' existence checkers
      static bool hasWords(Command *node);
      static bool hasNumbers(Command *node);
      static bool hasBooleans(Command *node);

      // return 0 for unknown id
      static size_t numberOfWords(Command *node);
      static size_t numberOfNumbers(Command *node);
      static size_t numberOfBooleans(Command *node);

      // get with index
      static std::string getWordAt(Command *node, mt::CR_SZ index);
      static mt::LD getNumberAt(Command *node, mt::CR_SZ index);
      static bool getBooleanAt(Command *node, mt::CR_SZ index);

      // get at front
      static std::string getFirstWord(Command *node);
      static mt::LD getFirstNumber(Command *node);
      static bool getFirstBoolean(Command *node);

      // get at back
      static std::string getLastWord(Command *node);
      static mt::LD getLastNumber(Command *node);
      static bool getLastBoolean(Command *node);

      /** PRINTERS */

      // display a node vector
      template <PRIMITIVE_TYPE T>
      static void printVector(Command *node, mt::CR_BOL withIndent);

      // belows are defined at 'command.cpp'
      static void print(Command *node);
      static void printAll();
    };

    /**
     * This class only stores strings.
     * It can also represent non-text output
     * so it can still be displayed in the menu.
     */
    class Output final {
    private:
      // better set this vector inside the 'Command' callback
      inline static COMUNORMAP<std::string> texts;

      // friend classes
      friend class Word;
      friend class Number;
      friend class Boolean;

    public:
      // setters
      static void push(Command *node, mt::CR_STR input);
      static void pop(Command *node);
      static void erase(Command *node);
      static bool clearAll();

      // getters
      static bool has(Command *node);
      static size_t numberOf(Command *node);
      static std::string getAt(Command *node, mt::CR_SZ index);
      static std::string getFirst(Command *node);
      static std::string getLast(Command *node);

      // concatenate string vector into a string
      static std::string concat(
        Command *node,
        mt::CR_STR separator = "\n"
      );

      // belows are defined at 'command.cpp'
      static void print(Command *node);
      static void printAll();
    };
  };
}

#include "data.tpp"
#endif // __CLI_MENU__DATA_HPP__