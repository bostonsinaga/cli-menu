#include <iostream>
#include "cli-menu.h"

void programFun() {
  std::cout << "\nProgram is called..\n";
}

CM_FUNCTION(dialogFun) {
  std::cout << "\nDialog is called..\n";

  std::cout << "\nTEXTS: ";
  for (mt::CR_STR v : TEXTS) {
    std::cout << v << ',';
  }

  std::cout << "\nNUMBERS: ";
  for (mt::CR_VEC_DBL v1 : NUMBERS) {
    std::cout << "[";
    for (mt::CR_DBL v2 : v1) {
      std::cout << v2 << ',';
    }
    std::cout << "]";
  }

  std::cout << "\nCONDITIONS: ";
  for (mt::CR_BOL v : CONDITIONS) {
    std::cout << v << ',';
  }
}

CM_FUNCTION(fooFun) {
  std::cout << "\nFoo is called..\n";
}

CM_FUNCTION(sentenceFun) {
  std::cout << "\nSentence is called..\n";
}

CM_FUNCTION(wordFun) {
  std::cout << "\nWrite word..\n";
}

CM_FUNCTION(tidyFun) {
  std::cout << "\nTidy up..\n";
}

int main(int argc, char *argv[]) {

  cm::Program *test = new cm::Program(
    "test",
    "Test 'cli-menu' library",
    "Boston Sinaga",
    cm::Version(1, 0, 0),
    CM_PLAIN_CALLBACK(programFun)
  );

  cm::Parameter *dialog = new cm::Parameter(
    "dialog",
    "Start conversation",
    true,
    test,
    cm::Parameter::NUMBER,
    false,
    CM_CALLBACK(dialogFun)
  );

  cm::Parameter *foo = new cm::Parameter(
    "foo",
    "foo description",
    true,
    dialog,
    cm::Parameter::TEXT,
    false,
    CM_CALLBACK(fooFun)
  );

  cm::Toggle *go = new cm::Toggle(
    "go",
    "go description",
    true,
    dialog
  );

  cm::Parameter *run = new cm::Parameter(
    "run",
    "run description",
    false,
    go,
    cm::Parameter::NUMBER,
    false
  );

  cm::Toggle *stop = new cm::Toggle(
    "stop",
    "stop description",
    true,
    go
  );

  cm::Parameter *sentence = new cm::Parameter(
    "sentence",
    "sentence description",
    true,
    dialog,
    cm::Parameter::TEXT,
    false,
    CM_CALLBACK(sentenceFun)
  );

  cm::Parameter *word = new cm::Parameter(
    "word",
    "word description",
    true,
    sentence,
    cm::Parameter::TEXT,
    false,
    CM_CALLBACK(wordFun)
  );

  cm::Toggle *tidy = new cm::Toggle(
    "tidy",
    "tidy description",
    false,
    sentence,
    CM_CALLBACK(tidyFun)
  );

  cm::Toggle *clean = new cm::Toggle(
    "clean",
    "clean description",
    true,
    sentence,
    CM_CALLBACK(tidyFun)
  );

  sentence->setAsUltimate();
  test->run(argc, argv, true);
  return 0;
}

