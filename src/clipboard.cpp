#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.hpp"

namespace cli_menu {

  void Clipboard::pasteText(std::string &dataRef) {

    // activate clipboard
    if (!OpenClipboard(nullptr)) {

      Console::logResponse(
        CONSOLE_ERROR,
        "Failed to open clipboard."
      );

      return;
    }

    /** Get clipboard data */

    HANDLE hData = GetClipboardData(CF_TEXT);

    if (!hData) {
      Console::logResponse(
        CONSOLE_ERROR,
        "Failed to get clipboard data."
      );

      CloseClipboard();
      return;
    }

    /** Convert it to string */

    char *pszText = static_cast<char*>(GlobalLock(hData));

    if (!pszText) {
      Console::logResponse(
        CONSOLE_ERROR,
        "Failed to lock clipboard data."
      );
    }
    else GlobalUnlock(hData);

    // assign to reference
    dataRef = std::string(pszText);

    // done with clipboard
    CloseClipboard();

    Console::logResponse(
      CONSOLE_SUCCEED,
      "Pasted from clipboard."
    );
  }

  mt::VEC_LD Clipboard::pasteNumbers() {
    mt::VEC_LD numbers;
    std::string textRef;
    pasteText(textRef);

    mt_uti::Scanner<mt::LD>::parseNumbers(
      textRef, numbers
    );

    return numbers;
  }

  void Clipboard::pasteConditions(mt::VEC_BOL &conditionsRef) {
    bool pushed = false;

    Util::BOOL_ENUM boolEnum;
    mt::VEC_STR textVec {""};

    std::string textRef;
    pasteText(textRef);

    // truncated by spaces
    for (mt::CR_CH ch : textRef) {

      if (mt_uti::StrTools::isWhitespace(ch)) {
        if (!pushed) {
          textVec.push_back("");
          pushed = true;
        }
      }
      else {
        textVec.back() += ch;
        pushed = false;
      }
    }

    // parse booleans
    for (mt::CR_STR str : textVec) {
      boolEnum = Util::booleanTest(str);

      if (boolEnum != Util::BOOL_OTHER) {
        conditionsRef.push_back(
          Util::revealBoolean(boolEnum)
        );
      }
    }
  }

  HyphensDetector::HyphensDetector(CR_VEC_STR raws) {
    int ctr = 0;

    for (mt::CR_STR str : raws) {

      for (mt::CR_CH ch : raws) {
        if (ch == '-') ctr++;
        else break;
        if (ctr == 2) break;
      }

      if (ctr == 1) singleKeywords.push_back(str);
      else doubleKeywords.push_back(str);
      ctr = 0;
    }
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__