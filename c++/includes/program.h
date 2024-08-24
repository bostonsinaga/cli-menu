#ifndef __CLI_MENU__PROGRAM_H__
#define __CLI_MENU__PROGRAM_H__

#include "command.h"

namespace cli_menu {

  class Version {
  private:
    mt::UI major = 0, minor = 0, patch = 0;
    ~Version();

  public:
    Version(
      mt::CR_UI major_in,
      mt::CR_UI minor_in,
      mt::CR_UI patch_in
    );

    std::string stringify();
    void clean() { delete this; }
  };

  class Program : public Command {
  private:
    std::string author;
    Version *version;
    ~Program();

  public:
    Program(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_STR author_in,
      Version *version_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    std::string getAuthor() { return author; }
    Version *getVersion() { return version; }
    mt::USI getInheritanceFlag() { return PROGRAM; }
  };
}

#endif // __CLI_MENU__PROGRAM_H__