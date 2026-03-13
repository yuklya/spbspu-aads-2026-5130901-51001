#include <cassert>
#include "list.hpp"

using namespace loseva;

int main()
{
  List<int> list;

  assert(list.empty());

  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  assert(list.front() == 1);
  assert(list.back() == 3);

  int sum = 0;

  for (auto it = list.begin(); it != list.end(); ++it)
    sum += *it;

  assert(sum == 6);

  list.pop_front();
  assert(list.front() == 2);

  list.pop_back();
  assert(list.back() == 2);

  list.clear();
  assert(list.empty());
}
