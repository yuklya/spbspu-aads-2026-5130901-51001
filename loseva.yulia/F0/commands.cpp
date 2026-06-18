#include "commands.hpp"
#include <sstream>

namespace loseva {

void processCommands(std::istream & in, std::ostream & out)
{
  DatasetsMap datasets;
  IteratorsMap iterators;
  std::string line;

  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    std::stringstream ss(line);
    std::string cmd;
    if (!(ss >> cmd)) {
      continue;
    }

    if (cmd == "create") {
      cmdCreate(ss, out, datasets);
    } else if (cmd == "insert") {
      cmdInsert(ss, out, datasets);
    } else if (cmd == "remove") {
      cmdRemove(ss, out, datasets);
    } else if (cmd == "search") {
      cmdSearch(ss, out, datasets);
    } else if (cmd == "print") {
      cmdPrint(ss, out, datasets);
    } else if (cmd == "tree") {
      cmdTree(ss, out, datasets);
    } else if (cmd == "height") {
      cmdHeight(ss, out, datasets);
    } else if (cmd == "clear") {
      cmdClear(ss, out, datasets);
    } else if (cmd == "rotate-left") {
      cmdRotateLeft(ss, out, datasets);
    } else if (cmd == "rotate-right") {
      cmdRotateRight(ss, out, datasets);
    } else if (cmd == "merge") {
      cmdMerge(ss, out, datasets);
    } else if (cmd == "contains") {
      cmdContains(ss, out, datasets);
    } else if (cmd == "begin") {
      cmdBegin(ss, out, datasets, iterators);
    } else if (cmd == "next") {
      cmdNext(ss, out, iterators);
    } else if (cmd == "iterator") {
      cmdIterator(ss, out, iterators);
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }
}

}
