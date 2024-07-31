#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__COMB_SORT_H__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__COMB_SORT_H__

#include "../../types.h"

namespace mini_tools {
namespace algorithms {
namespace sorters {

  template <class T>
  class Comb {
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

#include "comb-sort.cpp"
#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__COMB_SORT_H__