#include "commands.hpp"

namespace loseva {

void cmdInsert(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val) || !dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  if (!dms[name].insert(val)) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << "В дерево " << name << " добавлен элемент: " << val << "\n";
}

}
