#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.hpp"

namespace cli_menu {

  std::string Clipboard::pasteText() {

    // activate clipboard
    if (!OpenClipboard(nullptr)) {

      Language::printResponse(
        LANGUAGE_CLIPBOARD_OPEN_FAILURE
      );

      return "";
    }

    /** Get clipboard data */

    HANDLE hData = GetClipboardData(CF_TEXT);

    if (!hData) {
      Language::printResponse(
        LANGUAGE_CLIPBOARD_GET_FAILURE
      );

      CloseClipboard();
      return "";
    }

    /** Convert 'hData' to string */

    char *pszText = static_cast<char*>(GlobalLock(hData));

    if (!pszText) {
      Language::printResponse(
        LANGUAGE_CLIPBOARD_LOCK_FAILURE
      );
    }
    else GlobalUnlock(hData);

    // copy for safety
    std::string pasted = std::string(pszText);

    // done with clipboard
    CloseClipboard();

    Language::printResponse(
      LANGUAGE_CLIPBOARD_PASTED
    );

    return pasted;
  }

  mt::VEC_LD Clipboard::pasteNumbers() {
    return mt_uti::Scanner::parseNumbers<mt::LD>(pasteText());
  }

  mt::VEC_BOL Clipboard::pasteConditions() {

    mt::VEC_BOL conditions;
    bool pushed = false;

    mt::VEC_STR textVec {""};
    std::string text = pasteText();

    // truncated by spaces
    for (mt::CR_CH ch : text) {

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
      conditions.push_back(Control::booleanizerTest(str));
    }

    return conditions;
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__