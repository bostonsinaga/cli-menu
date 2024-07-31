#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__MERGE_SORT_H__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__MERGE_SORT_H__

#include "../../types.h"

namespace mini_tools {
namespace algorithms {
namespace sorters {

  template <class T>
  class Merge {
  private:
    static void merge(
      std::vector<T> *messy,
      CR_INT left,
      CR_INT mid,
      CR_INT right,
      bool ascending
    );

    static void partition(
      std::vector<T> *messy,
      CR_INT begin,
      CR_INT end,
      bool ascending
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

#include "merge-sort.cpp"
#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__MERGE_SORT_H__