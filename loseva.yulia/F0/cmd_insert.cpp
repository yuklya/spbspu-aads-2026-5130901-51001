#include "commands.hpp"

namespace loseva {

void cmdInsert(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val) || !dms.count(name)) {
    os << "<INVALID COMMAND>\n";
  } else if (dms[name].insert(val)) {
    os << "В дерево " << name << " добавлен элемент: " << val << "\n";
  } else {
    os << "<INVALID COMMAND>\n";
  }
}

}
