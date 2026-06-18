#include "commands.hpp"

namespace loseva {

void cmdBegin(std::istream & is, std::ostream & os, DatasetsMap & dms, IteratorsMap & ims)
{
  std::string iterName, treeName;
  if (!(is >> iterName >> treeName) || !dms.count(treeName)) {
    os << "<INVALID COMMAND>\n";
  } else {
    ims[iterName] = dms[treeName].cbegin();
    os << "Итератор " << iterName << " привязан к дереву " << treeName
       << " и установлен на его начало\n";
  }
}

}
