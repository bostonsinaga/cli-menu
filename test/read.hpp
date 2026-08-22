void initRead(cm::Parameter *cli_log) {

  cm::Word *cli_read = cli_log->addWord(
    "read",
    "Read TXT file and display it to terminal",
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {
      std::cout << cm::Data::getWord(current);
      return cm::COMMAND_CALLBACK_DONE;
    }
  );

  if (cli_read) {
    cli_read->makeStrict();
    cm::Preset::File::applyTextIn(cli_read, true);
  }
}
