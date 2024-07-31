#ifndef __MINI_TOOLS__UTILS__READER_H__
#define __MINI_TOOLS__UTILS__READER_H__

#include "../types.h"

namespace mini_tools {
namespace utils {

  namespace Reader {
    bool isSeparator(char &ch);

    void getFrom(
      std::string &filename,
      std::string &textHook
    );
  }

  template <class T>
  class Scanner {
  public:
    static std::vector<T> txtToNumbers(std::string filename);
    static std::vector<T> txtToLetters(std::string filename);
  };
}}

#include "reader.cpp"
#endif // __MINI_TOOLS__UTILS__READER_H__