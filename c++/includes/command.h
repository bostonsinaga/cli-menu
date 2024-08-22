#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include <memory>
#include "mini-tools.h"

namespace cli_menu {

  class Command {
  private:
    std::string name, description;
    std::shared_ptr<CALLBACK> callback;
    Command *holder;
    VEC_COM items;

    void setItems(CR_VEC_COM newItems);

  protected:
    int tier = 0;
    Command *ultimate;
    static Command *program;
    virtual ~Command();

    static bool cleanCapturedPositionalInputs(
      mt::VEC_STR &inputs,
      mt::CR_SZ startIdx,
      mt::CR_SZ endIdx
    );

    virtual void pullData(
      mt::CR_VEC_STR &TEXTS,
      mt::CR_VEC_DBL &NUMBERS,
      mt::CR_VEC_BOL &CONDITIONS
    ) {}

    void deepPull(
      mt::CR_VEC_STR &TEXTS,
      mt::CR_VEC_DBL &NUMBERS,
      mt::CR_VEC_BOL &CONDITIONS
    );

  public:
    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    virtual std::string getName(CR_INT fully = false) {
      return name;
    }

    std::string getDescription() { return description; }
    VEC_COM getItems() { return items; }
    Command *getHolder() { return holder; }

    Command *getItem(CR_INT index);
    Command *getProgram();
    Command *getRoot();

    VEC_COM setItemsRelease(CR_VEC_COM newItems);
    void setItemsReplace(CR_VEC_COM newItems);

    void addItem(Command *com);
    void cleanItems();
    void removeItem(Command *com);
    void removeItem(int index);
    void releaseItem(Command *com);
    void releaseItem(int index);
    void setHolder(Command *newHolder, bool addBack = true);

    void remove();
    bool isRequired();
    virtual int getInheritanceFlag() { return COMMAND; }
    void setUltimate(Command *newUltimate);
    static void setProgram(Command *newProgram);
    static void execute();
  };
}

#endif // __CLI_MENU__COMMAND_H__