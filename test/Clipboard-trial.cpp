#include <windows.h>
#include <iostream>

int main() {
  /** Open the Clipboard */

  // only fot string copy
  if (!OpenClipboard(nullptr)) {
    std::cerr << "Failed to open clipboard" << std::endl;
    return 1;
  }

  /** Get Clipboard Data */

  HANDLE hData = GetClipboardData(CF_TEXT);

  if (hData == nullptr) {
    std::cerr << "Failed to get clipboard data" << std::endl;
    CloseClipboard();
    return 1;
  }

  /** Lock the Clipboard Data */

  char* pszText = static_cast<char*>(GlobalLock(hData));

  if (pszText == nullptr) {
    std::cerr << "Failed to lock clipboard data" << std::endl;
    CloseClipboard();
    return 1;
  }

  /** Use the Clipboard Data */

  // std::cout << "Clipboard data: " << pszText << std::endl;
  GlobalUnlock(hData);

  /** Close the Clipboard */

  CloseClipboard();

  // print 'pszText' length (super fast)
  std::cout
    << "\n\x1B[38;2;63;255;63mSTRING LENGTH = "
    << strlen(pszText)
    << "\x1B[0m\n";

  return 0;
}

