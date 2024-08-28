#include <iostream>
#include "cli-menu.h"

CM_FUNCTION(programFun) {
  std::cout << "\nWill have default function from 'Program' which displaying error message";
}

CM_FUNCTION(dialogFun) {
  std::cout << "\nDialog is called..";
}

CM_FUNCTION(sentenceFun) {
  std::cout << "\nSentence is called..";
}

int main(int argc, char *argv[]) {
  cm::Program *testProgram = new cm::Program(
    "test",
    "Test 'cli-menu' library",
    "Boston Sinaga",
    cm::Version(1, 0, 0),
    CM_CALLBACK(programFun)
  );

  cm::Toggle *dialog = new cm::Toggle(
    "dialog",
    "Start conversation",
    CM_CALLBACK(dialogFun)
  );

  cm::Parameter *sentence = new cm::Parameter(
    "sentence",
    "Give words with double quotes if have spaces",
    cm::Parameter::TEXT,
    CM_CALLBACK(sentenceFun)
  );

  testProgram->addItem(dialog);
  dialog->addItem(sentence);
  sentence->setAsUltimate();

  cm::Executor::create(testProgram, argc, argv);
  cm::Executor::execute();
  cm::Executor::end();

  std::cout << std::endl;
  return 0;
}

