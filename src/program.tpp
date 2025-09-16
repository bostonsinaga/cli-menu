#ifndef __CLI_MENU__PROGRAM_TPP__
#define __CLI_MENU__PROGRAM_TPP__

#include <csignal>

namespace cli_menu {

  ProgramVersion::ProgramVersion(
    mt::CR_INT major_in,
    mt::CR_INT minor_in,
    mt::CR_INT patch_in
  ) {
    major = major_in;
    minor = minor_in;
    patch = patch_in;
  }

  std::string ProgramVersion::stringify() const {
    return std::to_string(major) + '.'
    + std::to_string(minor) + '.'
    + std::to_string(patch);
  }

  ProgramAbout::ProgramAbout(
    mt::CR_STR description_in,
    mt::CR_STR author_in,
    mt::CR_STR url_in,
    const ProgramVersion &version_in
  ) {
    description = description_in;
    author = author_in;
    url = url_in;
    version = version_in;
  }

  std::string ProgramAbout::stringify() const {
    return description + '\n'
    + Langu::ageProgram::getLabel(PROGRAM_LABEL_VERSION) + ": " + version.stringify() + '\n'
    + Langu::ageProgram::getLabel(PROGRAM_LABEL_AUTHOR) + ": " + author + '\n'
    + Langu::ageProgram::getLabel(PROGRAM_LABEL_LINK) + ": " + url;
  }

  template <CommandType T>
  Program<T>::Program(
    mt::CR_STR keyword_in,
    const ProgramAbout &about_in,
    COMMAND_CALLBACK callback_in
  ) : T(
    keyword_in,
    about_in.stringify(),
    callback_in
  ) { Command::makeRequired(); }

  template <CommandType T>
  Program<T>::Program(
    mt::CR_STR keyword_in,
    const ProgramAbout &about_in
  ) : T(
    keyword_in,
    about_in.stringify()
  ) { Command::makeRequired(); }

  template <CommandType T>
  Program<T> *Program<T>::create(
    mt::CR_STR keyword_in,
    const ProgramAbout &about_in,
    COMMAND_CALLBACK callback_in
  ) {
    Program<T> *program = new Program<T>(
      keyword_in, about_in, callback_in
    );

    program->setPresetHelpList();
    return program;
  }

  template <CommandType T>
  Program<T> *Program<T>::create(
    mt::CR_STR keyword_in,
    const ProgramAbout &about_in
  ) {
    Program<T> *program = new Program<T>(keyword_in, about_in);
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