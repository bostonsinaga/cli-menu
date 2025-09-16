#ifndef __CLI_MENU__PROGRAM_HPP__
#define __CLI_MENU__PROGRAM_HPP__

#include "type.hpp"

namespace cli_menu {

  template <typename T>
  concept CommandType =
    std::is_same_v<T, Word> ||
    std::is_same_v<T, Number> ||
    std::is_same_v<T, Boolean>;

  class ProgramVersion final {
  private:
    int major = 0, minor = 0, patch = 0;

  public:
    ProgramVersion() {}

    ProgramVersion(
      mt::CR_INT major_in,
      mt::CR_INT minor_in,
      mt::CR_INT patch_in
    );

    std::string stringify() const;
  };

  class ProgramAbout final {
  private:
    std::string description, author, url;
    ProgramVersion version;

  public:
    ProgramAbout() {}

    ProgramAbout(
      mt::CR_STR description_in,
      mt::CR_STR author_in,
      mt::CR_STR url_in,
      const ProgramVersion &version_in
    );

    std::string stringify() const;
  };

  template <CommandType T>
  class Program : public T {
  private:
    Program(
      mt::CR_STR keyword_in,
      const ProgramAbout &about_in,
      COMMAND_CALLBACK callback_in
    );

    Program(
      mt::CR_STR keyword_in,
      const ProgramAbout &about_in
    );

  public:
    Program() = delete;

    static Program *create(
      mt::CR_STR keyword_in,
      const ProgramAbout &about_in,
      COMMAND_CALLBACK callback_in
    );

    static Program *create(
      mt::CR_STR keyword_in,
      const ProgramAbout &about_in
    );

    /**
     * Start match and dialog.
     * This will not be deleted automatically.
     * Delete it manually by calling 'destroy'.
     */
    void run(mt::CR_INT argc, char *argv[]);
  };
}

#include "program.tpp"
#endif // __CLI_MENU__PROGRAM_HPP__