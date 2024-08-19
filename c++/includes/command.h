#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include <memory>
#include "mini-tools.h"

namespace cli_menu {

  // callback format
  typedef std::function<void(mt::CR_VEC_STR, mt::CR_VEC2_DBL, mt::CR_VEC_BOL)> CALLBACK;

  // inheritance flags
  enum {COMMAND, PARAMETER, TOGGLE};

  class DashTest {
  public:
    static bool isSingle(mt::CR_STR str);
    static bool isDouble(mt::CR_STR str);
    static bool cleanSingle(std::string &str);
    static bool cleanDouble(std::string &str);
  };

  class Command;
  typedef std::vector<Command*> VEC_COM;
  typedef const VEC_COM& CR_VEC_COM;

  class Command {
  private:
    std::string name, description;
    std::shared_ptr<CALLBACK> callback;
    Command *holder;
    VEC_COM items;

    void setItems(CR_VEC_COM newItems);

  protected:
    int tier = 0;
    static Command *ultimate;
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

    std::string getName() { return name; }
    std::string getDescription() { return description; }
    VEC_COM getItems() { return items; }
    Command *getHolder() { return holder; }

    Command *getItem(int index);
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
    virtual int getInheritanceFlag() { return COMMAND; }
    static void setUltimate(Command *newUltimate);
    static void execute();
  };

  class Parameter {
  private:
    std::string argument;
    bool type = false;

    ~Parameter() {
      argument = "";
      type = false;
    }

    void pullData(
      mt::CR_VEC_STR &TEXTS,
      mt::CR_VEC_DBL &NUMBERS,
      mt::CR_VEC_BOL &CONDITIONS
    );

  public:
    enum {TEXT, NUMBER};

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    bool getType();
    std::string getStringifiedType();
    std::string getArgument();
    bool match(mt::VEC_STR &inputs);
    int getInheritanceFlag() { return PARAMETER; }
  };

  class Toggle {
  private:
    bool condition;
    ~Toggle() { condition = false; }

    void pullData(
      mt::CR_VEC_STR &TEXTS,
      mt::CR_VEC_DBL &NUMBERS,
      mt::CR_VEC_BOL &CONDITIONS
    );

  public:
    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    bool getCondition();
    bool match(mt::VEC_STR &inputs);
    int getInheritanceFlag() { return TOGGLE; }
  };
}

#endif // __CLI_MENU__COMMAND_H__