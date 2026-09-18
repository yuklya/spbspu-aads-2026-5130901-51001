#include "commands.hpp"

namespace loseva {

void cmdCreate(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name) || dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  dms[name] = Dataset();
  os << "Создано дерево: " << name << "\n";
}

}
