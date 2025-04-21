#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include "base.hpp"

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
      levelPlaceholder, namePlaceholder, taskPlaceholder;

    static bool hasIndex(mt::CR_INT collectionIndex);

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

    /**
     * Call the 'mt_uti::StrTools::tidyUp'
     * after this for neater text.
     */
    static void solveTemplate(
      mt::CR_STR placeholder,
      mt::CR_VEC_STR replacements,
      mt::CR_PAIR<char> wordBracket = {'\0','\0'}
    );

    static void solvePlaceholders(
      CR_LANGNUM sentencesIndex,
      mt::CR_VEC_STR levelReplacements = {},
      mt::CR_VEC_STR nameReplacements = {},
      mt::CR_VEC_STR typeReplacements = {}
    );

    /** Below are case sensitive */

    static void setLevelPlaceholder(mt::CR_STR value) {
      levelPlaceholder = value;
    }

    static void setNamePlaceholder(mt::CR_STR value) {
      namePlaceholder = value;
    }

    static void setTaskPlaceholder(mt::CR_STR value) {
      taskPlaceholder = value;
    }
  };
}

#endif // __CLI_MENU__LANGUAGE_HPP__