#ifndef __CLI_MENU__PROGRAM_TPP__
#define __CLI_MENU__PROGRAM_TPP__

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

  template <ParameterType T>
  Program<T> *Program<T>::create(
    mt::CR_STR keyword_in,
    const ProgramAbout &about_in,
    mt::CR<CODE_CALLBACK> callback_in
  ) {
    Program<T> *program = new Program<T>(
      keyword_in, about_in, callback_in
    );

    program->setPresetHelpList();
    return program;
  }

  template <ParameterType T>
  void Program<T>::run(mt::CR_INT argc, char *argv[]) {

    // register signal handler for Ctrl+C (SIGINT)
    Control::registerInterruptedCtrlC();

    // in case this method is called more than once
    Command::raws.clear();

    // skip the first 'argv' which is unpredictable executable filename
    for (int i = argc - 1; i > 0; i--) {
      Command::raws.push_back(argv[i]);
    }

    /** Start Recursion */

    Command *lastNode = Program<T>::match();

    while (true) {
      if (lastNode->getStatusCode() == COMMAND_PSEUDO) {
        break;
      }
      else if (lastNode->getStatusCode() == COMMAND_TERMINATED) {
        Langu::ageMessage::printResponse(SENTENCE_PROGRAM_TERMINATED);
        break;
      }
      else if (lastNode->getStatusCode() == COMMAND_DONE) {
        printWelcome(lastNode);
        Langu::ageMessage::printResponse(SENTENCE_PROGRAM_DONE);
      }
      else if (lastNode->getStatusCode() == COMMAND_CANCELED) {
        Langu::ageMessage::printResponse(SENTENCE_PROGRAM_CANCELED);
      }
      else if (lastNode->getStatusCode() == COMMAND_ERROR) {
        printWelcome(lastNode);
        Langu::ageMessage::printResponse(SENTENCE_PROGRAM_ERROR);
      }

      // dialog continued to the last node
      if (lastNode->isDialogued()) lastNode = lastNode->dialog();
      // match only (no dialog)
      else break;
    }

    // delete this and its descendants
    this->destroy();
  }

  template <ParameterType T>
  void Program<T>::printWelcome(Command *lastNode) {
    static bool displayed = false;

    if (!displayed && lastNode->isDialogued()) {
      displayed = true;
      Command *root = static_cast<Command*>(this->getRoot());

      // keyword
      Console::logString(
        Langu::ageMessage::getWelcomeToString() + ' ' +
        mt_uti::StrTool::copyStringToUppercase(root->getKeyword()) + '\n',
        Console::messageColors[CONLOR_TITLE]
      );

      // description
      Console::logItalicString(
        root->getDescription() + '\n',
        Console::messageColors[CONLOR_DESCRIPTION]
      );
    }
  }
}

#endif // __CLI_MENU__PROGRAM_TPP__