#ifndef LOSEVA_COMMANDS_HPP
#define LOSEVA_COMMANDS_HPP

#include "avl_tree.hpp"
#include <map>
#include <string>
#include <iostream>

namespace loseva {

using Dataset = AVLTree< int >;
using DatasetsMap = std::map< std::string, Dataset >;
using IteratorsMap = std::map< std::string, Dataset::const_iterator >;

void cmdCreate(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdInsert(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdRemove(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdSearch(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdPrint(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdTree(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdHeight(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdClear(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdRotateLeft(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdRotateRight(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdMerge(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdContains(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdBegin(std::istream & is, std::ostream & os, DatasetsMap & dms, IteratorsMap & ims);
void cmdNext(std::istream & is, std::ostream & os, IteratorsMap & ims);
void cmdIterator(std::istream & is, std::ostream & os, IteratorsMap & ims);

void processCommands(std::istream & in, std::ostream & out);

}

#endif
