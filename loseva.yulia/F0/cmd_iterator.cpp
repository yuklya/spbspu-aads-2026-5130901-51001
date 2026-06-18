#include "commands.hpp"

namespace loseva {

void cmdIterator(std::istream & is, std::ostream & os, IteratorsMap & ims)
{
  std::string iterName;
  if (!(is >> iterName) || !ims.count(iterName)) {
    os << "<INVALID COMMAND>\n";
  } else {
    auto it = ims[iterName];
    if (it.getNode() == nullptr) {
      os << "<INVALID COMMAND>\n";
    } else {
      os << "Текущий элемент итератора " << iterName << ": " << *it << "\n";
    }
  }
}

}
