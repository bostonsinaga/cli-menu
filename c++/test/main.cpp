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

CM_FUNCTION(sentenceFun) {
  std::cout << "\nSentence is called..\n";
}

CM_FUNCTION(fileInFun) {
  std::cout << "\nRead..\n";
}

CM_FUNCTION(fileOutFun) {
  std::cout << "\nWrite..\n";
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
    false
  );

  cm::Toggle *go = new cm::Toggle(
    "go",
    "go description",
    true,
    dialog
  );

  cm::Toggle *run = new cm::Toggle(
    "run",
    "run description",
    true,
    go
  );

  cm::Parameter *stop = new cm::Parameter(
    "stop",
    "stop description",
    false,
    go,
    cm::Parameter::NUMBER,
    false
  );

  cm::Parameter *sentence = new cm::Parameter(
    "sentence",
    "Start conversation",
    true,
    dialog,
    cm::Parameter::NUMBER,
    false,
    CM_CALLBACK(sentenceFun)
  );

  cm::Parameter *fileIn = new cm::Parameter(
    "file-in",
    "Filename input",
    true,
    sentence,
    cm::Parameter::TEXT,
    false,
    CM_CALLBACK(fileInFun)
  );

  cm::Parameter *fileOut = new cm::Parameter(
    "file-out",
    "Filename output",
    false,
    fileIn,
    cm::Parameter::TEXT,
    false,
    CM_CALLBACK(fileOutFun)
  );

  cm::Toggle *tidy = new cm::Toggle(
    "tidy",
    "Tidy up text",
    false,
    fileOut,
    CM_CALLBACK(tidyFun)
  );

  go->setAsUltimate();
  sentence->setAsUltimate();

  test->run(argc, argv, true);
  return 0;
}

