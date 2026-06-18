#include "commands.hpp"

namespace loseva {

void cmdClear(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name) || !dms.count(name)) {
    os << "<INVALID COMMAND>\n";
  } else {
    dms[name].clear();
    os << "Дерево " << name << " очищено\n";
  }
}

}
