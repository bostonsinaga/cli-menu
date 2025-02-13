#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.h"

namespace cli_menu {

  void Clipboard::printSucceed() {
    Message::printNeatDialog(
      Message::SUCCEED_FLAG,
      "pasted from clipboard"
    );
  }

  void Clipboard::pasteText(std::string &dataRef) {

    // activate clipboard
    if (!OpenClipboard(nullptr)) {

      Message::printNeatDialog(
        Message::ERROR_FLAG,
        "failed to open clipboard"
      );

      return;
    }

    /** Get clipboard data */

    HANDLE hData = GetClipboardData(CF_TEXT);

    if (!hData) {
      Message::printNeatDialog(
        Message::ERROR_FLAG,
        "failed to get clipboard data"
      );

      CloseClipboard();
      return;
    }

    /** Convert it to string */

    char *pszText = static_cast<char*>(GlobalLock(hData));

    if (!pszText) {
      Message::printNeatDialog(
        Message::ERROR_FLAG,
        "failed to lock clipboard data"
      );
    }
    else GlobalUnlock(hData);

    // assign to reference
    dataRef = std::string(pszText);

    // done with clipboard
    CloseClipboard();

    printSucceed();
  }

  void Clipboard::pasteNumbers(mt::VEC_LD &numbersRef) {
    std::string textRef;
    pasteText(textRef);
    mt_uti::Scanner<mt::LD>::parseNumbers(textRef, numbersRef);
  }

  void Clipboard::pasteConditions(mt::VEC_BOL &conditionsRef) {

    Util::BOOL_FLAG boolFlag;
    std::string textRef;
    mt::VEC_STR textVec {""};

    pasteText(textRef);

    // truncated by spaces
    for (mt::CR_CH ch : textRef) {
      if (Util::isWhitespace(ch)) {
        textVec.push_back("");
      }
      else textVec.back() += ch;
    }

    // parse booleans
    for (mt::CR_STR str : textVec) {
      boolFlag = Util::booleanTest(str);

      if (boolFlag) {
        conditionsRef.push_back(
          Util::revealBoolean(boolFlag)
        );
      }
    }
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__