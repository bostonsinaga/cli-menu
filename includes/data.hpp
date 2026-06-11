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

      static void pushWord(Command *command, mt::CR_STR input);
      static void pushNumber(Command *command, mt::CR_LD input);
      static void pushBoolean(Command *command, mt::CR_BOL input);

      static void popWord(Command *command);
      static void popNumber(Command *command);
      static void popBoolean(Command *command);

      static void eraseWords(Command *command);
      static void eraseNumbers(Command *command);
      static void eraseBooleans(Command *command);

      // return false (empty) or true (cleared)
      static bool clearWords();
      static bool clearNumbers();
      static bool clearBooleans();
      static bool clearAll();

      /** GETTERS */

      // 'std::unordered_map' existence checkers
      static bool hasWords(Command *command);
      static bool hasNumbers(Command *command);
      static bool hasBooleans(Command *command);

      // return 0 for unknown id
      static size_t numberOfWords(Command *command);
      static size_t numberOfNumbers(Command *command);
      static size_t numberOfBooleans(Command *command);

      // get with index
      static std::string getWordAt(Command *command, mt::CR_SZ index);
      static mt::LD getNumberAt(Command *command, mt::CR_SZ index);
      static bool getBooleanAt(Command *command, mt::CR_SZ index);

      // get at front
      static std::string getFirstWord(Command *command);
      static mt::LD getFirstNumber(Command *command);
      static bool getFirstBoolean(Command *command);

      // get at back
      static std::string getLastWord(Command *command);
      static mt::LD getLastNumber(Command *command);
      static bool getLastBoolean(Command *command);

      /** PRINTERS */

      // display a command vector
      template <PRIMITIVE_TYPE T>
      static void printVector(Command *command, mt::CR_BOL withIndent);

      // belows are defined at 'command.cpp'
      static void print(Command *command);
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

    public:
      // setters
      static void push(Command *command, mt::CR_STR input);
      static void pop(Command *command);
      static void erase(Command *command);
      static bool clearAll();

      // getters
      static bool has(Command *command);
      static size_t numberOf(Command *command);
      static std::string getAt(Command *command, mt::CR_SZ index);
      static std::string getFirst(Command *command);
      static std::string getLast(Command *command);

      // concatenate string vector into a string
      static std::string concat(
        Command *command,
        mt::CR_STR separator = "\n"
      );

      // belows are defined at 'command.cpp'
      static void print(Command *command);
      static void printAll();
    };
  };
}

#include "data.tpp"
#endif // __CLI_MENU__DATA_HPP__