#ifndef __MINI_TOOLS__UTILS__TIMER_H__
#define __MINI_TOOLS__UTILS__TIMER_H__

#include <iostream>
#include <chrono>

namespace mini_tools {
namespace utils {

  class Timer {
  private:
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> TP;
    std::chrono::duration<float> difference;
    TP prior;

  public:
    void reset() {
      prior = std::chrono::high_resolution_clock::now();
      difference = std::chrono::nanoseconds::zero();
    }

    Timer() { reset(); }

    void check() {
      TP now = std::chrono::high_resolution_clock::now();
      difference = now - prior;
      prior = now;
    }

    float getSeconds(bool needCheck = true) {
      if (needCheck) check();
      return difference.count();
    }

    float getMilliseconds(bool needCheck = true) {
      if (needCheck) check();
      return std::chrono::duration_cast<std::chrono::milliseconds>(difference).count();
    }

    void print(bool needCheck = true, std::string title = "DURATION:") {
      if (needCheck) check();
      std::cout << title << std::endl;
      std::cout << getSeconds(false) << "s\n";
      std::cout << getMilliseconds(false) << "ms\n";
    }
  };
}}

#endif // __MINI_TOOLS__UTILS__TIMER_H__