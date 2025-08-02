#ifndef __CLI_MENU__PROGRAM_HPP__
#define __CLI_MENU__PROGRAM_HPP__

#include "type.hpp"

namespace cli_menu {

  struct Version {
    int major = 0, minor = 0, patch = 0;

    std::string stringify() const {
      return std::to_string(major) + '.'
      + std::to_string(minor) + '.'
      + std::to_string(patch);
    }
  };

  template <typename T>
  concept CommandType =
    std::is_same_v<T, Word> ||
    std::is_same_v<T, Number> ||
    std::is_same_v<T, Toggle>;

  template <CommandType T>
  class Program : public T {
  private:
    Program(
      mt::CR_STR keyword,
      mt::CR_STR description,
      const Version &version,
      mt::CR_STR author,
      mt::CR_STR url,
      const COMMAND_CALLBACK &callback
    );

  public:
    Program() = delete;

    static Program *create(
      mt::CR_STR keyword,
      mt::CR_STR description,
      const Version &version,
      mt::CR_STR author,
      mt::CR_STR url,
      const COMMAND_CALLBACK &callback
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