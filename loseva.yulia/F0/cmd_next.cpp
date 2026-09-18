#include "commands.hpp"

namespace loseva {

void cmdNext(std::istream & is, std::ostream & os, IteratorsMap & ims)
{
  std::string iterName;
  if (!(is >> iterName) || !ims.count(iterName)) {
    throw CommandException("<INVALID COMMAND>");
  }
  auto & it = ims[iterName];
  if (it.getNode() == nullptr) {
    throw CommandException("<INVALID COMMAND>");
  }
  ++it;
  os << "Итератор " << iterName << " перешел к следующему элементу\n";
}

}
