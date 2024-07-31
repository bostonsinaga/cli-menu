#ifndef __MINI_TOOLS__ALGORITHMS__SORTERS__SELECTION_SORT_CPP__
#define __MINI_TOOLS__ALGORITHMS__SORTERS__SELECTION_SORT_CPP__

namespace mini_tools {
namespace algorithms {
namespace sorters {

	template <typename T>
  void Selection<T>::solve(std::vector<T> *messy, bool ascending) {
    if constexpr (CheckType::isNumber<T>()) {
      int i, j, least; 

      for (i = 0; i < messy->size() - 1; i++) {
        least = i; 

        for (j = i + 1; j < messy->size(); j++) {
          if ((ascending && messy->at(j) < messy->at(least)) ||
            (!ascending && messy->at(j) > messy->at(least))
          ) { least = j; }
        }

        std::swap(messy->at(least), messy->at(i));
      }
    }
  }

  template <typename T>
  std::vector<T> Selection<T>::solve(std::vector<T> messy, bool ascending) {
    Selection<T>::solve(&messy, ascending);
    return messy;
  }
}}}

#endif // __MINI_TOOLS__ALGORITHMS__SORTERS__SELECTION_SORT_CPP__