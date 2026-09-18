#include "commands.hpp"

namespace loseva {

void cmdTree(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name) || !dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  dms[name].printTreeShape(os);
}

}
