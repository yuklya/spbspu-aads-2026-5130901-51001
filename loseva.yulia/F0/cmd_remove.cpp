#include "commands.hpp"

namespace loseva {

void cmdRemove(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val) || !dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  if (!dms[name].remove(val)) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << "Из дерева " << name << " удалён элемент: " << val << "\n";
}

}
