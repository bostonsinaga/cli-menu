#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include <string>
#include "language.h"

namespace cli_menu {

  /** English Presets */
  const std::string Language::DEFAULT_SENTENCES[max_sentences_size] = {

    // LANGERR_DIALOG_ALREADY_SELECTING,
    "already in selection mode",
    // LANGERR_DIALOG_BACK_AT_ROOT
    "no more groups above",
    // LANGERR_DIALOG_EMPTY_REQUIRED_ARGUMENTS
    "unable to #TASK without explicit arguments",
    // LANGERR_DIALOG_FORBIDDEN_HIDDEN_TEXT_PASTING
    "hidden text pasting is only available on insertion",
    // LANGERR_DIALOG_INCOMPLETE_ULTIMATE_PROCESS
    "unable to proceed until all required inputs are provided",
    // LANGERR_DIALOG_NO_ITEMS
    "this #LEVEL does not have any items",
    // LANGERR_DIALOG_NO_NEIGHBORS
    "this #LEVEL has no neighbors",
    // LANGERR_DIALOG_NOT_FOUND
    "#LEVEL not found",
    // LANGERR_DIALOG_UNPROCESSED_DIRECT_INPUTS
    "unable to #TASK until the rest of direct inputs are processed",

    // LANGERR_MATCH_EMPTY_REQUIRED_ARGUMENTS
    "the #LEVEL named #NAME requires explicit arguments",
    // LANGERR_MATCH_NEEDS_DIALOG_FOR_MANY
    "the #NAME #LEVEL has #LEVEL that need to be used",
    // LANGERR_MATCH_NEEDS_DIALOG_FOR_ONE
    "the #NAME #LEVEL has #LEVEL named #NAME that need to be used",
    // LANGERR_MATCH_UNKNOWN_NEIGHBOR
    "unknown #LEVEL named #NAME"
  };

  std::string Language::LEVEL_PLACEHOLDER = "#LEVEL",
    Language::NAME_PLACEHOLDER = "#NAME",
    Language::TYPE_PLACEHOLDER = "#TYPE";

  void Language::addCollection(mt::CR_STR name) {
    LANGARR newArr;
    newArr.fill("");

    if (mt_uti::VecTools<PAIR_LANGARR>::getIndex(
      SENTENCES_COLLECTION, {name, newArr},
      [](mt::CR<PAIR_LANGARR> rep, mt::CR<PAIR_LANGARR> com)->bool {
        return rep.first == com.first;
      }
    ) != -1) {
      SENTENCES_COLLECTION.push_back({
        name, newArr
      });
    }
  }

  void Language::removeCollection(mt::CR_INT collectionIndex) {
    mt_uti::VecTools<PAIR_LANGARR>::cutSingle(
      SENTENCES_COLLECTION,
      collectionIndex,
      true
    );
  }

  void Language::translate(
    mt::CR_INT collectionIndex,
    CR_LANGNUM sentencesIndex,
    mt::CR_STR sentence
  ) {
    if (mt_uti::VecTools<PAIR_LANGARR>::hasIndex(
      SENTENCES_COLLECTION, collectionIndex
    )) {
      SENTENCES_COLLECTION[collectionIndex].second[sentencesIndex] = sentence;
    }
  }

  std::string Language::solveTemplate(
    mt::CR_INT collectionIndex,
    CR_LANGNUM sentencesIndex,
    mt::CR_STR placeholder,
    mt::CR_VEC_STR replacements,
    mt::CR_PAIR<std::string> brackets
  ) {
    std::string textCopy = SENTENCES_COLLECTION[collectionIndex].second[sentencesIndex];
    size_t textIndex;

    for (mt::CR_STR word : replacements) {
      textIndex = textCopy.find(placeholder);

      if (textIndex != std::string::npos) {

        // insert 'word' before 'placeholder'
        textCopy.insert(textIndex, brackets.first + word + brackets.second);
        textIndex += brackets.first.length() + word.length() + brackets.second.length();

        // remove 'placeholder' from sentence
        return textCopy.substr(0, textIndex)
          + textCopy.substr(textIndex + placeholder.length());
      }
    }

    return textCopy;
  }

  std::string Language::solveName(
    mt::CR_INT collectionIndex,
    CR_LANGNUM sentencesIndex,
    mt::CR_VEC_STR replacements
  ) {
    return solveTemplate(
      collectionIndex, sentencesIndex,
      NAME_PLACEHOLDER, replacements,
      {"'","'"}
    );
  }

  std::string Language::solveLevelName(
    mt::CR_INT collectionIndex,
    CR_LANGNUM sentencesIndex,
    mt::CR_PAIR<mt::VEC_STR> replacements
  ) {
    std::string textCopy = solveTemplate(
      collectionIndex, sentencesIndex,
      LEVEL_PLACEHOLDER, replacements.first
    );

    return solveName(
      collectionIndex, sentencesIndex, replacements.second
    );
  }

  std::string Language::solveNameType(
    mt::CR_INT collectionIndex,
    CR_LANGNUM sentencesIndex,
    mt::CR_PAIR<mt::VEC_STR> replacements
  ) {
    std::string textCopy = solveName(
      collectionIndex, sentencesIndex, replacements.first
    );

    return solveTemplate(
      collectionIndex, sentencesIndex,
      TYPE_PLACEHOLDER, replacements.second
    );
  }

  std::string Language::solveLevelNameType(
    mt::CR_INT collectionIndex,
    CR_LANGNUM sentencesIndex,
    mt::CR_ARR<mt::VEC_STR, 3> replacements
  ) {
    std::string textCopy = solveTemplate(
      collectionIndex, sentencesIndex,
      LEVEL_PLACEHOLDER, replacements[0]
    );

    textCopy += solveName(
      collectionIndex, sentencesIndex, replacements[1]
    );

    return solveTemplate(
      collectionIndex, sentencesIndex,
      TYPE_PLACEHOLDER, replacements[2]
    );
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__