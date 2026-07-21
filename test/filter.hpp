void initFilter (cm::Parameter *cli_log) {

  cm::Boolean *cli_filter = cli_log->addBoolean(
    "filter",
    "Filter specific IDs, names, and values based on given string",
    [](cm::Command *current)->cm::COMMAND_CALLBACK_CODE {
      std::cout << "__cli_filter_called\n";
      return cm::COMMAND_CALLBACK_DONE;
    }
  );

  if (cli_filter) cli_filter->makeStrict();
}
