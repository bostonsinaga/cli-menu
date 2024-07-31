#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

void printSucceed() {
  std::cout << "\nSeeds generated..\n";
}

void printError() {
  std::cerr
    << "\nExpected inputs:\n"
    << "   type<string>[number, text]\n"
    << "   count<int>\n"
    << "   maxNumber<int>\n"
    << "   maxLetterCount<int>\n"
    << "   outputFilename<string\n>";
}

void generateNumbers(std::ofstream &writer, int count, int maxNumber) {
  for (int i = 0; i < count; i++) {
    writer << std::rand() % maxNumber + 1 << ' ';
  }
  printSucceed();
}

void generateStrings(std::ofstream &writer, int count, int maxLetterCount) {

  for (int i = 0; i < count; i++) {
    for (int j = 0; j < std::rand() % maxLetterCount + 1; j++) {
      bool isCapital = std::rand() % 2;
      if (isCapital) writer << char(std::rand() % 26 + 65); // big
      else writer << char(std::rand() % 26 + 97); // small
    }
    writer << ' ';
  }

  printSucceed();
}

// filename example: "resources/sample.txt"
int main(int argc, char* argv[]) {

  if (argc == 6) {
    std::string type, outputFilename;
    int count, maxNumber, maxLetterCount;

    try {
      type = argv[1];
      count = std::stoi(argv[2]);
      maxNumber = std::stoi(argv[3]);
      maxLetterCount = std::stoi(argv[4]);
      outputFilename = argv[5];
    }
    catch(...) {
      printError();
      return 0;
    }

    std::ofstream writer(outputFilename);
    std::srand(std::time(0));

    if (type == "number") {
      generateNumbers(writer, count, maxNumber);
    }
    else if (type == "text") {
      generateStrings(writer, count, maxLetterCount);
    }
    else printError();

    writer.close();
  }
  else printError();

  return 0;
}

