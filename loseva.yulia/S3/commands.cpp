#include "commands.hpp"

#include <algorithm>
#include <set>
#include <sstream>
#include <vector>

namespace loseva {

static std::vector< std::string > tokenize(const std::string & line)
{
  std::vector< std::string > tokens;
  std::istringstream iss(line);
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

void runCommands(GraphTable & table, std::istream & in, std::ostream & out)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }

    const std::vector< std::string > tokens = tokenize(line);
    if (tokens.empty()) {
      continue;
    }

    const std::string & cmd = tokens[0];

    if (cmd == "graphs") {
      if (tokens.size() != 1) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > names;
      for (auto it = table.begin(); it != table.end(); ++it) {
        names.push_back((*it).first);
      }
      std::sort(names.begin(), names.end());
      if (names.empty()) {
        out << "\n";
      } else {
        for (const std::string & name : names) {
          out << name << "\n";
        }
      }
    }
    else if (cmd == "vertexes") {
      if (tokens.size() != 2) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & gName = tokens[1];
      if (!table.has(gName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & g = table.get(gName);
      const std::vector< std::string > verts = g.sortedVertices();
      if (verts.empty()) {
        out << "\n";
      } else {
        for (const std::string & v : verts) {
          out << v << "\n";
        }
      }
    }
    else if (cmd == "bind") {
      if (tokens.size() != 5) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & gName = tokens[1];
      if (!table.has(gName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & from = tokens[2];
      const std::string & to = tokens[3];

      try {
        std::size_t processed = 0;
        const long long wVal = std::stoll(tokens[4], &processed);
        if (processed != tokens[4].size() || wVal < 0) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        Graph & g = table.get(gName);
        g.addEdge(from, to, static_cast< unsigned int >(wVal));
      } catch (...) {
        out << "<INVALID COMMAND>\n";
      }
    }
    else if (cmd == "cut") {
      if (tokens.size() != 5) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & gName = tokens[1];
      if (!table.has(gName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & from = tokens[2];
      const std::string & to = tokens[3];

      try {
        std::size_t processed = 0;
        const long long wVal = std::stoll(tokens[4], &processed);
        if (processed != tokens[4].size() || wVal < 0) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        Graph & g = table.get(gName);
        if (!g.removeEdge(from, to, static_cast< unsigned int >(wVal))) {
          out << "<INVALID COMMAND>\n";
        }
      } catch (...) {
        out << "<INVALID COMMAND>\n";
      }
    }
    else if (cmd == "outbound") {
      if (tokens.size() != 3) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & gName = tokens[1];
      const std::string & vName = tokens[2];
      if (!table.has(gName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & g = table.get(gName);
      if (!g.hasVertex(vName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto outEdges = g.outbound(vName);
      for (const auto & pair : outEdges) {
        out << pair.first;
        for (const unsigned int w : pair.second) {
          out << " " << w;
        }
        out << "\n";
      }
    }
    else if (cmd == "inbound") {
      if (tokens.size() != 3) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & gName = tokens[1];
      const std::string & vName = tokens[2];
      if (!table.has(gName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & g = table.get(gName);
      if (!g.hasVertex(vName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto inEdges = g.inbound(vName);
      for (const auto & pair : inEdges) {
        out << pair.first;
        for (const unsigned int w : pair.second) {
          out << " " << w;
        }
        out << "\n";
      }
    }
    else if (cmd == "create") {
      if (tokens.size() < 3) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & gName = tokens[1];
      if (table.has(gName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      try {
        std::size_t processed = 0;
        const long long count = std::stoll(tokens[2], &processed);
        if (processed != tokens[2].size() || count < 0 ||
            tokens.size() != 3 + static_cast< std::size_t >(count)) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        Graph g(static_cast< std::size_t >(count * 2 + 16));
        for (std::size_t i = 0; i < static_cast< std::size_t >(count); ++i) {
          g.addVertex(tokens[3 + i]);
        }
        table.add(gName, g);
      } catch (...) {
        out << "<INVALID COMMAND>\n";
      }
    }
    else if (cmd == "merge") {
      if (tokens.size() != 4) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & target = tokens[1];
      const std::string & g1Name = tokens[2];
      const std::string & g2Name = tokens[3];
      if (table.has(target) || !table.has(g1Name) || !table.has(g2Name)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & g1 = table.get(g1Name);
      const Graph & g2 = table.get(g2Name);

      Graph merged(g1.edgeCapacity() + g2.edgeCapacity() + 16);
      const std::vector< std::string > v1 = g1.sortedVertices();
      for (const std::string & v : v1) {
        merged.addVertex(v);
      }
      const std::vector< std::string > v2 = g2.sortedVertices();
      for (const std::string & v : v2) {
        merged.addVertex(v);
      }
      for (const auto & entry : g1.edges()) {
        const auto & key = entry.first;
        const auto & weights = entry.second;
        for (const unsigned int w : weights) {
          merged.addEdge(key.first, key.second, w);
        }
      }
      for (const auto & entry : g2.edges()) {
        const auto & key = entry.first;
        const auto & weights = entry.second;
        for (const unsigned int w : weights) {
          merged.addEdge(key.first, key.second, w);
        }
      }
      table.add(target, merged);
    }
    else if (cmd == "extract") {
      if (tokens.size() < 4) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const std::string & target = tokens[1];
      const std::string & source = tokens[2];
      if (table.has(target) || !table.has(source)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      try {
        std::size_t processed = 0;
        const long long count = std::stoll(tokens[3], &processed);
        if (processed != tokens[3].size() || count < 0 ||
            tokens.size() != 4 + static_cast< std::size_t >(count)) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        const Graph & srcGraph = table.get(source);
        bool valid = true;
        std::set< std::string > allowedVertices;
        for (std::size_t i = 0; i < static_cast< std::size_t >(count); ++i) {
          const std::string & v = tokens[4 + i];
          if (!srcGraph.hasVertex(v)) {
            valid = false;
            break;
          }
          allowedVertices.insert(v);
        }
        if (!valid) {
          out << "<INVALID COMMAND>\n";
          continue;
        }
        Graph extracted(srcGraph.edgeCapacity() + 16);
        for (const std::string & v : allowedVertices) {
          extracted.addVertex(v);
        }
        for (const auto & entry : srcGraph.edges()) {
          const auto & key = entry.first;
          if (allowedVertices.count(key.first) && allowedVertices.count(key.second)) {
            for (const unsigned int w : entry.second) {
              extracted.addEdge(key.first, key.second, w);
            }
          }
        }
        table.add(target, extracted);
      } catch (...) {
        out << "<INVALID COMMAND>\n";
      }
    }
    else {
      out << "<INVALID COMMAND>\n";
    }
  }
}

}
