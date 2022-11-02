#include <iostream>
#include <deque>
#include <ctime>
#include <iomanip>
#include "SortCollections.hpp"
#include "StreamGuard.hpp"

int main(int argv, char** argc)
{
  std::srand(std::time(nullptr));

  if (argv != 4) {
    std::cerr << "Invalid amount of arguments arguments!\n";
    return 1;
  }

  try {
    size_t size = std::stoull(argc[3]);
    std::string direction = argc[1];
    std::string type = argc[2];
    if (type == "ints") {
      gorbunova::SortCollections< int > collections(size, direction);
      collections.printCollections(std::cout) << "\n";
    } else if (type == "floats") {
      gorbunova::SortCollections< float > collections(size, direction);
      gorbunova::StreamGuard guard(std::cout);
      std::cout << std::fixed << std::setprecision(1);
      collections.printCollections(std::cout) << "\n";
    } else {
      throw std::logic_error("Invalid type!");
    }
  } catch (...) {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

}
