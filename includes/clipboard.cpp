#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.h"

namespace cli_menu {

  Clipboard::Clipboard(
    mt::CR_STR errorMessage_in
  ) {
    rule = [](uchar_ptr text)->bool { return true; };
    errorMessage = errorMessage_in;
  }

  Clipboard::Clipboard(
    rule_clbk rule_in,
    mt::CR_STR errorMessage_in
  ) {
    rule = rule_in;
    errorMessage = errorMessage_in;
  }

  // only for string copy
  void Clipboard::paste() {

    if (!OpenClipboard(nullptr)) {
      std::cerr << "\n__Failed to open clipboard" << std::endl;
      return;
    }

    HANDLE hData = GetClipboardData(CF_TEXT);

    if (hData == nullptr) {
      std::cerr << "\n__Failed to get clipboard data" << std::endl;
      CloseClipboard();
      return;
    }

    uchar_ptr pszText = static_cast<uchar_ptr>(GlobalLock(hData));

    if (pszText == nullptr) {
      std::cerr << "\n__Failed to lock clipboard data" << std::endl;
    }
    else GlobalUnlock(hData);

    CloseClipboard();

    if (!(rule(pszText) || errorMessage.empty())) {
      Message::printDialogError(errorMessage);
    }
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__