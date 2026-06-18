#include "commands.hpp"

namespace loseva {

void cmdContains(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val) || !dms.count(name)) {
    os << "<INVALID COMMAND>\n";
  } else {
    os << (dms[name].has(val) ? "true\n" : "false\n");
  }
}

}
