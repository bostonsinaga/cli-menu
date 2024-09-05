#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "base.h"

namespace cli_menu {

  class Command;
  typedef std::vector<Command*> VEC_COM;
  typedef const VEC_COM& CR_VEC_COM;

  class Command {
  private:
    std::string name, description;
    SP_CALLBACK callback = nullptr;
    SP_PLAIN_CALLBACK plainCallback = nullptr;
    VEC_COM items;

    Command *next = nullptr,
      *holder = nullptr,
      *ultimate = nullptr;

    mt::UI tier = 0;
    bool required = false;

    void cleanItems();
    void sewNext(CR_INT index);
    void setItems(CR_VEC_COM newItems);
    void spreadUltimateDown(Command *newUltimate);

    void setMetaData(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in
    );

    static void printHelp(bool isClosed);
    static mt::USI chooseQuestion(ParamData &paramData);
    mt::USI closedQuestion(ParamData &paramData);
    mt::USI openQuestion(ParamData &paramData);
    mt::USI select(ParamData &paramData);

  protected:
    virtual ~Command();

  public:
    enum DIALOG_FLAG {CANCELED, COMPLETE};

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

    VEC_STR getTreeNamesVector(
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

    mt::UI getTier() { return tier; }
    size_t getNumberOfItems() { return items.size(); }

    VEC_COM getItems() { return items; }
    Command *getNext() { return next; }
    Command *getHolder() { return holder; }
    Command *getUltimate() { return ultimate; }

    Command *hasItem(Command *command);
    Command *hasItem(mt::CR_STR name_in);
    Command *getItem(mt::CR_INT index);
    Command *getItem(mt::CR_STR name_in);
    Command *getRoot();

    VEC_COM setItemsRelease(CR_VEC_COM newItems);
    void setItemsReplace(CR_VEC_COM newItems);

    void addItem(Command *command);
    void removeItem(Command *command);
    void removeItem(int index);
    void releaseItem(Command *command);
    void releaseItem(int index);
    void setHolder(Command *newHolder, bool addBack = true);
    void remove(mt::CR_BOL firstOccurrence = true);

    mt::USI dialog(std::string &statusDescription);

    bool isRequired();
    bool isUltimate();
    bool isGroup();
    bool isSupporter();
    bool isOptional();
    bool run();
    bool run(ParamData &paramData);

    virtual bool setData(mt::CR_STR str) {}
    virtual bool setData(mt::CR_BOL cond) {}
    virtual bool match(mt::VEC_STR &inputs) { return false; }

    void deepPull(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );

    virtual void pullData(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );
  };
}

#endif // __CLI_MENU__COMMAND_H__