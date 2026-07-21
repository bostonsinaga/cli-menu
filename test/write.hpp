void initWrite(cm::Parameter *cli_log) {

  cm::Boolean *cli_write = cli_log->addBoolean(
    "write",
    "Write input to a TXT file in 'logs' folder",
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {
      std::cout << "__cli_write_called\n";
      return cm::COMMAND_CALLBACK_DONE;
    }
  );

  if (cli_write) {
    cli_write->makeStrict();
    cm::Preset::File::applyTextOutFallback(cli_write, true);
  }
}
