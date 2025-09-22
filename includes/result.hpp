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
    inline static COMUNORMAP<mt::VEC_BOL> booleans;

    // better set inside the 'Command' callback
    inline static COMUNORMAP<mt::VEC_STR> outputs;

    // display current node vector
    template <typename T>
    static void printVector(mt::CR_VEC<T> vec, mt::CR_BOL withIndent);

    // friend classes
    friend class Word;
    friend class Number;
    friend class Boolean;

  public:
    /** Setters */

    static void pushWord(Command *node, mt::CR_STR input);
    static void pushNumber(Command *node, mt::CR_LD input);
    static void pushBoolean(Command *node, mt::CR_BOL input);
    static void pushOutput(Command *node, mt::CR_STR input);

    static void popWord(Command *node);
    static void popNumber(Command *node);
    static void popBoolean(Command *node);
    static void popOutput(Command *node);

    static void eraseWords(Command *node);
    static void eraseNumbers(Command *node);
    static void eraseBooleans(Command *node);
    static void eraseOutputs(Command *node);

    static void clearWords();
    static void clearNumbers();
    static void clearBooleans();
    static void clearOutputs();

    // return false (empty) or true (cleared)
    static bool clearAll();

    /** Getters */

    // 'std::unordered_map' existence checkers
    static bool hasWords(Command *node);
    static bool hasNumbers(Command *node);
    static bool hasBooleans(Command *node);
    static bool hasOutputs(Command *node);

    // return 0 for unknown id
    static size_t numberOfWords(Command *node);
    static size_t numberOfNumbers(Command *node);
    static size_t numberOfBooleans(Command *node);
    static size_t numberOfOutputs(Command *node);

    // get with index
    static std::string getWordAt(Command *node, mt::CR_SZ index);
    static mt::LD getNumberAt(Command *node, mt::CR_SZ index);
    static bool getBooleanAt(Command *node, mt::CR_SZ index);
    static std::string getOutputAt(Command *node, mt::CR_SZ index);

    // get at front
    static std::string getFirstWord(Command *node);
    static mt::LD getFirstNumber(Command *node);
    static bool getFirstBoolean(Command *node);
    static std::string getFirstOutput(Command *node);

    // get at back
    static std::string getLastWord(Command *node);
    static mt::LD getLastNumber(Command *node);
    static bool getLastBoolean(Command *node);
    static std::string getLastOutput(Command *node);

    // concatenate string vector into a string
    static std::string concatOutputs(
      Command *node,
      mt::CR_STR separator = "\n"
    );

    // display current node vector
    template <typename T>
    static void printVector(Command *node, mt::CR_BOL withIndent);

    // display all visited node vectors (defined at 'command.cpp')
    static void printInputs(Command *onlyThis);
    static void printOutputs(Command *onlyThis);
  };
}

#endif // __CLI_MENU__RESULT_HPP__