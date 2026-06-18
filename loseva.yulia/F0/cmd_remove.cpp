#include "commands.hpp"

namespace loseva {

void cmdRemove(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val) || !dms.count(name)) {
    os << "<INVALID COMMAND>\n";
  } else if (dms[name].remove(val)) {
    os << "Из дерева " << name << " удалён элемент: " << val << "\n";
  } else {
    os << "<INVALID COMMAND>\n";
  }
}

}
