#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.h"

namespace cli_menu {

  template<>
  void Clipboard::paste() {

    // activate clipboard
    if (!OpenClipboard(nullptr)) {
      std::cerr << "\n__Failed to open clipboard" << std::endl;
      return "";
    }

    /** Get clipboard data */

    HANDLE hData = GetClipboardData(CF_TEXT);

    if (!hData) {
      Message::printNeatDialog(
        Message::ERROR_FLAG,
        "failed to get clipboard data"
      );

      CloseClipboard();
      return "";
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

    std::string text(pszText);

    // done with clipboard
    CloseClipboard();

    // print error message
    if (!rule(text)) {
      if (errorMessage.empty()) {
        Message::printNeatDialog(
          Message::ERROR_FLAG,
          "invalid clipboard content", 0
        );
      }
      else Message::printNeatDialog(
        Message::ERROR_FLAG,
        errorMessage
      );
    }
    else Message::printNeatDialog(
      Message::SUCCEED_FLAG,
      "pasted from clipboard"
    );

    // the product
    return text;
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__