#include "commands.hpp"
#include <sstream>
#include <algorithm>

namespace loseva {

void handleOutbound(GraphTable & table, std::istream & is, std::ostream & os)
{
  std::string graphName, vertex;
  if (!(is >> graphName >> vertex)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  if (!table.has(graphName)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  const auto & g = table.at(graphName);
  if (!g.hasVertex(vertex)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  auto edges = g.outbound(vertex);
  if (edges.empty()) {
    os << "\n";
    return;
  }
  for (const auto & edge : edges) {
    for (unsigned int w : edge.second) {
      os << edge.first << " " << w << "\n";
    }
  }
}

void handleInbound(GraphTable & table, std::istream & is, std::ostream & os)
{
  std::string graphName, vertex;
  if (!(is >> graphName >> vertex)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  if (!table.has(graphName)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  const auto & g = table.at(graphName);
  if (!g.hasVertex(vertex)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  auto edges = g.inbound(vertex);
  if (edges.empty()) {
    os << "\n";
    return;
  }
  for (const auto & edge : edges) {
    for (unsigned int w : edge.second) {
      os << edge.first << " " << w << "\n";
    }
  }
}

void handleMerge(GraphTable & table, std::istream & is, std::ostream & os)
{
  std::string target, g1, g2;
  if (!(is >> target >> g1 >> g2)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  if (table.has(target) || !table.has(g1) || !table.has(g2)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  const auto & graph1 = table.at(g1);
  const auto & graph2 = table.at(g2);
  
  Graph merged;
  for (const auto & v : graph1.sortedVertices()) {
    merged.addVertex(v);
  }
  for (const auto & v : graph2.sortedVertices()) {
    merged.addVertex(v);
  }
  
  for (const auto & v : graph1.sortedVertices()) {
    for (const auto & edge : graph1.outbound(v)) {
      for (unsigned int w : edge.second) {
        merged.addEdge(v, edge.first, w);
      }
    }
  }
  for (const auto & v : graph2.sortedVertices()) {
    for (const auto & edge : graph2.outbound(v)) {
      for (unsigned int w : edge.second) {
        merged.addEdge(v, edge.first, w);
      }
    }
  }
  
  table.insert_or_assign(target, merged);
}

void handleExtract(GraphTable & table, std::istream & is, std::ostream & os)
{
  std::string target, source;
  std::size_t count = 0;
  if (!(is >> target >> source >> count)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  if (table.has(target) || !table.has(source)) {
    os << "<INVALID COMMAND>\n";
    return;
  }
  const auto & srcGraph = table.at(source);
  std::vector<std::string> allowedVertices;
  for (std::size_t i = 0; i < count; ++i) {
    std::string v;
    if (!(is >> v)) {
      os << "<INVALID COMMAND>\n";
      return;
    }
    if (!srcGraph.hasVertex(v)) {
      os << "<INVALID COMMAND>\n";
      return;
    }
    allowedVertices.push_back(v);
  }
  
  Graph extracted;
  for (const auto & v : allowedVertices) {
    extracted.addVertex(v);
  }
  
  for (const auto & v : allowedVertices) {
    for (const auto & edge : srcGraph.outbound(v)) {
      if (std::find(allowedVertices.begin(), allowedVertices.end(), edge.first) 
          != allowedVertices.end()) {
        for (unsigned int w : edge.second) {
          extracted.addEdge(v, edge.first, w);
        }
      }
    }
  }
  
  table.insert_or_assign(target, extracted);
}

void runCommands(GraphTable & table, std::istream & in, std::ostream & out)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string cmd;
    if (!(iss >> cmd)) {
      continue;
    }
    if (cmd == "outbound") {
      handleOutbound(table, iss, out);
    } else if (cmd == "inbound") {
      handleInbound(table, iss, out);
    } else if (cmd == "merge") {
      handleMerge(table, iss, out);
    } else if (cmd == "extract") {
      handleExtract(table, iss, out);
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }
}

}
