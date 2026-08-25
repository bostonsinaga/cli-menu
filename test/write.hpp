void initWrite(cm::Parameter *cli_log) {

  cm::Word *cli_write = cli_log->addWord(
    "write",
    "Write input to a TXT file in 'logs' folder",
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {
      cm::Data::copyInputToOutputWords(current);
      return cm::COMMAND_CALLBACK_DONE;
    }
  );

  if (cli_write) {
    cli_write->makeStrict();
    cm::Preset::File::applyTextOutFallback(cli_write, true);
  }
}
