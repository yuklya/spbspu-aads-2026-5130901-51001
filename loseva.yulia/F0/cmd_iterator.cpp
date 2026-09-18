#include "commands.hpp"

namespace loseva {

void cmdIterator(std::istream & is, std::ostream & os, IteratorsMap & ims)
{
  std::string iterName;
  if (!(is >> iterName) || !ims.count(iterName)) {
    throw CommandException("<INVALID COMMAND>");
  }
  auto it = ims[iterName];
  if (it.getNode() == nullptr) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << "Текущий элемент итератора " << iterName << ": " << *it << "\n";
}

}
