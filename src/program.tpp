#ifndef __CLI_MENU__PROGRAM_TPP__
#define __CLI_MENU__PROGRAM_TPP__

#include <csignal>

namespace cli_menu {

  template <CommandType T>
  Program<T>::Program(
    mt::CR_STR keyword,
    mt::CR_STR description,
    const Version &version,
    mt::CR_STR author,
    mt::CR_STR url,
    COMMAND_CALLBACK callback
  ) : T(
    keyword,
    description + '\n' +
    Langu::ageProgram::getLabel(PROGRAM_LABEL_VERSION) + ": " + version.stringify() + '\n' +
    Langu::ageProgram::getLabel(PROGRAM_LABEL_AUTHOR) + ": " + author + '\n' +
    Langu::ageProgram::getLabel(PROGRAM_LABEL_LINK) + ": " + url,
    callback
  ) {
    Command::required = true;
  }

  template <CommandType T>
  Program<T> *Program<T>::create(
    mt::CR_STR keyword,
    mt::CR_STR description,
    const Version &version,
    mt::CR_STR author,
    mt::CR_STR url,
    COMMAND_CALLBACK callback
  ) {
    Program<T> *program = new Program<T>(
      keyword, description,
      version, author, url,
      callback
    );

    program->setPresetHelpList();
    return program;
  }

  template <CommandType T>
  void Program<T>::run(mt::CR_INT argc, char *argv[]) {

    // register signal handler for Ctrl+C (SIGINT)
    std::signal(SIGINT, Control::setInterruptedCtrlC);

    // in case this method is called more than once
    Command::raws.clear();

    // skip the first 'argv' which is unpredictable executable filename
    for (int i = argc - 1; i > 0; i--) {
      Command::raws.push_back(argv[i]);
    }

    // start recursion
    COMMAND_CODE commandCode = Program<T>::match();

    switch (commandCode) {
      case COMMAND_FAILED: {
        Langu::ageMessage::printResponse(SENTENCE_PROGRAM_FAILED);
      break;}
      case COMMAND_SUCCEEDED: {
        Langu::ageMessage::printResponse(SENTENCE_PROGRAM_SUCCEEDED);
      break;}
      case COMMAND_TERMINATED: {
        Langu::ageMessage::printResponse(SENTENCE_PROGRAM_TERMINATED);
      break;}
    }
  }
}

#endif // __CLI_MENU__PROGRAM_TPP__