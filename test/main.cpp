#include "cli-menu.hpp"

int main(int argc, char *argv[]) {

  cm::Toggle *organism = new cm::Toggle(
    "organism",
    "Describe your lovely animals or plants",
    true,
    []()->bool {
      std::cout << "Hello Organism";
      return true;
    }
  );

  cm::Toggle *animals = new cm::Toggle(
    "animals",
    "Oxygen consumers",
    true,
    []()->bool {
      std::cout << "Hello Animals";
      return true;
    }
  );

  cm::Toggle *plants = new cm::Toggle(
    "plants",
    "Carbon dioxide consumers",
    true,
    []()->bool {
      std::cout << "Hello Plants";
      return true;
    }
  );

  organism->addChild(animals);
  organism->addChild(plants);
  organism->run(argc, argv);

  return 0;
}

