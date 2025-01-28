#include <iostream>
#include "cli-menu.h"

void programFun(cm::ResultInputs &resultInputs) {
  std::cout << "\nProgram is called..\n";
}

void dialogFun(cm::ResultInputs &resultInputs) {
  std::cout << "\nDialog is called..\n";
}

void fooFun(cm::ResultInputs &resultInputs) {
  std::cout << "\nFoo is called..\n";
}

void sentenceFun(cm::ResultInputs &resultInputs) {
  std::cout << "\nSentence is called..\n";
}

void wordFun(cm::ResultInputs &resultInputs) {
  std::cout << "\nWrite word..\n";
}

void tidyFun(cm::ResultInputs &resultInputs) {
  std::cout << "\nTidy up..\n";
}

int main(int argc, char *argv[]) {

  cm::Program *test = new cm::Program(
    "test",
    "Test 'cli-menu' library",
    "Boston Sinaga",
    cm::Version(1, 0, 0),
    false,
    programFun
  );

  cm::Parameter *dialog = new cm::Parameter(
    "dialog",
    "Start conversation",
    true,
    test,
    cm::Parameter::NUMBER,
    false,
    dialogFun
  );

  cm::Parameter *foo = new cm::Parameter(
    "foo",
    "foo description",
    true,
    dialog,
    cm::Parameter::TEXT,
    false,
    fooFun
  );

  cm::Toggle *go = new cm::Toggle(
    "go",
    "go description",
    true,
    dialog,
    false
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
    go,
    false
  );

  cm::Parameter *sentence = new cm::Parameter(
    "sentence",
    "sentence description",
    true,
    dialog,
    cm::Parameter::TEXT,
    false,
    sentenceFun
  );

  cm::Parameter *word = new cm::Parameter(
    "word",
    "word description",
    true,
    sentence,
    cm::Parameter::TEXT,
    false,
    wordFun
  );

  cm::Toggle *tidy = new cm::Toggle(
    "tidy",
    "tidy description",
    false,
    sentence,
    false,
    tidyFun
  );

  cm::Toggle *clean = new cm::Toggle(
    "clean",
    "clean description",
    true,
    sentence,
    false,
    tidyFun
  );

  sentence->setAsUltimate();
  test->run(argc, argv, true);
  return 0;
}

