#include "commands.hpp"
#include <functional>
#include <limits>

namespace loseva {

struct CommandItem {
  std::string name;
  std::function< void(std::istream &, std::ostream &, DatasetsMap &, IteratorsMap &) > handler;

  bool operator<(const CommandItem & rhs) const noexcept { return name < rhs.name; }
  bool operator>(const CommandItem & rhs) const noexcept { return name > rhs.name; }
  bool operator==(const CommandItem & rhs) const noexcept { return name == rhs.name; }
};

void processCommands(std::istream & in, std::ostream & os)
{
  DatasetsMap datasets;
  IteratorsMap iterators;

  AVLTree< CommandItem > commandTree;

  commandTree.insert({"begin", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap & ims) { cmdBegin(is, out, dms, ims); }});
  commandTree.insert({"clear", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdClear(is, out, dms); }});
  commandTree.insert({"contains", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdContains(is, out, dms); }});
  commandTree.insert({"create", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdCreate(is, out, dms); }});
  commandTree.insert({"height", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdHeight(is, out, dms); }});
  commandTree.insert({"insert", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdInsert(is, out, dms); }});
  commandTree.insert({"iterator", [](std::istream & is, std::ostream & out, DatasetsMap &, IteratorsMap & ims) { cmdIterator(is, out, ims); }});
  commandTree.insert({"merge", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdMerge(is, out, dms); }});
  commandTree.insert({"next", [](std::istream & is, std::ostream & out, DatasetsMap &, IteratorsMap & ims) { cmdNext(is, out, ims); }});
  commandTree.insert({"print", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdPrint(is, out, dms); }});
  commandTree.insert({"remove", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdRemove(is, out, dms); }});
  commandTree.insert({"rotate-left", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdRotateLeft(is, out, dms); }});
  commandTree.insert({"rotate-right", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdRotateRight(is, out, dms); }});
  commandTree.insert({"search", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdSearch(is, out, dms); }});
  commandTree.insert({"tree", [](std::istream & is, std::ostream & out, DatasetsMap & dms, IteratorsMap &) { cmdTree(is, out, dms); }});

  std::string cmd;
  while (in >> cmd) {
    try {
      CommandItem dummy{cmd, nullptr};
      CommandItem * item = commandTree.find(dummy);
      if (!item) {
        throw CommandException("<INVALID COMMAND>");
      }
      item->handler(in, os, datasets, iterators);
    } catch (const std::exception &) {
      os << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

}
