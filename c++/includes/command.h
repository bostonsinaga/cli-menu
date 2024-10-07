#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "control.h"

namespace cli_menu {

  class Command;
  typedef std::vector<Command*> VEC_COM;
  typedef const VEC_COM& CR_VEC_COM;

  class Command {
  private:
    std::string description;
    SP_CALLBACK callback = nullptr;
    SP_PLAIN_CALLBACK plainCallback = nullptr;

    // only exists in the 'ultimate' scope
    VEC_COM requiredItems;

    Command *holder = nullptr,
      *next = nullptr;

    bool accumulating = false,
      required = false;

    static const std::string
      cannotProcessErrorString,
      cannotSkipErrorString;

    void cleanDuplicatesInItems();
    void cleanDuplicateToLastAdded(Command *command);
    void cleanItems();
    void sewNext(mt::CR_INT index);
    void remove(mt::CR_BOL firstOccurrence);

    static void connectNext(
      int &index,
      VEC_COM &vecCom
    );

    void setItems(
      CR_VEC_COM newItems,
      mt::CR_BOL needEmpty,
      mt::CR_BOL validating
    );

    Command* dismantle(mt::CR_INT index);
    void dismantleRemove(mt::CR_INT index);
    Command* dismantleRelease(mt::CR_INT index);
    std::string getFullNameWithUltimate();

    mt::UI getRequiredCount() {
      return ultimate->requiredItems.size();
    }

    void updateRequiredItems(
      Command *command,
      mt::CR_BOL adding
    );

    void collapseUltimateItems(
      Command *newUltimate,
      VEC_COM &united
    );

    void setMetaData(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      Command *holder_in,
      mt::CR_BOL required_in,
      mt::CR_BOL accumulating_in
    );

    static void printDialogError(
      mt::CR_STR reason,
      mt::CR_STR suggestion = "Try Again"
    );

    void printDialogStatus();
    static void getDialogInput(std::string &buffer);
    static mt::USI chooseQuestion(Command *command);
    mt::USI closedQuestion();
    mt::USI openQuestion();

    /**
     * Prints '-' signs horizontally before names list or
     * names list with prominent background color.
     */
    void printAfterBoundaryLine(std::string comName);

    /**
     * A state setter for 'disguised' where the derived class
     * will behave as its base class (hiding its own abilities).
     * But still need to set 'disguised' on/off conditions.
     */
    void disguise(mt::CR_BOL isIt) { disguised = isIt; }

  protected:
    std::string name;
    VEC_COM items;
    Command *ultimate = nullptr;

    // can only be set through the 'Program'
    static bool usingCaseSensitiveName,
      usingLowercaseName,
      usingUppercaseName,
      usingDashesBoundaryLine;

    // useful in overriding 'match' method
    mt::UI level = 0;

    bool disguised = false,
      used = false;

    std::string getBranchLeafString(
      mt::CR_INT spacesCount,
      mt::CR_INT columnIndex,
      mt::CR_BOL withDescription
    );

    std::string getMainLabel();
    void updateRequiredSelf(mt::CR_BOL adding);

    static bool isTemporaryLetterCaseChange();
    static void onFreeChangeInputLetterCase(std::string &strIn);

    void changeTreeNamesToLowercase();
    void changeTreeNamesToUppercase();

    virtual void setData(mt::CR_STR str) {}
    virtual void setData(mt::CR_BOL cond) {}

    // will invoked from 'pullData'
    void deepPull(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );

    // destructor only invoked from 'remove' method
    virtual ~Command();

    // accessing letter case conditions and setters
    friend class Executor;

  public:
    enum DIALOG { CANCELED, COMPLETE };

    Command() {}

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_CALLBACK callback_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    virtual mt::USI getInheritanceFlag() { return COMMAND; }
    virtual std::string getDashedName() { return name; }
    virtual std::string getFullName() { return name; }
    std::string getName() { return name; }
    std::string getDescription() { return description; }

    void setCallback(CR_SP_CALLBACK callback_in);
    void setCallback(CR_SP_PLAIN_CALLBACK callback_in);
    void setAsUltimate();
    void resignFromUltimate();

    void setAccumulating(mt::CR_BOL isIt) { accumulating = isIt; }
    void setRequired(mt::CR_BOL isIt) { required = isIt; }

    mt::UI getLevel() { return level; }
    size_t getNumberOfItems() { return items.size(); }

    VEC_COM getItems() { return items; }
    Command *getHolder() { return holder; }
    Command *getNext() { return next; }
    Command *getUltimate() { return ultimate; }

    bool hasItem(Command *command);
    bool hasItem(mt::CR_STR name_in);
    Command *getItem(mt::CR_INT index);
    Command *getItem(mt::CR_STR name_in);
    Command *getRoot();

    VEC_COM setItemsRelease(
      CR_VEC_COM newItems,
      mt::CR_BOL validating = true
    );

    void setItemsReplace(
      CR_VEC_COM newItems,
      mt::CR_BOL validating = true
    );

    void addItems(
      CR_VEC_COM newItems,
      mt::CR_BOL validating = true
    );

    /**
     * NOTE :
     * Cannot change 'items',
     * if this an 'ultimate' supporter.
     */
    void addItem(Command *command, mt::CR_BOL reconnected = true);
    void setHolder(Command *newHolder, mt::CR_BOL reconnected = true);

    void removeItem(Command *command);
    void removeItem(mt::CR_INT index);
    Command *releaseItem(Command *command);
    Command *releaseItem(mt::CR_INT index);
    VEC_COM releaseItems();
    void remove() { remove(true); }

    bool isUltimate() { return this == ultimate; }
    bool isGroup() { return !ultimate || isUltimate(); }
    bool isSupporter() { return holder && holder == ultimate; }
    bool isRequired() { return isGroup() || required; }
    bool isOptional() { return !isRequired(); }
    bool isAccumulating() { return accumulating; }
    bool isUsed() { return used; }

    bool run();
    bool run(ParamData &paramData);

    virtual bool match(mt::VEC_STR &inputs) { return false; }
    mt::USI dialog();

    virtual void pullData(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );

    std::string getInlineRootNames(
      mt::CR_STR separator,
      mt::CR_BOL fully
    );

    std::string getBranchLeafString(
      int spacesCount,
      mt::CR_BOL withDescription
    );

    virtual void printHelp();
    virtual void printError();
  };
}

#endif // __CLI_MENU__COMMAND_H__