#ifndef __ALGORITHMS__CALCULATORS_H__
#define __ALGORITHMS__CALCULATORS_H__

#include <cmath>
#include <algorithm>

namespace mini_tools {
namespace algorithms {
  class Calc {
  public:
    static int factorial(int n) {
      int ret = n;

      while (n > 2) {
        ret *= n-1;
        n--;
      }

      return ret;
    }

    /**
    * Stands of parameters:
    *
    * a = first number
    * an = last number
    * d = difference
    * o = order
    * r = ratio
    * q = quantity
    */

    static int arithmeticSeries(int a, int an, int q) {
      return q * ((a + an) / 2);
    }

    static int geometricSeries(int a, int r, int q) {
      return a * (1 - std::pow(r, q)) / (1 - r);
    }

    static int arithmeticSequence(int a, int o, int d) {
      return a + (o - 1) * d;
    }

    static int geometricSequence(int a, int r, int o) {
      return a * std::pow(r, o - 1);
    }

    static bool isArithmeticSequence(std::vector<int> &nums, int d) {
      if (nums.size() == 0) return false;

      for (int i = 0; i < nums.size() - 1; i++) {
        if (nums[i] + d != nums[i+1]) return false;
      }

      return true;
    }

    static bool isArithmeticSequence(const std::vector<int> &nums, int d) {
      if (nums.size() == 0) return false;

      for (int i = 0; i < nums.size() - 1; i++) {
        if (nums[i] + d != nums[i+1]) return false;
      }

      return true;
    }

    static bool isGeometricSequence(std::vector<int> &nums, int r) {
      if (nums.size() == 0) return false;

      for (int i = 0; i < nums.size() - 1; i++) {
        if (nums[i] * r != nums[i+1]) return false;
      }

      return true;
    }

    static bool isGeometricSequence(const std::vector<int> &nums, int r) {
      if (nums.size() == 0) return false;

      for (int i = 0; i < nums.size() - 1; i++) {
        if (nums[i] * r != nums[i+1]) return false;
      }

      return true;
    }

    /**
     * Returns true if 'nums' is single
     * or 'startIndex' is the last when 'ascending' true
     * or 'startIndex' is the first when 'ascending' false
     */
    static bool isSorted(
      std::vector<int> &nums,
      bool ascending = true,
      int startIndex = 0,
      int partialTestCount = 0,
      int skipTestCount = 0
    ) {
      int last = nums.size() - 1;
      if (last < 0) return true;

      auto limitPartialTestCount = [&]() {
        if (partialTestCount <= 0 ||
          partialTestCount > nums.size()
        ) { partialTestCount = nums.size(); }
      };

      // bad arguments
      if (startIndex < 0 || startIndex > last) return false;
      limitPartialTestCount();

      // this function will not change the 'nums'
      std::vector<int> backup;

      if (skipTestCount > 0) {

        // limit 'skipTestCount'
        if (skipTestCount + startIndex > last) {
          skipTestCount = last - startIndex;
        }

        backup = nums;
        nums = {};

        // skip 'nums'
        for (int i = startIndex; i <= last; i += skipTestCount + 1) {
          nums.push_back(backup[i]);
        }

        last = nums.size() - 1;
        limitPartialTestCount();
      }

      if (startIndex >= last) return true;
      partialTestCount--;

      for (int i = 0; i < last; i += partialTestCount) {

        int nextCount = i + partialTestCount;
        if (nextCount > last) nextCount = last;

        if (ascending) {
          if (!std::is_sorted(
            nums.begin() + i, nums.begin() + nextCount + 1
          )) { return false; }
        }
        else {
          if (!std::is_sorted(
            nums.begin() + i, nums.begin() + nextCount + 1, std::greater<int>()
          )) { return false; }
        }
      }

      if (backup.size() > 0) nums = backup;
      return true;
    }
  };
}}

#endif // __ALGORITHMS__CALCULATORS_H__