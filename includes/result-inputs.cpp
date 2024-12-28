#ifndef __CLI_MENU__RESULT_INPUTS_CPP__
#define __CLI_MENU__RESULT_INPUTS_CPP__

#include "result-inputs.h"

namespace cli_menu {

  std::string ResultInputs::title = "";

  void ResultInputs::printVector() {
    Message::printBoundaryLine(1, 1);

    if (!ResultInputs::title.empty()) {
      std::cout << "'" << ResultInputs::title << "' ";
    }

    std::cout << "ResultInputs (" << ResultInputs::texts.size() << "):";
    Message::printBoundaryLine(1, 0);

    static const std::string subtitles[3] = {
      "TEXTS", "NUMBERS", "CONDITIONS"
    };

    int mostCharsCount = 0;

    // specify number of spaces before the colon to tidy up
    for (mt::CR_STR nm : names) {
      if (nm.length() > mostCharsCount) {
        mostCharsCount = nm.length();
      }
    }

    // print the list (all inputs from commands)
    for (int i = 0; i < 3; i++) {

      std::cout << "\n" << subtitles[i] << ": ";

      for (int j = 0; j < names.size(); j++) {
        std::cout
          << "\n  " << names[j]
          << std::string(mostCharsCount - names[j].length(), ' ')
          << " : [";

        if (i == 0) {
          ResultInputs::printList<std::string>(texts[j], mostCharsCount);
        }
        else if (i == 1) {
          ResultInputs::printList<mt::LD>(numbers[j], mostCharsCount);
        }
        else ResultInputs::printList<bool>(conditions[j], mostCharsCount);

        std::cout << ']' << (j < names.size() - 1 ? ',' : '\0');
      }

      std::cout << std::endl;
    }

    Message::printBoundaryLine(0, 1);
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
}

#endif // __CLI_MENU__RESULT_INPUTS_CPP__