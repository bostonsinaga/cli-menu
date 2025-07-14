#ifndef __CLI_MENU__COMMAND_HPP__
#define __CLI_MENU__COMMAND_HPP__

#include "clipboard.hpp"
#include "result.hpp"

namespace cli_menu {

  // final status codes
  enum COMMAND_CODE {
    COMMAND_FAILED, COMMAND_ONGOING,
    COMMAND_REQUIRED, COMMAND_SUCCEEDED, COMMAND_CANCELED
  };

  typedef const COMMAND_CODE& CR_COMMAND_CODE;

  class Command : public mt_ds::GeneralTree {
  private:
    std::string description;
    bool required, selecting = false;

    inline static bool
      dialogued = true,
      propagation = true;

    // the program will stop if the callback returns false
    typedef std::function<bool()> CALLBACK;
    CALLBACK callback = []()->bool { return true; };

    COMMAND_CODE match(mt::CR_VEC_STR raws);
    COMMAND_CODE dialog();
    COMMAND_CODE enter();
    COMMAND_CODE goToNeighbor(mt_ds::LinkedList* neighbor);
    COMMAND_CODE resultInput(mt::CR_STR input);

    void printHelp();
    void printList();

  protected:
    std::string hyphens, keyword;

    Command(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in = false
    );

    virtual void copyPaste() {}
    virtual void pushUnormap(mt::CR_STR raw) {}
    virtual void resetUnormap() {}

  public:
    Command() = delete;

    /** Forbidden Base Class Methods */

    LinkedList *slice() override = delete;
    void merge(LinkedList *outsider) override = delete;
    void detach() override = delete;
    void appoint(LinkedList *newStart) override = delete;
    void join(LinkedList *insider) override = delete;
    void accept(LinkedList *outsider) override = delete;
    void annihilate() override = delete;

    void setParent(GT *object) override = delete;
    void removeChild(GT *child) override = delete;
    void cleanChildren() override = delete;

  private:
    /** Restricted Base Class Methods */

    void addChild(GT *object) override {
      mt_ds::GeneralTree::addChild(object);
    }

    void destroy() override {
      mt_ds::GeneralTree::destroy();
    }
  };

  class Word;
  class Number;
  class Toggle;
}

#endif // __CLI_MENU__COMMAND_HPP__