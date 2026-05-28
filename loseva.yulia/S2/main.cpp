#include <iostream>
#include <string>
#include <sstream>
#include "list.hpp"

using namespace loseva;

int main() {
  List<std::pair<std::string, List<int>>> data;
  std::string line;

  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    std::string name;
    if (!(ss >> name)) {
      std::cerr << "Invalid format: empty line or missing name\n";
      return 1;
    }

    List<int> numbers;
    int x = 0;
    while (ss >> x) {
      numbers.push_back(x);
    }

    if (!ss.eof()) {
      std::cerr << "Invalid format: input contains non-integer values\n";
      return 1;
    }

    data.push_back({name, numbers});
  }

  if (data.empty()) {
    std::cout << 0 << "\n";
    return 0;
  }

  for (auto it = data.begin(); it != data.end(); ++it) {
    std::cout << (*it).first << " ";
  }
  std::cout << "\n";

  List<typename List<int>::iterator> its;
  List<typename List<int>::iterator> ends;

  for (auto it = data.begin(); it != data.end(); ++it) {
    its.push_back((*it).second.begin());
    ends.push_back((*it).second.end());
  }

  List<int> results;

  bool done = false;
  while (!done) {
    done = true;

    auto it_it = its.begin();
    auto it_end = ends.begin();

    int res = 0;
    bool has = false;

    while (it_it != its.end()) {
      if ((*it_it) != *it_end) {
        const int val = **it_it;
        if (!has) {
          res = val;
          has = true;
        } else {
          if (val < res) {
            res = val;
          }
        }
        done = false;
      }
      ++it_it;
      ++it_end;
    }

    if (!done) {
      it_it = its.begin();
      it_end = ends.begin();

      while (it_it != its.end()) {
        if ((*it_it) != *it_end) {
          if (**it_it == res) {
            ++(*it_it);
          }
        }
        ++it_it;
        ++it_end;
      }

      if (has) {
        results.push_back(res);
      }
    }
  }

  for (auto it = results.begin(); it != results.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << "\n";

  return 0;
}
