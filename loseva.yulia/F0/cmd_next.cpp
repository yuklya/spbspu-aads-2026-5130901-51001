#include "commands.hpp"

namespace loseva {

void cmdNext(std::istream & is, std::ostream & os, IteratorsMap & ims)
{
  std::string iterName;
  if (!(is >> iterName) || !ims.count(iterName)) {
    os << "<INVALID COMMAND>\n";
  } else {
    auto & it = ims[iterName];
    if (it.getNode() == nullptr) {
      os << "<INVALID COMMAND>\n";
    } else {
      ++it;
      os << "Итератор " << iterName << " перешел к следующему элементу\n";
    }
  }
}

}
