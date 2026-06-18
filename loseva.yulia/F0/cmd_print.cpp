#include "commands.hpp"

namespace loseva {

void cmdPrint(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name) || !dms.count(name)) {
    os << "<INVALID COMMAND>\n";
  } else {
    const Dataset & ds = dms[name];
    if (ds.empty()) {
      os << "<EMPTY>\n";
    } else {
      bool first = true;
      for (auto it = ds.cbegin(); it != ds.cend(); ++it) {
        if (!first) os << " ";
        os << *it;
        first = false;
      }
      os << "\n";
    }
  }
}

}
