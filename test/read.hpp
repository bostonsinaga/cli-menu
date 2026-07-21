void initRead(cm::Parameter *cli_log) {

  cm::Boolean *cli_read = cli_log->addBoolean(
    "read",
    "Read TXT file and display it to terminal",
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {
      std::cout << "__cli_read_called\n";
      return cm::COMMAND_CALLBACK_DONE;
    }
  );

  if (cli_read) {
    cli_read->makeStrict();
    cm::Preset::File::applyTextIn(cli_read, true);
  }
}
