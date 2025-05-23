#include "cli-menu.hpp"

void programFun() {
  std::cout << "\nProgram is called..\n";
}

void dialogFun() {
  std::cout << "\nDialog is called..\n";
}

void fooFun() {
  std::cout << "\nFoo is called..\n";
}

void sentenceFun() {
  std::cout << "\nSentence is called..\n";
}

void wordFun() {
  std::cout << "\nWrite word..\n";
}

void tidyFun() {
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
    cm::Parameter::PARAM_NUMBER,
    false,
    dialogFun
  );

  cm::Parameter *foo = new cm::Parameter(
    "foo",
    "foo description",
    true,
    dialog,
    cm::Parameter::PARAM_TEXT,
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
    cm::Parameter::PARAM_NUMBER,
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
    cm::Parameter::PARAM_TEXT,
    false,
    sentenceFun
  );

  cm::Parameter *word = new cm::Parameter(
    "word",
    "word description",
    true,
    sentence,
    cm::Parameter::PARAM_TEXT,
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

