#ifndef __CLI_MENU__LANGUAGE_H__
#define __CLI_MENU__LANGUAGE_H__

#include "base.h"

namespace cli_menu {

  enum LANG_ENUM {
    LANGERR_DIALOG_ALREADY_SELECTING,
    LANGERR_DIALOG_BACK_AT_ROOT,
    LANGERR_DIALOG_EMPTY_REQUIRED_ARGUMENTS,
    LANGERR_DIALOG_FORBIDDEN_HIDDEN_TEXT_PASTING,
    LANGERR_DIALOG_INCOMPLETE_ULTIMATE_PROCESS,
    LANGERR_DIALOG_NO_ITEMS,
    LANGERR_DIALOG_NO_NEIGHBORS,
    LANGERR_DIALOG_NOT_FOUND,
    LANGERR_DIALOG_UNPROCESSED_DIRECT_INPUTS,
    LANGERR_MATCH_EMPTY_REQUIRED_ARGUMENTS,
    LANGERR_MATCH_NEEDS_DIALOG_FOR_MANY,
    LANGERR_MATCH_NEEDS_DIALOG_FOR_ONE,
    LANGERR_MATCH_UNKNOWN_NEIGHBOR
  };

  typedef const LANG_ENUM& CR_LANGNUM;

  class Language {
  private:
    static const int max_sentences_size = 13;
    typedef mt::ARR<std::string, max_sentences_size> LANGARR;

    // name and sentences
    typedef mt::PAIR2<std::string, LANGARR> PAIR_LANGARR;

    static const std::string DEFAULT_SENTENCES[max_sentences_size];
    static mt::VEC<PAIR_LANGARR> SENTENCES_COLLECTION;

  public:
    static std::string
      LEVEL_PLACEHOLDER, NAME_PLACEHOLDER, TYPE_PLACEHOLDER;

    // first called for 'SENTENCES' initialization
    static void reset();

    static void addCollection(mt::CR_STR name);
    static void removeCollection(mt::CR_INT collectionIndex);

    static void translate(
      mt::CR_INT collectionIndex,
      CR_LANGNUM sentencesIndex,
      mt::CR_STR sentence
    );

    static std::string solveTemplate(
      mt::CR_INT collectionIndex,
      CR_LANGNUM sentencesIndex,
      mt::CR_STR placeholder,
      mt::CR_VEC_STR replacements,
      mt::CR_PAIR<std::string> brackets = {"",""}
    );

    std::string solveName(
      mt::CR_INT collectionIndex,
      CR_LANGNUM sentencesIndex,
      mt::CR_VEC_STR replacements
    );

    std::string solveLevelName(
      mt::CR_INT collectionIndex,
      CR_LANGNUM sentencesIndex,
      mt::CR_PAIR<mt::VEC_STR> replacements
    );

    std::string solveNameType(
      mt::CR_INT collectionIndex,
      CR_LANGNUM sentencesIndex,
      mt::CR_PAIR<mt::VEC_STR> replacements
    );

    std::string solveLevelNameType(
      mt::CR_INT collectionIndex,
      CR_LANGNUM sentencesIndex,
      mt::CR_ARR<mt::VEC_STR, 3> replacements
    );
  };
}

#endif // __CLI_MENU__LANGUAGE_H__