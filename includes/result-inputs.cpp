#ifndef __CLI_MENU__RESULT_INPUTS_CPP__
#define __CLI_MENU__RESULT_INPUTS_CPP__

#include <memory>
#include "result-inputs.h"

namespace cli_menu {

  std::string ResultInputs::title = "";

  const std::string ResultInputs::subtitles[3] = {
    "TEXTS", "NUMBERS", "CONDITIONS"
  };

  mt::VEC_STR ResultInputs::cutLongText(
    mt::CR_VEC_STR textVec,
    mt::CR_INT mostCharsCount
  ) {
    mt::VEC_STR retTextVec;

    const int maxLength = (
      Message::boundaryCharactersAmount
      - mostCharsCount - Message::frameSpacesCount
    );

    for (std::string text : textVec) {

      int excludedIndex = maxLength;
      const size_t newlineIndex = text.find('\n');

      if (newlineIndex != std::string::npos) {
        excludedIndex = newlineIndex - 1;

        if (excludedIndex > maxLength) {
          excludedIndex = maxLength;
        }
      }
      else if (text.length() <= maxLength) {
        retTextVec.push_back(text);
        continue;
      }

      retTextVec.push_back(
        text.substr(0, excludedIndex) + ".."
      );
    }

    return retTextVec;
  }

  void ResultInputs::printVector() {
    Message::printBoundaryLine(1, 1);

    if (!ResultInputs::title.empty()) {
      std::cout << "'" << ResultInputs::title << "' ";
    }

    Message::printString("ResultInputs", Color::SKY_BLUE);
    std::cout << " (" << ResultInputs::texts.size() << "):";
    Message::printBoundaryLine(1, 0);

    int mostCharsCount = 0;

    // specify number of spaces before the colon to tidy up
    for (mt::CR_STR nm : names) {
      if (nm.length() > mostCharsCount) {
        mostCharsCount = nm.length();
      }
    }

    // print the list (all inputs from commands)
    for (int i = 0; i < 3; i++) {

      Message::printString(
        "\n" + subtitles[i] + ": ", Color::LIGHT_GREEN
      );

      for (int j = 0; j < names.size(); j++) {
        std::cout
          << "\n  " << names[j]
          << std::string(mostCharsCount - names[j].length(), ' ')
          << " : [";

        // texts
        if (i == 0) {
          ResultInputs::printList<std::string>(
            cutLongText(texts[j], mostCharsCount), mostCharsCount
          );
        }
        // numbers
        else if (i == 1) {
          ResultInputs::printList<mt::LD>(
            numbers[j], mostCharsCount
          );
        }
        // conditions
        else ResultInputs::printList<bool>(
          conditions[j], mostCharsCount
        );

        std::cout << ']' << (j < names.size() - 1 ? ',' : '\0');
      }

      std::cout << std::endl;
    }

    Message::printBoundaryLine(0, 2);
  }

  void ResultInputs::printAt(
    RESULT_TYPE type,
    mt::CR_INT index
  ) {
    Message::printBoundaryLine(1, 1);
    Message::printString(subtitles[type] + ": ", Color::SKY_BLUE);
    std::cout << '[';

    // texts
    if (type == RESULT_TEXT) {
      ResultInputs::printList<std::string>(
        cutLongText(texts.size() > index ?
          texts[index] : mt::VEC_STR{}, 2
        ), 2
      );
    }
    // numbers
    else if (type == RESULT_NUMBER) {
      ResultInputs::printList<mt::LD>(
        numbers.size() > index ? numbers[index] : mt::VEC_LD{}, 4
      );
    }
    // conditions
    else ResultInputs::printList<bool>(
      conditions.size() > index ? conditions[index] : mt::VEC_BOL{}, 7
    );

    std::cout << ']';
    Message::printBoundaryLine(1, 2);
  }

  /** SETTERS */

  void ResultInputs::rename(mt::CR_INT i, mt::CR_STR name) {
    if (mt_uti::VecTools<std::string>::hasIndex(names, i)) {
      names[i] = name;
    }
  }

  void ResultInputs::addName(mt::CR_STR name) {
    names.push_back(name);
    texts.push_back({});
    numbers.push_back({});
    conditions.push_back({});
  }

  void ResultInputs::addTexts(mt::CR_STR name, mt::CR_VEC_STR vec) {
    names.push_back(name);
    texts.push_back(vec);
    numbers.push_back({});
    conditions.push_back({});
  }

  void ResultInputs::addNumbers(mt::CR_STR name, mt::CR_VEC_LD vec) {
    names.push_back(name);
    texts.push_back({});
    numbers.push_back(vec);
    conditions.push_back({});
  }

  void ResultInputs::addConditions(mt::CR_STR name, mt::CR_VEC_BOL vec) {
    names.push_back(name);
    texts.push_back({});
    numbers.push_back({});
    conditions.push_back(vec);
  }

  void ResultInputs::clearText(mt::CR_INT i) {
    texts[i] = {};
  }

  void ResultInputs::clearNumber(mt::CR_INT i) {
    numbers[i] = {};
  }

  void ResultInputs::clearCondition(mt::CR_INT i) {
    conditions[i] = {};
  }

  void ResultInputs::pushText(mt::CR_INT i, mt::CR_STR value) {
    if (mt_uti::VecTools<mt::VEC_STR>::hasIndex(texts, i)) {
      texts[i].push_back(value);
    }
  }

  void ResultInputs::pushNumber(mt::CR_INT i, mt::CR_LD value) {
    if (mt_uti::VecTools<mt::VEC_LD>::hasIndex(numbers, i)) {
      numbers[i].push_back(value);
    }
  }

  void ResultInputs::pushCondition(mt::CR_INT i, mt::CR_BOL value) {
    if (mt_uti::VecTools<mt::VEC_BOL>::hasIndex(conditions, i)) {
      conditions[i].push_back(value);
    }
  }

  void ResultInputs::pushTexts(mt::CR_INT i, mt::CR_VEC_STR vec) {
    if (mt_uti::VecTools<mt::VEC_STR>::hasIndex(texts, i)) {
      mt_uti::VecTools<std::string>::concat(texts[i], vec);
    }
  }

  void ResultInputs::pushNumbers(mt::CR_INT i, mt::CR_VEC_LD vec) {
    if (mt_uti::VecTools<mt::VEC_LD>::hasIndex(numbers, i)) {
      mt_uti::VecTools<mt::LD>::concat(numbers[i], vec);
    }
  }

  void ResultInputs::pushConditions(mt::CR_INT i, mt::CR_VEC_BOL vec) {
    if (mt_uti::VecTools<mt::VEC_BOL>::hasIndex(conditions, i)) {
      mt_uti::VecTools<bool>::concat(conditions[i], vec);
    }
  }

  void ResultInputs::cutName(mt::CR_INT i) {
    mt_uti::VecTools<std::string>::cutSingle(names, i);
    mt_uti::VecTools<mt::VEC_STR>::cutSingle(texts, i);
    mt_uti::VecTools<mt::VEC_LD>::cutSingle(numbers, i);
    mt_uti::VecTools<mt::VEC_BOL>::cutSingle(conditions, i);
  }

  void ResultInputs::cutText(mt::CR_INT i, mt::CR_INT j) {
    if (mt_uti::VecTools<mt::VEC_STR>::hasIndex(texts, i)) {
      mt_uti::VecTools<std::string>::cutSingle(texts[i], j);
    }
  }

  void ResultInputs::cutNumbers(mt::CR_INT i, mt::CR_INT j) {
    if (mt_uti::VecTools<mt::VEC_LD>::hasIndex(numbers, i)) {
      mt_uti::VecTools<mt::LD>::cutSingle(numbers[i], j);
    }
  }

  void ResultInputs::cutCondition(mt::CR_INT i, mt::CR_INT j) {
    if (mt_uti::VecTools<mt::VEC_BOL>::hasIndex(conditions, i)) {
      mt_uti::VecTools<bool>::cutSingle(conditions[i], j);
    }
  }

  void ResultInputs::popName() {
    names.pop_back();
    texts.pop_back();
    numbers.pop_back();
    conditions.pop_back();
  }

  void ResultInputs::popText(mt::CR_INT i) {
    if (mt_uti::VecTools<mt::VEC_STR>::hasIndex(texts, i)) {
      texts[i].pop_back();
    }
  }

  void ResultInputs::popNumbers(mt::CR_INT i) {
    if (mt_uti::VecTools<mt::VEC_LD>::hasIndex(numbers, i)) {
      numbers[i].pop_back();
    }
  }

  void ResultInputs::popCondition(mt::CR_INT i) {
    if (mt_uti::VecTools<mt::VEC_BOL>::hasIndex(conditions, i)) {
      conditions[i].pop_back();
    }
  }

  /** GETTERS */

  std::string ResultInputs::getName(mt::CR_INT i) {
    return mt_uti::VecTools<std::string>::getAt(names, i, "");
  }

  std::string ResultInputs::getText(mt::CR_INT i, mt::CR_INT j) {

    if (mt_uti::VecTools<mt::VEC_STR>::hasIndex(texts, i)) {
      return mt_uti::VecTools<std::string>::getAt(texts[i], j, "");
    }

    return "";
  }

  mt::LD ResultInputs::getNumber(mt::CR_INT i, mt::CR_INT j) {

    if (mt_uti::VecTools<mt::VEC_LD>::hasIndex(numbers, i)) {
      return mt_uti::VecTools<mt::LD>::getAt(numbers[i], j, 0);
    }

    return 0;
  }

  bool ResultInputs::getCondition(mt::CR_INT i, mt::CR_INT j) {

    if (mt_uti::VecTools<mt::VEC_BOL>::hasIndex(conditions, i)) {
      return mt_uti::VecTools<bool>::getAt(conditions[i], j, false);
    }

    return false;
  }

  mt::VEC_STR ResultInputs::getTexts(mt::CR_INT i) {
    return mt_uti::VecTools<mt::VEC_STR>::getAt(texts, i, {});
  }

  mt::VEC_LD ResultInputs::getNumbers(mt::CR_INT i) {
    return mt_uti::VecTools<mt::VEC_LD>::getAt(numbers, i, {});
  }

  mt::VEC_BOL ResultInputs::getConditions(mt::CR_INT i) {
    return mt_uti::VecTools<mt::VEC_BOL>::getAt(conditions, i, {});
  }

  bool ResultInputs::doesAnyVectorContain(mt::CR_INT index) {
    return (
      (mt_uti::VecTools<mt::VEC_STR>::hasIndex(texts, index) &&
        !texts[index].empty()
      ) ||
      (mt_uti::VecTools<mt::VEC_LD>::hasIndex(numbers, index) &&
        !numbers[index].empty()
      ) ||
      (mt_uti::VecTools<mt::VEC_BOL>::hasIndex(conditions, index) &&
        !conditions[index].empty()
      )
    );
  }
}

#endif // __CLI_MENU__RESULT_INPUTS_CPP__