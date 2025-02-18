#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.h"

namespace cli_menu {

  bool Clipboard::internalCalling = false;

  void Clipboard::printSucceed(mt::CR_INT endNewlinesCount) {

    Message::printNeatDialog(
      MESSAGE_SUCCEED,
      "pasted from clipboard",
      endNewlinesCount
    );

    internalCalling = false;
  }

  void Clipboard::pasteText(std::string &dataRef) {

    // activate clipboard
    if (!OpenClipboard(nullptr)) {

      Message::printNeatDialog(
        MESSAGE_ERROR,
        "failed to open clipboard"
      );

      return;
    }

    /** Get clipboard data */

    HANDLE hData = GetClipboardData(CF_TEXT);

    if (!hData) {
      Message::printNeatDialog(
        MESSAGE_ERROR,
        "failed to get clipboard data"
      );

      CloseClipboard();
      return;
    }

    /** Convert it to string */

    char *pszText = static_cast<char*>(GlobalLock(hData));

    if (!pszText) {
      Message::printNeatDialog(
        MESSAGE_ERROR,
        "failed to lock clipboard data"
      );
    }
    else GlobalUnlock(hData);

    // assign to reference
    dataRef = std::string(pszText);

    // done with clipboard
    CloseClipboard();

    if (!internalCalling) {
      printSucceed(2);
    }
  }

  void Clipboard::pasteNumbers(mt::VEC_LD &numbersRef) {
    internalCalling = true;

    std::string textRef;
    pasteText(textRef);

    mt_uti::Scanner<mt::LD>::parseNumbers(
      textRef, numbersRef
    );

    printSucceed(!numbersRef.empty() + 1);
  }

  void Clipboard::pasteConditions(mt::VEC_BOL &conditionsRef) {
    internalCalling = true;

    bool pushed = false;
    Util::BOOL_ENUM boolEnum;
    mt::VEC_STR textVec {""};

    std::string textRef;
    pasteText(textRef);

    // truncated by spaces
    for (mt::CR_CH ch : textRef) {

      if (Util::isWhitespace(ch)) {
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

    printSucceed(!conditionsRef.empty() + 1);
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__