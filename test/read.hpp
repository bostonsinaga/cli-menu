void initRead(cm::Parameter *cli_log) {

  cm::Word *cli_read = cli_log->addWord(
    "read", "Read TXT file and display it to terminal"
  );

  if (cli_read) {
    cli_read->makeStrict();
    cm::Preset::File::applyTextIn(cli_read, true);
  }
}
