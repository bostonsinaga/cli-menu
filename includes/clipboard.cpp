#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.h"

namespace cli_menu {

  Clipboard::Clipboard(
    mt::CR_STR errorMessage_in
  ) {
    rule = [](mt::CR_STR text)->bool { return true; };
    errorMessage = errorMessage_in;
  }

  Clipboard::Clipboard(
    rule_clbk rule_in,
    mt::CR_STR errorMessage_in
  ) {
    rule = rule_in;
    errorMessage = errorMessage_in;
  }

  std::string Clipboard::paste() {

    // activate clipboard
    if (!OpenClipboard(nullptr)) {
      std::cerr << "\n__Failed to open clipboard" << std::endl;
      return "";
    }

    /** Get clipboard data */

    HANDLE hData = GetClipboardData(CF_TEXT);

    if (hData == nullptr) {
      std::cerr << "\n__Failed to get clipboard data" << std::endl;
      CloseClipboard();
      return "";
    }

    /** Convert it to string */

    char *pszText = static_cast<char*>(GlobalLock(hData));

    if (pszText == nullptr) {
      std::cerr << "\n__Failed to lock clipboard data" << std::endl;
    }
    else GlobalUnlock(hData);

    std::string text(pszText);

    // done with clipboard
    CloseClipboard();

    // print error message
    if (!(rule(text) || errorMessage.empty())) {
      Message::printDialogError(errorMessage);
    }

    // the product
    return text;
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__