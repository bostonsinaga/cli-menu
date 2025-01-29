#ifndef __CLI_MENU__PROGRAM_H__
#define __CLI_MENU__PROGRAM_H__

#include "toggle.h"

namespace cli_menu {

  class Version {
  public:
    mt::UI
      major = 0,
      minor = 0,
      patch = 0;

    Version() {}

    Version(
      mt::CR_UI major_in,
      mt::CR_UI minor_in,
      mt::CR_UI patch_in
    );

    std::string stringify();
  };

  typedef const Version& CR_VERSION;

  class Program : public Toggle {
  private:
    std::string author;
    Version version;

  public:
    Program();

    Program(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_STR author_in,
      CR_VERSION version_in,
      mt::CR_BOL accumulating_in,
      CM_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Program(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_STR author_in,
      CR_VERSION version_in,
      mt::CR_BOL accumulating_in
    );

    std::string getAuthor() { return author; }
    Version getVersion() { return version; }
    mt::USI getInheritanceFlag() override { return PROGRAM; }
    std::string getDashedName() override;

    std::string getFullName(
      mt::CR_BOL useDash = true,
      mt::CR_BOL useInputType = true,
      mt::CR_BOL useLevelName = true,
      CR_CLR nameColor = Color()
    ) override;

    static void useCaseSensitiveName(mt::CR_BOL isIt);
    static void useLowercaseName(mt::CR_BOL isIt);
    static void useUppercaseName(mt::CR_BOL isIt);
    static void useDashesBoundaryLine(mt::CR_BOL isIt);

    void printHelp() override;
    void printError() override;

    // will remove the root at the end
    void run(
      mt::CR_INT argc,
      char *argv[],
      mt::CR_BOL completingDialog
    );
  };
}

#endif // __CLI_MENU__PROGRAM_H__