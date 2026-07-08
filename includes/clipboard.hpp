#ifndef __CLI_MENU__CLIPBOARD_HPP__
#define __CLI_MENU__CLIPBOARD_HPP__

#include "control.hpp"

namespace cli_menu {

  class Clipboard final {
  private:
    static void paste(
      mt::CR_UI formatID,
      mt::CR<std::function<void(void*)>> usePMem
    );

  public:
    static void copy(
      mt::CR_UI formatID,
      const void* pData,
      mt::CR_SZ size
    );

    template <typename T>
    static T paste(
      mt::CR_UI formatID,
      mt::CR<std::function<T()>> customDefaultValueGetter = nullptr
    ) {
      T data = mt::getDefaultFromType<T>(customDefaultValueGetter);

      Clipboard::paste(formatID, [&](void *pMem) {
        data = static_cast<T>(pMem);
      });

      return data;
    }

    // text special
    static void copyText(std::string *strPtr);
    static std::string pasteText();
  };
}

#endif // __CLI_MENU__CLIPBOARD_HPP__