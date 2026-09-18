#include "commands.hpp"

namespace loseva {

void cmdClear(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name) || !dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  dms[name].clear();
  os << "Дерево " << name << " очищено\n";
}

}
