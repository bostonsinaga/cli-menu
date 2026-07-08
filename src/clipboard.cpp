#ifndef __CLI_MENU__CLIPBOARD_CPP__
#define __CLI_MENU__CLIPBOARD_CPP__

#include <windows.h>
#include "clipboard.hpp"

namespace cli_menu {

  void Clipboard::copy(
    mt::CR_UI formatID,
    const void* pData,
    mt::CR_SZ size
  ) {
    if (OpenClipboard(nullptr)) {
      EmptyClipboard();
      HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, size);

      if (hGlob) {
        void *pMem = GlobalLock(hGlob);

        if (pMem) {
          memcpy(pMem, pData, size);
          GlobalUnlock(hGlob);
          SetClipboardData(formatID, hGlob);
          Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_COPY_SUCCEED);
        }
        else {
          GlobalFree(hGlob);
          Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_GLOBAL_LOCK_FAILURE);
        }
      }
      else Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_GLOBAL_ALLOC_FAILURE);

      CloseClipboard();
    }
    else Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_OPEN_FAILURE);
  }

  void Clipboard::paste(
    mt::CR_UI formatID,
    mt::CR<std::function<void(void*)>> usePMem
  ) {
    bool succeeded = false;

    if (OpenClipboard(nullptr)) {
      HANDLE hData = GetClipboardData(formatID);

      if (hData) {
        void *pMem = GlobalLock(hData);

        if (pMem) {
          succeeded = true;
          usePMem(pMem);
          GlobalUnlock(hData);
          Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_PASTE_SUCCEED);
        }
        else {
          GlobalFree(hData);
          Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_GLOBAL_LOCK_FAILURE);
        }
      }
      else Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_GET_DATA_FAILURE);

      CloseClipboard();
    }
    else Langu::ageMessage::printResponse(SENTENCE_CLIPBOARD_OPEN_FAILURE);
  }

  /** Text Special */

  void Clipboard::copyText(std::string *strPtr) {
    Clipboard::copy(CF_TEXT, strPtr, strPtr->length());
  }

  std::string Clipboard::pasteText() {
    std::string str;

    Clipboard::paste(CF_TEXT, [&](void *pMem) {
      str = static_cast<char*>(pMem);
    });

    return str;
  }
}

#endif // __CLI_MENU__CLIPBOARD_CPP__