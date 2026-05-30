#include "commands.hpp"
#include "graph.hpp"
#include "hash_table.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

namespace loseva {

void runCommands(GraphTable & table, std::istream & in, std::ostream & out)
{
  std::string line = "";
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream ss(line);
    std::string cmd = "";
    if (!(ss >> cmd)) {
      continue;
    }

    if (cmd == "graphs") {
      std::string extra = "";
      if (ss >> extra) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > names;
      for (auto it = table.begin(); it != table.end(); ++it) {
        names.push_back((*it).first);
      }
      std::sort(names.begin(), names.end());
      for (const auto & name : names) {
        out << name << "\n";
      }
    }
    else if (cmd == "vertexes") {
      std::string graphName = "";
      if (!(ss >> graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string extra = "";
      if ((ss >> extra) || !table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & g = table.get(graphName);
      const std::vector< std::string > verts = g.sortedVertices();
      for (const auto & v : verts) {
        out << v << "\n";
      }
    }
    else if (cmd == "bind") {
      std::string graphName = "";
      std::string from = "";
      std::string to = "";
      unsigned int weight = 0;
      if (!(ss >> graphName >> from >> to >> weight)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string extra = "";
      if ((ss >> extra) || !table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      Graph & g = table.get(graphName);
      g.addEdge(from, to, weight);
    }
    else if (cmd == "cut") {
      std::string graphName = "";
      std::string from = "";
      std::string to = "";
      unsigned int weight = 0;
      if (!(ss >> graphName >> from >> to >> weight)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string extra = "";
      if ((ss >> extra) || !table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      Graph & g = table.get(graphName);
      if (!g.hasVertex(from) || !g.hasVertex(to) || !g.removeEdge(from, to, weight)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
    }
    else if (cmd == "inbound") {
      std::string graphName = "";
      std::string vertexName = "";
      if (!(ss >> graphName >> vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string extra = "";
      if ((ss >> extra) || !table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & g = table.get(graphName);
      if (!g.hasVertex(vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto edges = g.inbound(vertexName);
      for (const auto & edge : edges) {
        out << edge.first;
        for (const unsigned int w : edge.second) {
          out << " " << w;
        }
        out << "\n";
      }
    }
    else if (cmd == "outbound") {
      std::string graphName = "";
      std::string vertexName = "";
      if (!(ss >> graphName >> vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string extra = "";
      if ((ss >> extra) || !table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & g = table.get(graphName);
      if (!g.hasVertex(vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto edges = g.outbound(vertexName);
      for (const auto & edge : edges) {
        out << edge.first;
        for (const unsigned int w : edge.second) {
          out << " " << w;
        }
        out << "\n";
      }
    }
    else if (cmd == "create") {
      std::string graphName = "";
      int numVertices = 0;
      if (!(ss >> graphName >> numVertices) || numVertices < 0) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > verts;
      std::string v = "";
      bool parseError = false;
      for (int i = 0; i < numVertices; ++i) {
        if (!(ss >> v)) {
          parseError = true;
          break;
        }
        verts.push_back(v);
      }
      std::string extra = "";
      if (parseError || (ss >> extra) || table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      Graph g;
      for (const auto & vertex : verts) {
        g.addVertex(vertex);
      }
      bool inserted = false;
      while (!inserted) {
        try {
          table.add(graphName, g);
          inserted = true;
        } catch (...) {
          table.rehash(table.capacity() * 2 + 1);
        }
      }
    }
    else if (cmd == "merge") {
      std::string resGraph = "";
      std::string g1 = "";
      std::string g2 = "";
      if (!(ss >> resGraph >> g1 >> g2)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string extra = "";
      if ((ss >> extra) || table.has(resGraph) || !table.has(g1) || !table.has(g2)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & graph1 = table.get(g1);
      const Graph & graph2 = table.get(g2);
      Graph resGraphObj;

      const std::vector< std::string > verts1 = graph1.sortedVertices();
      for (const auto & v : verts1) {
        resGraphObj.addVertex(v);
        const auto edges = graph1.outbound(v);
        for (const auto & edge : edges) {
          for (const unsigned int w : edge.second) {
            resGraphObj.addEdge(v, edge.first, w);
          }
        }
      }

      const std::vector< std::string > verts2 = graph2.sortedVertices();
      for (const auto & v : verts2) {
        resGraphObj.addVertex(v);
        const auto edges = graph2.outbound(v);
        for (const auto & edge : edges) {
          for (const unsigned int w : edge.second) {
            resGraphObj.addEdge(v, edge.first, w);
          }
        }
      }

      bool inserted = false;
      while (!inserted) {
        try {
          table.add(resGraph, resGraphObj);
          inserted = true;
        } catch (...) {
          table.rehash(table.capacity() * 2 + 1);
        }
      }
    }
    else if (cmd == "extract") {
      std::string resGraph = "";
      std::string srcGraph = "";
      int numVertices = 0;
      if (!(ss >> resGraph >> srcGraph >> numVertices) || numVertices < 0) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > verts;
      std::string v = "";
      bool parseError = false;
      for (int i = 0; i < numVertices; ++i) {
        if (!(ss >> v)) {
          parseError = true;
          break;
        }
        verts.push_back(v);
      }
      std::string extra = "";
      if (parseError || (ss >> extra) || table.has(resGraph) || !table.has(srcGraph)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const Graph & source = table.get(srcGraph);
      bool allExist = true;
      for (const auto & vertex : verts) {
        if (!source.hasVertex(vertex)) {
          allExist = false;
          break;
        }
      }
      if (!allExist) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      Graph resGraphObj;
      for (const auto & vertex : verts) {
        resGraphObj.addVertex(vertex);
      }
      for (const auto & from : verts) {
        const auto edges = source.outbound(from);
        for (const auto & edge : edges) {
          const std::string & to = edge.first;
          const auto it = std::find(verts.begin(), verts.end(), to);
          if (it != verts.end()) {
            for (const unsigned int w : edge.second) {
              resGraphObj.addEdge(from, to, w);
            }
          }
        }
      }
      bool inserted = false;
      while (!inserted) {
        try {
          table.add(resGraph, resGraphObj);
          inserted = true;
        } catch (...) {
          table.rehash(table.capacity() * 2 + 1);
        }
      }
    }
    else {
      out << "<INVALID COMMAND>\n";
    }
  }
}

}
