#include <iostream>
#include "cli-menu.h"

void foo(mt::CR_VEC_STR, mt::CR_VEC_BOL) {
  std::cout << "\nFoo is called..";
}

int main(int argc, char *argv[]) {

  cm::Command *com = new cm::Command(
    "conversation",
    "simple dialog",
    new cm::Parameters(
      mt::VEC_STR{"say"}, 
      mt::VEC_BOL{cm::Parameters::TEXT}, 
      mt::VEC_BOL{true}, 
      mt::VEC_STR{"Give a polite sentence"}),
    new cm::Toggles(),
    std::make_shared<cm::CALLBACK>(foo)
  );

  cm::Executor exe("TEST", {com});
  exe.iterate(argc, argv);

  return 0;
}

