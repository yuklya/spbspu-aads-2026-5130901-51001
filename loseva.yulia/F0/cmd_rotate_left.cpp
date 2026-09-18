#include "commands.hpp"

namespace loseva {

void cmdRotateLeft(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val) || !dms.count(name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  try {
    dms[name].rotateLeftManual(val);
    os << "Выполнен левый поворот для узла " << val << " в дереве " << name << "\n";
  } catch (...) {
    throw CommandException("<INVALID COMMAND>");
  }
}

}
