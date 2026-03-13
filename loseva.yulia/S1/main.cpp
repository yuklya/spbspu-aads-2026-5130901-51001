#include <iostream>
#include "sequence.hpp"

namespace loseva
{

void ReadSequences(Storage& data)
{
  std::string name;

  while (std::cin >> name)
  {
    Seq seq;

    int value;

    while (std::cin.peek() != '\n' && std::cin >> value)
    {
      seq.push_front(value);
    }

    data.push_front({name, seq});
  }
}

void PrintNames(const Storage& data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    std::cout << (*it).first << " ";
  }
  std::cout << "\n";
}

void Process(const Storage& data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    auto seq = (*it).second;

    for (auto sit = seq.begin(); sit != seq.end(); ++sit)
    {
      std::cout << *sit << " ";
    }

    std::cout << "\n";
  }
}

}
