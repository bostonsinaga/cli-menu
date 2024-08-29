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

    Command *holder = nullptr,
      *ultimate = nullptr;

    mt::UI tier = 0;
    bool required = false;

    void cleanItems();
    void setItems(CR_VEC_COM newItems);
    void spreadUltimateDown(Command *newUltimate);

    void setData(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in
    );

  protected:
    virtual ~Command();

  public:
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
    virtual std::string getFullName() { return name; }
    std::string getName() { return name; }
    std::string getDescription() { return description; }

    void setCallback(CR_SP_CALLBACK callback_in);
    void setCallback(CR_SP_PLAIN_CALLBACK callback_in);
    void setRequired(mt::CR_BOL condition) { required = condition; }
    void setAsUltimate();

    mt::UI getTier() { return tier; }
    size_t getNumberOfItems() { return items.size(); }

    VEC_COM getItems() { return items; }
    Command *getHolder() { return holder; }
    Command *getUltimate() { return ultimate; }

    Command *getItem(mt::CR_INT index);
    Command *getRoot();

    VEC_COM setItemsRelease(CR_VEC_COM newItems);
    void setItemsReplace(CR_VEC_COM newItems);

    void addItem(Command *command);
    void removeItem(Command *command);
    void removeItem(int index);
    void releaseItem(Command *command);
    void releaseItem(int index);
    void setHolder(Command *newHolder, bool addBack = true);
    void remove();

    bool isRequired();
    bool isUltimate();
    bool isClassifier();
    bool run();
    bool run(ParamData &paramData);
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