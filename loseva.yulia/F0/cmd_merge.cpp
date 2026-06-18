#include "commands.hpp"

namespace loseva {

void cmdMerge(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string target, src1, src2;
  if (!(is >> target >> src1 >> src2) || !dms.count(src1) || !dms.count(src2)) {
    os << "<INVALID COMMAND>\n";
  } else if (dms.count(target)) {
    os << "<INVALID COMMAND>\n";
  } else {
    Dataset merged;
    for (auto it = dms[src1].cbegin(); it != dms[src1].cend(); ++it) {
      merged.insert(*it);
    }
    for (auto it = dms[src2].cbegin(); it != dms[src2].cend(); ++it) {
      merged.insert(*it);
    }
    dms[target] = merged;
    os << "Создано объединённое дерево: " << target << "\n";
  }
}

}
