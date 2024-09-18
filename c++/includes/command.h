#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "base.h"

namespace cli_menu {

  class Control {
  public:
    static const std::string CANCEL, ENTER, NEXT;
    static bool cancelTest(std::string &str);
    static bool enterTest(std::string &str);
    static bool nextTest(std::string &str);
  };

  class Command;
  typedef std::vector<Command*> VEC_COM;
  typedef const VEC_COM& CR_VEC_COM;

  class Command {
  private:
    std::string name, description;
    SP_CALLBACK callback = nullptr;
    SP_PLAIN_CALLBACK plainCallback = nullptr;

    VEC_COM items,
      requiredItems; // only exists in the 'ultimate' scope

    Command *next = nullptr,
      *holder = nullptr,
      *ultimate = nullptr;

    mt::UI tier = 0;
    bool required = false;
    static std::string boundaryLine;

    int getRequiredCount();
    void updateRequiredItems(Command *command, mt::CR_BOL adding);

    void cleanDuplicatesInItems();
    void cleanDuplicateToLastAdded(Command *command);

    void cleanItems();
    void sewNext(mt::CR_INT index);
    void setItems(CR_VEC_COM newItems);
    void remove(mt::CR_BOL firstOccurrence);

    Command* dismantle(mt::CR_INT index);
    void dismantleRemove(mt::CR_INT index);
    Command* dismantleRelease(mt::CR_INT index);

    void collapseUltimateItems(
      Command *newUltimate,
      VEC_COM &united
    );

    void setMetaData(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in
    );

    static void printHelp(mt::CR_BOL idx);
    static void printTryAgain(mt::CR_STR about);
    static void printError_enter(mt::CR_BOL selecting);
    static void printError_next(mt::CR_BOL selecting);
    static mt::USI chooseQuestion(Command *com);

    mt::USI closedQuestion();
    mt::USI openQuestion();
    mt::USI select();

  protected:
    virtual ~Command();

  public:
    static struct {
      enum {CANCELED, COMPLETE};
    } DIALOG_FLAG;

    Command() {}

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_CALLBACK callback_in,
      mt::CR_BOL required_in = false
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      mt::CR_BOL required_in = false
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in = false
    );

    virtual mt::USI getInheritanceFlag() { return COMMAND; }
    virtual std::string getDashedName() { return name; }
    virtual std::string getFullName() { return name; }
    std::string getName() { return name; }
    std::string getDescription() { return description; }

    mt::VEC_STR getTreeNamesVector(
      mt::CR_BOL fully = false,
      mt::CR_BOL withThis = false
    );

    std::string getTreeNames(
      mt::CR_STR separator = " ",
      mt::CR_BOL fully = false,
      mt::CR_BOL withThis = false
    );

    void setCallback(CR_SP_CALLBACK callback_in);
    void setCallback(CR_SP_PLAIN_CALLBACK callback_in);
    void setRequired(mt::CR_BOL condition) { required = condition; }
    void setAsUltimate();
    void resignFromUltimate();

    mt::UI getTier() { return tier; }
    size_t getNumberOfItems() { return items.size(); }

    VEC_COM getItems() { return items; }
    Command *getNext() { return next; }
    Command *getHolder() { return holder; }
    Command *getUltimate() { return ultimate; }

    bool hasItem(Command *command);
    bool hasItem(mt::CR_STR name_in);
    Command *getItem(mt::CR_INT index);
    Command *getItem(mt::CR_STR name_in);
    Command *getRoot();

    VEC_COM setItemsRelease(CR_VEC_COM newItems);
    void setItemsReplace(CR_VEC_COM newItems);

    /**
     * NOTE :
     * Cannot change 'items',
     * if this an 'ultimate' supporter.
     */
    void addItem(Command *command);
    void removeItem(Command *command);
    void removeItem(mt::CR_INT index);
    Command *releaseItem(Command *command);
    Command *releaseItem(mt::CR_INT index);
    void setHolder(Command *newHolder, bool addBack = true);
    void remove() { remove(true); }

    bool isUltimate();
    bool isGroup();
    bool isSupporter();
    bool isRequired();
    bool isOptional();
    bool run();
    bool run(ParamData &paramData);

    virtual void setData(mt::CR_STR str) {}
    virtual void setData(mt::CR_BOL cond) {}
    virtual bool match(mt::VEC_STR &inputs) { return false; }
    mt::USI dialog();

    void deepPull(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );

    virtual void pullData(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );

    // invoked in constructors with default size of 40
    static void setBoundaryLine(mt::CR_SI size);

    static void printBoundaryLine();

    void printAfterBoundaryLine(
      mt::CR_STR about,
      mt::CR_BOL idx
    );
  };
}

#endif // __CLI_MENU__COMMAND_H__