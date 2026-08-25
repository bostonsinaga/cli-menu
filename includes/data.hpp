#ifndef __CLI_MENU__DATA_HPP__
#define __CLI_MENU__DATA_HPP__

#include "language.hpp"

namespace cli_menu {

  class Command;

  /** Unordered Map Structs */

  struct TextMaps {
    mt::UNORMAP<Command*, mt::PAIR2<int, mt::VEC_STR>> comvec;
    mt::PAIR2<int, mt::VEC_STR> empvec;
    mt::VEC_STR defvec;
  };

  struct WordMaps {
    mt::UNORMAP<Command*, mt::PAIR2<int, mt::VEC_STR>> comvec;
    mt::PAIR2<int, mt::VEC_STR> empvec;
    mt::VEC_STR defvec;
  };

  struct NumberMaps {
    mt::UNORMAP<Command*, mt::PAIR2<int, mt::VEC_DBL>> comvec;
    mt::PAIR2<int, mt::VEC_DBL> empvec;
    mt::VEC_DBL defvec;
  };

  struct BooleanMaps {
    mt::UNORMAP<Command*, mt::PAIR2<int, mt::VEC_BOL>> comvec;
    mt::PAIR2<int, mt::VEC_BOL> empvec;
    mt::VEC_BOL defvec;
  };

  /** Type Template Constraints */

  template <typename T>
  concept UNORMAP_COMVEC_TYPE =
    std::is_same_v<T, TextMaps> || std::is_same_v<T, WordMaps> ||
    std::is_same_v<T, NumberMaps> || std::is_same_v<T, BooleanMaps>;

  template <typename T>
  concept PRIMITIVE_TYPE =
    std::is_same_v<T, std::string> ||
    std::is_same_v<T, double> ||
    std::is_same_v<T, bool>;

  class Data final {
  public:
    typedef mt::ARR<CONLOR_CODE, 2> ConlorHighlightCodeSet;

    inline static ConlorHighlightCodeSet
      ConlorHighlightSticked { CONLOR_LIST, CONLOR_HIGHLIGHT },
      ConlorHighlightBranched { CONLOR_DESCRIPTION, CONLOR_HIGHLIGHT };

  private:
    inline static TextMaps textMaps {{}, {}, {""}};
    inline static WordMaps wordMaps {{}, {}, {""}};
    inline static NumberMaps numberMaps {{}, {}, {0}};
    inline static BooleanMaps booleanMaps {{}, {}, {false}};

    /** Reusable Template Methods */

    template <UNORMAP_COMVEC_TYPE T>
    static T &use();

    template <UNORMAP_COMVEC_TYPE T>
    static void ragister(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T>
    static void unragister(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T>
    static bool has(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T>
    static bool isEmpty(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static mt::VEC<U>::reference getValue(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static mt::VEC<U>::reference getValue(
      Command *comkey,
      int &index
    );

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static mt::VEC<U> getVector(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static void set(
      Command *comkey,
      mt::CR<U> value
    );

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static void set(
      Command *comkey,
      mt::CR<U> value,
      int &index
    );

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static void replace(
      Command *comkey,
      mt::CR_VEC<U> values
    );

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static void add(
      Command *comkey,
      mt::CR_VEC<U> values
    );

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static void push(
      Command *comkey,
      mt::CR<U> value
    );

    template <UNORMAP_COMVEC_TYPE T, PRIMITIVE_TYPE U>
    static U pop(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T>
    static void reset(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T>
    static bool select(
      Command *comkey,
      mt::CR_INT direction
    );

    template <UNORMAP_COMVEC_TYPE T>
    static mt::VEC_STR vectorStringify(Command *comkey);

    template <UNORMAP_COMVEC_TYPE T>
    inline static std::string stringify(
      Command *comkey,
      mt::CR_STR separator
    );

    template <UNORMAP_COMVEC_TYPE T>
    static std::string joinColorize(
      Command *comkey,
      mt::CR_VEC_STR vecstr,
      mt::CR<ConlorHighlightCodeSet> conlorSet,
      mt::CR_STR separator
    );

    template <UNORMAP_COMVEC_TYPE T>
    static void print(
      Command *comkey,
      mt::CR<Console::Indent> indent,
      mt::CR<ConlorHighlightCodeSet> conlorSet
    );

    template <UNORMAP_COMVEC_TYPE T>
    static void copyInputToOutput(Command *comkey);

    friend class Parameter;

  public:
    Data() = delete;

    /** Register Vector */

    inline static void registerTexts(Command *comkey) {
      ragister<TextMaps>(comkey);
    }

    inline static void registerWords(Command *comkey) {
      ragister<WordMaps>(comkey);
    }

    inline static void registerNumbers(Command *comkey) {
      ragister<NumberMaps>(comkey);
    }

    inline static void registerBooleans(Command *comkey) {
      ragister<BooleanMaps>(comkey);
    }

    /** Unregister Vector */

    inline static void unregisterTexts(Command *comkey) {
      unragister<TextMaps>(comkey);
    }

    inline static void unregisterWords(Command *comkey) {
      unragister<WordMaps>(comkey);
    }

    inline static void unregisterNumbers(Command *comkey) {
      unragister<NumberMaps>(comkey);
    }

    inline static void unregisterBooleans(Command *comkey) {
      unragister<BooleanMaps>(comkey);
    }

    /** Check Key Existence */

    inline static bool hasTexts(Command *comkey) {
      return has<TextMaps>(comkey);
    }

    inline static bool hasWords(Command *comkey) {
      return has<WordMaps>(comkey);
    }

    inline static bool hasNumbers(Command *comkey) {
      return has<NumberMaps>(comkey);
    }

    inline static bool hasBooleans(Command *comkey) {
      return has<BooleanMaps>(comkey);
    }

    /** Check Empty Vector */

    inline static bool isTextsEmpty(Command *comkey) {
      return isEmpty<TextMaps>(comkey);
    }

    inline static bool isWordsEmpty(Command *comkey) {
      return isEmpty<WordMaps>(comkey);
    }

    inline static bool isNumbersEmpty(Command *comkey) {
      return isEmpty<NumberMaps>(comkey);
    }

    inline static bool isBooleansEmpty(Command *comkey) {
      return isEmpty<BooleanMaps>(comkey);
    }

    /** Count Vector Size */

    inline static size_t getTextsSize(Command *comkey) {
      return hasTexts(comkey) ? textMaps.comvec[comkey].second.size() : 0;
    }

    inline static size_t getWordsSize(Command *comkey) {
      return hasWords(comkey) ? wordMaps.comvec[comkey].second.size() : 0;
    }

    inline static size_t getNumbersSize(Command *comkey) {
      return hasNumbers(comkey) ? numberMaps.comvec[comkey].second.size() : 0;
    }

    inline static size_t getBooleansSize(Command *comkey) {
      return hasBooleans(comkey) ? booleanMaps.comvec[comkey].second.size() : 0;
    }

    /** Get Selected Index */

    inline static int getTextIndex(Command *comkey) {
      return textMaps.comvec[comkey].first;
    }

    inline static int getWordIndex(Command *comkey) {
      return wordMaps.comvec[comkey].first;
    }

    inline static int getNumberIndex(Command *comkey) {
      return numberMaps.comvec[comkey].first;
    }

    inline static int getBooleanIndex(Command *comkey) {
      return booleanMaps.comvec[comkey].first;
    }

    /** Get Selected Single Value */

    inline static mt::VEC_STR::reference getText(Command *comkey) {
      return getValue<TextMaps, std::string>(comkey);
    }

    inline static mt::VEC_STR::reference getWord(Command *comkey) {
      return getValue<WordMaps, std::string>(comkey);
    }

    inline static mt::VEC_DBL::reference getNumber(Command *comkey) {
      return getValue<NumberMaps, double>(comkey);
    }

    inline static mt::VEC_BOL::reference getBoolean(Command *comkey) {
      return getValue<BooleanMaps, bool>(comkey);
    }

    /** Get Single Value At Index (Safe) */

    inline static mt::VEC_STR::reference getText(
      Command *comkey,
      int index
    ) {
      return getValue<TextMaps, std::string>(comkey, index);
    }

    inline static mt::VEC_STR::reference getWord(
      Command *comkey,
      int index
    ) {
      return getValue<WordMaps, std::string>(comkey, index);
    }

    inline static mt::VEC_DBL::reference getNumber(
      Command *comkey,
      int index
    ) {
      return getValue<NumberMaps, double>(comkey, index);
    }

    inline static mt::VEC_BOL::reference getBoolean(
      Command *comkey,
      int index
    ) {
      return getValue<BooleanMaps, bool>(comkey, index);
    }

    /** Get Single Value At Index (Unsafe) */

    inline static mt::VEC_STR::reference xgetText(
      Command *comkey,
      mt::CR_INT index
    ) {
      return textMaps.comvec[comkey].second[index];
    }

    inline static mt::VEC_STR::reference xgetWord(
      Command *comkey,
      mt::CR_INT index
    ) {
      return wordMaps.comvec[comkey].second[index];
    }

    inline static mt::VEC_DBL::reference xgetNumber(
      Command *comkey,
      mt::CR_INT index
    ) {
      return numberMaps.comvec[comkey].second[index];
    }

    inline static mt::VEC_BOL::reference xgetBoolean(
      Command *comkey,
      mt::CR_INT index
    ) {
      return booleanMaps.comvec[comkey].second[index];
    }

    /** Get Vector Copy (Safe) */

    inline static mt::VEC_STR getTexts(Command *comkey) {
      return getVector<TextMaps, std::string>(comkey);
    }

    inline static mt::VEC_STR getWords(Command *comkey) {
      return getVector<WordMaps, std::string>(comkey);
    }

    inline static mt::VEC_DBL getNumbers(Command *comkey) {
      return getVector<NumberMaps, double>(comkey);
    }

    inline static mt::VEC_BOL getBooleans(Command *comkey) {
      return getVector<BooleanMaps, bool>(comkey);
    }

    /** Get Vector Reference (Unsafe) */

    inline static mt::VEC_STR &xgetTexts(Command *comkey) {
      return textMaps.comvec[comkey].second;
    }

    inline static mt::VEC_STR &xgetWords(Command *comkey) {
      return wordMaps.comvec[comkey].second;
    }

    inline static mt::VEC_DBL &xgetNumbers(Command *comkey) {
      return numberMaps.comvec[comkey].second;
    }

    inline static mt::VEC_BOL &xgetBooleans(Command *comkey) {
      return booleanMaps.comvec[comkey].second;
    }

    /** Set Selected Single Value */

    inline static void setText(
      Command *comkey,
      mt::CR_STR value
    ) {
      set<TextMaps, std::string>(comkey, value);
    }

    inline static void setWord(
      Command *comkey,
      mt::CR_STR value
    ) {
      set<WordMaps, std::string>(comkey, value);
    }

    inline static void setNumber(
      Command *comkey,
      mt::CR_DBL value
    ) {
      set<NumberMaps, double>(comkey, value);
    }

    inline static void setBoolean(
      Command *comkey,
      mt::CR_BOL value
    ) {
      set<BooleanMaps, bool>(comkey, value);
    }

    /** Set Single Value At Index (Safe) */

    inline static void setText(
      Command *comkey,
      mt::CR_STR value,
      int index
    ) {
      set<TextMaps, std::string>(comkey, value, index);
    }

    inline static void setWord(
      Command *comkey,
      mt::CR_STR value,
      int index
    ) {
      set<WordMaps, std::string>(comkey, value, index);
    }

    inline static void setNumber(
      Command *comkey,
      mt::CR_DBL value,
      int index
    ) {
      set<NumberMaps, double>(comkey, value, index);
    }

    inline static void setBoolean(
      Command *comkey,
      mt::CR_BOL value,
      int index
    ) {
      set<BooleanMaps, bool>(comkey, value, index);
    }

    /** Set Single Value At Index (Unsafe) */

    inline static void xsetText(
      Command *comkey,
      mt::CR_STR value,
      mt::CR_INT index
    ) {
      textMaps.comvec[comkey].second[index] = value;
    }

    inline static void xsetWord(
      Command *comkey,
      mt::CR_STR value,
      mt::CR_INT index
    ) {
      wordMaps.comvec[comkey].second[index] = value;
    }

    inline static void xsetNumber(
      Command *comkey,
      mt::CR_DBL value,
      mt::CR_INT index
    ) {
      numberMaps.comvec[comkey].second[index] = value;
    }

    inline static void xsetBoolean(
      Command *comkey,
      mt::CR_BOL value,
      mt::CR_INT index
    ) {
      booleanMaps.comvec[comkey].second[index] = value;
    }

    /** Set Default Value */

    inline static void setDefaultText(mt::CR_STR value) {
      textMaps.defvec.back() = value;
    }

    inline static void setDefaultWord(mt::CR_STR value) {
      wordMaps.defvec.back() = value;
    }

    inline static void setDefaultNumber(mt::CR_DBL value) {
      numberMaps.defvec.back() = value;
    }

    inline static void setDefaultBoolean(mt::CR_BOL value) {
      booleanMaps.defvec.back() = value;
    }

    /** Replace Vector */

    inline static void replaceTexts(
      Command *comkey,
      mt::CR_VEC_STR values
    ) {
      replace<TextMaps, std::string>(comkey, values);
    }

    inline static void replaceWords(
      Command *comkey,
      mt::CR_VEC_STR values
    ) {
      replace<WordMaps, std::string>(comkey, values);
    }

    inline static void replaceNumbers(
      Command *comkey,
      mt::CR_VEC_DBL values
    ) {
      replace<NumberMaps, double>(comkey, values);
    }

    inline static void replaceBooleans(
      Command *comkey,
      mt::CR_VEC_BOL values
    ) {
      replace<BooleanMaps, bool>(comkey, values);
    }

    /** Add Multiple Values */

    inline static void addTexts(
      Command *comkey,
      mt::CR_VEC_STR values
    ) {
      add<TextMaps, std::string>(comkey, values);
    }

    inline static void addWords(
      Command *comkey,
      mt::CR_VEC_STR values
    ) {
      add<WordMaps, std::string>(comkey, values);
    }

    inline static void addNumbers(
      Command *comkey,
      mt::CR_VEC_DBL values
    ) {
      add<NumberMaps, double>(comkey, values);
    }

    inline static void addBooleans(
      Command *comkey,
      mt::CR_VEC_BOL values
    ) {
      add<BooleanMaps, bool>(comkey, values);
    }

    /** Push Value (Safe) */

    inline static void pushText(
      Command *comkey,
      mt::CR_STR value
    ) {
      push<TextMaps, std::string>(comkey, value);
    }

    inline static void pushWord(
      Command *comkey,
      mt::CR_STR value
    ) {
      push<WordMaps, std::string>(comkey, value);
    }

    inline static void pushNumber(
      Command *comkey,
      mt::CR_DBL value
    ) {
      push<NumberMaps, double>(comkey, value);
    }

    inline static void pushBoolean(
      Command *comkey,
      mt::CR_BOL value
    ) {
      push<BooleanMaps, bool>(comkey, value);
    }

    /** Push Value (Unsafe) */

    inline static void xpushText(
      Command *comkey,
      mt::CR_STR value
    ) {
      textMaps.comvec[comkey].second.push_back(value);
    }

    inline static void xpushWord(
      Command *comkey,
      mt::CR_STR value
    ) {
      wordMaps.comvec[comkey].second.push_back(value);
    }

    inline static void xpushNumber(
      Command *comkey,
      mt::CR_DBL value
    ) {
      numberMaps.comvec[comkey].second.push_back(value);
    }

    inline static void xpushBoolean(
      Command *comkey,
      mt::CR_BOL value
    ) {
      booleanMaps.comvec[comkey].second.push_back(value);
    }

    /** Pop Value (Safe) */

    inline static std::string popText(Command *comkey) {
      return pop<TextMaps, std::string>(comkey);
    }

    inline static std::string popWord(Command *comkey) {
      return pop<WordMaps, std::string>(comkey);
    }

    inline static double popNumber(Command *comkey) {
      return pop<NumberMaps, double>(comkey);
    }

    inline static bool popBoolean(Command *comkey) {
      return pop<BooleanMaps, bool>(comkey);
    }

    /** Pop Value (Unsafe) */

    inline static void xpopText(Command *comkey) {
      textMaps.comvec[comkey].second.pop_back();
    }

    inline static void xpopWord(Command *comkey) {
      wordMaps.comvec[comkey].second.pop_back();
    }

    inline static void xpopNumber(Command *comkey) {
      numberMaps.comvec[comkey].second.pop_back();
    }

    inline static void xpopBoolean(Command *comkey) {
      booleanMaps.comvec[comkey].second.pop_back();
    }

    /** Reset Vector */

    inline static void resetTexts(Command *comkey) {
      reset<TextMaps>(comkey);
    }

    inline static void resetWords(Command *comkey) {
      reset<WordMaps>(comkey);
    }

    inline static void resetNumbers(Command *comkey) {
      reset<NumberMaps>(comkey);
    }

    inline static void resetBooleans(Command *comkey) {
      reset<BooleanMaps>(comkey);
    }

    /** Select Member By Direction */

    inline static bool selectText(
      Command *comkey,
      mt::CR_INT direction
    ) {
      return select<TextMaps>(comkey, direction);
    }

    inline static bool selectWord(
      Command *comkey,
      mt::CR_INT direction
    ) {
      return select<WordMaps>(comkey, direction);
    }

    inline static bool selectNumber(
      Command *comkey,
      mt::CR_INT direction
    ) {
      return select<NumberMaps>(comkey, direction);
    }

    inline static bool selectBoolean(
      Command *comkey,
      mt::CR_INT direction
    ) {
      return select<BooleanMaps>(comkey, direction);
    }

    /** Convert Vector Into String */

    inline static std::string stringifyTexts(
      Command *comkey,
      mt::CR_STR separator = "\n"
    ) {
      return stringify<TextMaps>(comkey, separator);
    }

    inline static std::string stringifyWords(
      Command *comkey,
      mt::CR_STR separator = "\n"
    ) {
      return stringify<WordMaps>(comkey, separator);
    }

    inline static std::string stringifyNumbers(
      Command *comkey,
      mt::CR_STR separator = "\n"
    ) {
      return stringify<NumberMaps>(comkey, separator);
    }

    // stringified boolean is based on 'mt_uti::Booleanizer'
    inline static std::string stringifyBooleans(
      Command *comkey,
      mt::CR_STR separator = "\n"
    ) {
      return stringify<BooleanMaps>(comkey, separator);
    }

    /** Print Stringified Vector */

    inline static void printTexts(
      Command *comkey,
      mt::CR<Console::Indent> indent,
      mt::CR<ConlorHighlightCodeSet> conlorSet
    ) {
      print<TextMaps>(comkey, indent, conlorSet);
    }

    inline static void printWords(
      Command *comkey,
      mt::CR<Console::Indent> indent,
      mt::CR<ConlorHighlightCodeSet> conlorSet
    ) {
      print<WordMaps>(comkey, indent, conlorSet);
    }

    inline static void printNumbers(
      Command *comkey,
      mt::CR<Console::Indent> indent,
      mt::CR<ConlorHighlightCodeSet> conlorSet
    ) {
      print<NumberMaps>(comkey, indent, conlorSet);
    }

    inline static void printBooleans(
      Command *comkey,
      mt::CR<Console::Indent> indent,
      mt::CR<ConlorHighlightCodeSet> conlorSet
    ) {
      print<BooleanMaps>(comkey, indent, conlorSet);
    }

    /** Copy Input To Output */

    static void copyInputToOutputWords(Command *comkey) {
      copyInputToOutput<WordMaps>(comkey);
    }

    static void copyInputToOutputNumbers(Command *comkey) {
      copyInputToOutput<NumberMaps>(comkey);
    }

    static void copyInputToOutputBooleans(Command *comkey) {
      copyInputToOutput<BooleanMaps>(comkey);
    }
  };
}

#include "data.tpp"
#endif // __CLI_MENU__DATA_HPP__