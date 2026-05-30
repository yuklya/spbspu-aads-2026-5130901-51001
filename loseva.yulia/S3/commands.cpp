#include "commands.hpp"

#include <algorithm>
#include <cctype>
#include <limits>
#include <sstream>
#include <vector>

namespace loseva {

void runCommands(GraphTable & table, std::istream & in, std::ostream & out)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::vector< std::string > tokens;
    std::string token;
    while (iss >> token) {
      tokens.push_back(token);
    }

    if (tokens.empty()) {
      continue;
    }

    const std::string cmd = tokens[0];

    if (cmd == "create") {
      if (tokens.size() != 2) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string graphName = tokens[1];
      if (table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      bool inserted = false;
      while (!inserted) {
        try {
          table.add(graphName, Graph());
          inserted = true;
        } catch (const TableFullException &) {
          table.rehash(table.capacity() * 2 + 1);
        }
      }
    } else if (cmd == "graphs") {
      if (tokens.size() != 1) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > names;
      for (const auto & entry : table) {
        names.push_back(entry.first);
      }
      std::sort(names.begin(), names.end());
      if (names.empty()) {
        out << "\n";
      } else {
        for (const auto & name : names) {
          out << name << "\n";
        }
      }
    } else if (cmd == "vertexes" || cmd == "vertices") {
      if (tokens.size() != 2) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string graphName = tokens[1];
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & g = table.at(graphName);
      const auto verts = g.sortedVertices();
      if (verts.empty()) {
        out << "\n";
      } else {
        for (const auto & v : verts) {
          out << v << "\n";
        }
      }
    } else if (cmd == "bind") {
      if (tokens.size() != 5) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string graphName = tokens[1];
      const std::string from = tokens[2];
      const std::string to = tokens[3];
      const std::string weightStr = tokens[4];

      bool isNum = !weightStr.empty();
      for (const char c : weightStr) {
        if (!std::isdigit(c)) {
          isNum = false;
        }
      }
      if (!isNum) {
        out << "<INVALID COMMAND>\n";
        continue;
      }

      const unsigned long long wVal = std::stoull(weightStr);
      if (wVal > std::numeric_limits< unsigned int >::max()) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const unsigned int weight = static_cast< unsigned int >(wVal);

      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      table.at(graphName).addEdge(from, to, weight);
    } else if (cmd == "cut") {
      if (tokens.size() != 5) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string graphName = tokens[1];
      const std::string from = tokens[2];
      const std::string to = tokens[3];
      const std::string weightStr = tokens[4];

      bool isNum = !weightStr.empty();
      for (const char c : weightStr) {
        if (!std::isdigit(c)) {
          isNum = false;
        }
      }
      if (!isNum) {
        out << "<INVALID COMMAND>\n";
        continue;
      }

      const unsigned long long wVal = std::stoull(weightStr);
      if (wVal > std::numeric_limits< unsigned int >::max()) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const unsigned int weight = static_cast< unsigned int >(wVal);

      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.at(graphName).removeEdge(from, to, weight)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
    } else if (cmd == "outbound") {
      if (tokens.size() != 3) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string graphName = tokens[1];
      const std::string vertex = tokens[2];
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & g = table.at(graphName);
      if (!g.hasVertex(vertex)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto adj = g.outbound(vertex);
      bool printed = false;
      for (const auto & p : adj) {
        for (const unsigned int w : p.second) {
          out << p.first << " " << w << "\n";
          printed = true;
        }
      }
      if (!printed) {
        out << "\n";
      }
    } else if (cmd == "inbound") {
      if (tokens.size() != 3) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string graphName = tokens[1];
      const std::string vertex = tokens[2];
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & g = table.at(graphName);
      if (!g.hasVertex(vertex)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto adj = g.inbound(vertex);
      bool printed = false;
      for (const auto & p : adj) {
        for (const unsigned int w : p.second) {
          out << p.first << " " << w << "\n";
          printed = true;
        }
      }
      if (!printed) {
        out << "\n";
      }
    } else if (cmd == "merge") {
      if (tokens.size() != 4) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string target = tokens[1];
      const std::string g1 = tokens[2];
      const std::string g2 = tokens[3];
      if (!table.has(g1) || !table.has(g2) || table.has(target)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (target == g1 || target == g2) {
        out << "<INVALID COMMAND>\n";
        continue;
      }

      Graph res;
      auto copyG = [](const Graph & src, Graph & dest) {
        const auto verts = src.sortedVertices();
        for (const auto & v : verts) {
          dest.addVertex(v);
          const auto adj = src.outbound(v);
          for (const auto & p : adj) {
            for (const unsigned int w : p.second) {
              dest.addEdge(v, p.first, w);
            }
          }
        }
      };

      copyG(table.at(g1), res);
      copyG(table.at(g2), res);

      bool inserted = false;
      while (!inserted) {
        try {
          table.add(target, res);
          inserted = true;
        } catch (const TableFullException &) {
          table.rehash(table.capacity() * 2 + 1);
        }
      }
    } else if (cmd == "extract") {
      if (tokens.size() < 4) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string target = tokens[1];
      const std::string source = tokens[2];
      const std::string countStr = tokens[3];

      bool isCountNum = !countStr.empty();
      for (const char c : countStr) {
        if (!std::isdigit(c)) {
          isCountNum = false;
        }
      }
      if (!isCountNum) {
        out << "<INVALID COMMAND>\n";
        continue;
      }

      const std::size_t count = std::stoull(countStr);
      if (tokens.size() != 4 + count) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.has(source) || table.has(target) || target == source) {
        out << "<INVALID COMMAND>\n";
        continue;
      }

      const auto & srcGraph = table.at(source);
      bool allExist = true;
      std::vector< std::string > allowedVertices;
      for (std::size_t i = 0; i < count; ++i) {
        const std::string v = tokens[4 + i];
        if (!srcGraph.hasVertex(v)) {
          allExist = false;
        }
        allowedVertices.push_back(v);
      }
      if (!allExist) {
        out << "<INVALID COMMAND>\n";
        continue;
      }

      Graph res;
      for (const auto & v : allowedVertices) {
        res.addVertex(v);
      }
      for (const auto & v : allowedVertices) {
        const auto adj = srcGraph.outbound(v);
        for (const auto & p : adj) {
          bool found = false;
          for (const auto & av : allowedVertices) {
            if (av == p.first) {
              found = true;
              break;
            }
          }
          if (found) {
            for (const unsigned int w : p.second) {
              res.addEdge(v, p.first, w);
            }
          }
        }
      }

      bool inserted = false;
      while (!inserted) {
        try {
          table.add(target, res);
          inserted = true;
        } catch (const TableFullException &) {
          table.rehash(table.capacity() * 2 + 1);
        }
      }
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }
}

}
