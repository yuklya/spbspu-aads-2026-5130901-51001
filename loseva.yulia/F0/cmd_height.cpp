#include "commands.hpp"

namespace loseva {

void cmdHeight(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name) || !dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << "Высота дерева " << name << ": " << dms[name].height() << "\n";
}

}
