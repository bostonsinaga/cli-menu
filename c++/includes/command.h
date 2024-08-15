#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include <memory>
#include "mini-tools.h"

namespace cli_menu {

  // callback format
  typedef std::function<void(mt::CR_VEC_STR, mt::CR_VEC_DBL, mt::CR_VEC_BOL)> CALLBACK;

  class Command;
  typedef std::vector<Command*> VEC_COM;
  typedef CR_VEC_COM const VEC_COM &;

  class Command {
  private:
    std::string name, description;
    int tier = 0;

    std::shared_ptr<CALLBACK> callback;
    Command *holder;
    VEC_COM items;
    static Command *ultimate;

    void setItems(CR_VEC_COM newItems);

    void pullData(
      mt::CR_VEC_STR &TEXTS,
      mt::CR_VEC_DBL &NUMBERS,
      mt::CR_VEC_BOL &CONDITIONS
    );

  protected:
    virtual ~Command();
    bool matchItems(mt::CR_STR test);

  public:
    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    std::string getName();
    std::string getDescription();
    Command *getHolder();
    VEC_COM getItems();
    Command *getItem(int index);

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
    static void setUltimate(Command *newUltimate);
    static void execute();
  };

  class Parameter {
  private:
    std::string argument;
    bool type = false;
    ~Parameters();

  public:
    enum {TEXT, NUMBER};

    Parameters(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    bool getType();
    std::string getStringifiedType();
    std::string getArgument();
    void match(mt::VEC_STR &tests);
  };

  class Toggles {
  private:
    bool condition;
    ~Toggles() { condition = false; }

  public:
    Toggles(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    bool getCondition();
    void match(mt::VEC_STR &tests);
  };
}

#endif // __CLI_MENU__COMMAND_H__