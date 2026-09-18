#include "commands.hpp"

namespace loseva {

void cmdSearch(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val) || !dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << (dms[name].has(val) ? "Элемент найден\n" : "Элемент не найден\n");
}

}
