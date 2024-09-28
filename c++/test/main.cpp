#include <iostream>
#include "cli-menu.h"

void programFun() {
  std::cout << "\nWill have default function from 'Program' which displaying error message";
}

CM_FUNCTION(dialogFun) {
  std::cout << "\nDialog is called..";
}

CM_FUNCTION(sentenceFun) {
  std::cout << "\nSentence is called..";
}

CM_FUNCTION(fileInFun) {
  std::cout << "\nRead..";
}

CM_FUNCTION(fileOutFun) {
  std::cout << "\nWrite..";
}

int main(int argc, char *argv[]) {
  cm::Program *testProgram = new cm::Program(
    "test",
    "Test 'cli-menu' library",
    "Boston Sinaga",
    cm::Version(1, 0, 0),
    CM_PLAIN_CALLBACK(programFun)
  );

  cm::Toggle *dialog = new cm::Toggle(
    "dialog",
    "Start conversation",
    CM_CALLBACK(dialogFun),
    testProgram
  );

  cm::Toggle *sentence = new cm::Toggle(
    "sentence",
    "Give words with double quotes if have spaces",
    CM_CALLBACK(sentenceFun),
    dialog
  );

  cm::Program *lorem = new cm::Program(
    "lorem",
    "Lore ipsum dolor amet",
    "Boston Sianipar",
    cm::Version(1, 0, 1)
  );

  sentence->addItem(lorem);

  cm::Parameter *fileIn = new cm::Parameter(
    "file-in",
    "file name",
    cm::Parameter::TEXT,
    CM_CALLBACK(fileInFun),
    lorem
  );

  cm::Parameter *fileOut = new cm::Parameter(
    "file-out",
    "file name",
    cm::Parameter::TEXT,
    CM_CALLBACK(fileOutFun),
    fileIn,
    true
  );

  lorem->setAsUltimate();
  cm::Executor::run(testProgram, argc, argv, true);
  return 0;
}

