#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include <string>
#include "language.hpp"

namespace cli_menu {

  /** English Presets */
  const std::string Language::defaultSentences[maxSentencesSize] = {

    // LANGERR_DIALOG_ALREADY_SELECTING,
    "already in selection mode",
    // LANGERR_DIALOG_BACK_AT_ROOT
    "no more groups above",
    // LANGERR_DIALOG_EMPTY_REQUIRED_ARGUMENTS
    "unable to ${TASK} without explicit arguments",
    // LANGERR_DIALOG_FORBIDDEN_HIDDEN_TEXT_PASTING
    "hidden text pasting is only available on insertion",
    // LANGERR_DIALOG_INCOMPLETE_ULTIMATE_PROCESS
    "unable to proceed until all required inputs are provided",
    // LANGERR_DIALOG_NO_ITEMS
    "this ${LEVEL} does not have any items",
    // LANGERR_DIALOG_NO_NEIGHBORS
    "this ${LEVEL} has no neighbors",
    // LANGERR_DIALOG_NOT_FOUND
    "${LEVEL} not found",
    // LANGERR_DIALOG_UNPROCESSED_DIRECT_INPUTS
    "unable to ${TASK} until the rest of direct inputs are processed",

    // LANGERR_MATCH_EMPTY_REQUIRED_ARGUMENTS
    "the ${LEVEL} named ${NAME} requires explicit arguments",
    // LANGERR_MATCH_NEEDS_DIALOG_FOR_MANY
    "the ${NAME} ${LEVEL} has ${LEVEL} that need to be used",
    // LANGERR_MATCH_NEEDS_DIALOG_FOR_ONE
    "the ${NAME} ${LEVEL} has ${LEVEL} named ${NAME} that need to be used",
    // LANGERR_MATCH_UNKNOWN_NEIGHBOR
    "unknown ${LEVEL} named ${NAME}"
  };

  mt::VEC<Language::LANGARR> Language::sentences = {};
  mt::VEC<std::string> Language::collections = {};
  int Language::currentCollectionIndex = 0;

  std::string Language::textCopy = "",
    Language::levelPlaceholder = "${LEVEL}",
    Language::namePlaceholder = "${NAME}",
    Language::taskPlaceholder = "${TASK}";

  bool Language::hasIndex(mt::CR_INT collectionIndex) {
    return mt_uti::VecTools<std::string>::hasIndex(
      collections, collectionIndex
    ) != -1;
  }

  void Language::reset() {
    sentences = {LANGARR{""}};

    for (int i = 0; i < maxSentencesSize; i++) {
      sentences[0][i] = defaultSentences[i];
    }
  }

  void Language::addCollection(mt::CR_STR name) {
    if (mt_uti::VecTools<std::string>::getIndex(
      collections, name
    ) != -1) {
      collections.push_back(name);
      sentences.push_back(LANGARR{""});
    }
  }

  void Language::switchCollection(mt::CR_INT collectionIndex) {
    if (hasIndex(collectionIndex)) {
      currentCollectionIndex = collectionIndex;
    } 
  }

  void Language::removeCollection(mt::CR_INT collectionIndex) {

    mt_uti::VecTools<std::string>::cutSingle(
      collections,
      collectionIndex,
      true
    );

    mt_uti::VecTools<LANGARR>::cutSingle(
      sentences,
      collectionIndex,
      true
    );
  }

  void Language::translate(
    mt::CR_INT collectionIndex,
    CR_LANGNUM sentencesIndex,
    mt::CR_STR sentence
  ) {
    if (hasIndex(collectionIndex)) {
      sentences[collectionIndex][sentencesIndex] = sentence;
    }
  }

  void Language::initTextCopy(CR_LANGNUM sentencesIndex) {
    textCopy = sentences[currentCollectionIndex][sentencesIndex];
  }

  void Language::solveTemplate(
    mt::CR_STR placeholder,
    mt::CR_VEC_STR replacements,
    mt::CR_PAIR<char> wordBracket
  ) {
    size_t foundIndex;

    for (mt::CR_STR word : replacements) {
      foundIndex = textCopy.find(placeholder);

      if (foundIndex != std::string::npos) {

        // insert 'word' before 'placeholder'
        textCopy.insert(
          foundIndex,
          wordBracket.first + word + wordBracket.second
        );

        // index moved right after the 'word'
        foundIndex += (
          bool(wordBracket.first)
          + word.length()
          + bool(wordBracket.second)
        );

        // remove 'placeholder' from 'textCopy'
        textCopy = textCopy.substr(0, foundIndex)
          + textCopy.substr(foundIndex + placeholder.length());
      }
    }
  }

  void Language::solvePlaceholders(
    CR_LANGNUM sentencesIndex,
    mt::CR_VEC_STR levelReplacements,
    mt::CR_VEC_STR nameReplacements,
    mt::CR_VEC_STR taskReplacements
  ) {
    initTextCopy(sentencesIndex);

    solveTemplate(
      levelPlaceholder, levelReplacements
    );

    solveTemplate(
      namePlaceholder,
      nameReplacements,
      {'\'','\''}
    );

    solveTemplate(
      taskPlaceholder, taskReplacements
    );
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__