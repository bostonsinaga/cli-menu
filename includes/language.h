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
    static const int maxSentencesSize = 13;
    typedef mt::ARR<std::string, maxSentencesSize> LANGARR;

    static const std::string defaultSentences[maxSentencesSize];
    static mt::VEC<LANGARR> sentences;
    static mt::VEC<std::string> collections;
    static int currentCollectionIndex;

    static std::string textCopy,
      levelPlaceholder, namePlaceholder, typePlaceholder;

    static bool hasIndex(mt::CR_INT collectionIndex) {
      return mt_uti::VecTools<std::string>::hasIndex(
        collections, collectionIndex
      ) != -1;
    }

  public:
    static int getCurrentCollectionIndex() {
      return currentCollectionIndex;
    }

    static std::string getTextCopy() {
      return textCopy;
    }

    // call this before using 'solveTemplate'
    static void initTextCopy(CR_LANGNUM sentencesIndex);

    // first called for 'sentences' initialization
    static void reset();

    static void addCollection(mt::CR_STR name);
    static void switchCollection(mt::CR_INT collectionIndex);
    static void removeCollection(mt::CR_INT collectionIndex);

    static void translate(
      mt::CR_INT collectionIndex,
      CR_LANGNUM sentencesIndex,
      mt::CR_STR sentence
    );

    static void solveTemplate(
      CR_LANGNUM sentencesIndex,
      mt::CR_STR placeholder,
      mt::CR_VEC_STR replacements,
      mt::CR_PAIR<std::string> brackets = {"",""}
    );

    static void solvePlaceholders(
      CR_LANGNUM sentencesIndex,
      mt::CR_VEC_STR levelReplacements = {},
      mt::CR_VEC_STR nameReplacements = {},
      mt::CR_VEC_STR typeReplacements = {}
    );
  };
}

#endif // __CLI_MENU__LANGUAGE_H__