#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__QUICK_SORT_H__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__QUICK_SORT_H__

#include "../../types.h"

namespace mini_tools {
namespace algorithms {
namespace sorters {

  template <class T>
  class Quick {
  private:
    /** The 'left' and 'right' are indexes of 'messy' */

    static int hoarePartition(
      std::vector<T> *messy,
      CR_INT left,
      CR_INT right,
      bool &ascending
    );

    static int randomPartition(
      std::vector<T> *messy,
      CR_INT left,
      CR_INT right,
      bool &ascending
    );

    static void recursion(
      std::vector<T> *messy,
      CR_INT left,
      CR_INT right,
      bool &ascending
    );

  public:
    static void solve(
      std::vector<T> *messy,
      bool ascending = true
    );

    static std::vector<T> solve(
      std::vector<T> messy,
      bool ascending = true
    );
  };
}}}

#include "quick-sort.cpp"
#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__QUICK_SORT_H__