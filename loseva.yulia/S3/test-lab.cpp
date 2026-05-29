#define BOOST_TEST_MODULE lab_s3_tests
#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "hash_table.hpp"
#include "xxhash.hpp"
#include "sequence.hpp"
#include "sort_utils.hpp"
#include "graph.hpp"
#include "commands.hpp"
#include "parser.hpp"

namespace {

struct IntLess {
  bool operator()(int lhs, int rhs) const
  {
    return lhs < rhs;
  }
};

struct StringLess {
  bool operator()(const std::string & lhs, const std::string & rhs) const
  {
    return lhs < rhs;
  }
};

struct ConstantHash {
  std::size_t operator()(const std::string &) const
  {
    return 0;
  }
};

using CollisionTable = loseva::HashTable<
  std::string, int, ConstantHash, loseva::StringEqual >;

using StringTable = loseva::HashTable<
  std::string, int, loseva::StringXxHash, loseva::StringEqual >;

}

BOOST_AUTO_TEST_CASE(sequence_operations)
{
  loseva::Sequence< int > values;
  BOOST_TEST(values.empty());
  BOOST_TEST(values.size() == 0u);

  values.push_back(10);
  values.push_back(20);
  values.push_back(30);
  BOOST_TEST(!values.empty());
  BOOST_TEST(values.size() == 3u);
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(values[1] == 20);
  BOOST_TEST(values[2] == 30);

  values.reserve(16);
  BOOST_TEST(values.size() == 3u);
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(values[1] == 20);
  BOOST_TEST(values[2] == 30);

  values.erase_at(1);
  BOOST_TEST(values.size() == 2u);
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(values[1] == 30);

  loseva::Sequence< int > copy(values);
  copy[0] = 100;
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(copy[0] == 100);

  loseva::Sequence< int > assigned;
  assigned.push_back(1);
  assigned = values;
  assigned[1] = 300;
  BOOST_TEST(values[1] == 30);
  BOOST_TEST(assigned[1] == 300);

  loseva::Sequence< int > moved(std::move(values));
  BOOST_TEST(moved.size() == 2u);
  BOOST_TEST(moved[0] == 10);
  BOOST_TEST(moved[1] == 30);

  loseva::Sequence< int > moveAssigned;
  moveAssigned = std::move(moved);
  BOOST_TEST(moveAssigned.size() == 2u);
  BOOST_TEST(moveAssigned[0] == 10);
  BOOST_TEST(moveAssigned[1] == 30);

  moveAssigned.clear();
  BOOST_TEST(moveAssigned.empty());
  BOOST_TEST(moveAssigned.size() == 0u);
}

BOOST_AUTO_TEST_CASE(sequence_erase_at_out_of_range)
{
  loseva::Sequence< int > s;
  s.push_back(1);
  BOOST_CHECK_THROW(s.erase_at(5), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(sequence_reserve_does_not_shrink)
{
  loseva::Sequence< int > s;
  s.push_back(1);
  s.reserve(100);
  BOOST_TEST(s.capacity() >= 100u);
  s.reserve(10);
  BOOST_TEST(s.capacity() >= 100u);
}

BOOST_AUTO_TEST_CASE(sort_helper_orders_sequences)
{
  loseva::Sequence< std::string > strings;
  strings.push_back("beta");
  strings.push_back("alpha");
  strings.push_back("alpha");
  strings.push_back("gamma");
  loseva::sort(strings, StringLess());
  BOOST_TEST(strings[0] == "alpha");
  BOOST_TEST(strings[1] == "alpha");
  BOOST_TEST(strings[2] == "beta");
  BOOST_TEST(strings[3] == "gamma");

  loseva::Sequence< int > numbers;
  numbers.push_back(5);
  numbers.push_back(1);
  numbers.push_back(3);
  numbers.push_back(1);
  loseva::sort(numbers, IntLess());
  BOOST_TEST(numbers[0] == 1);
  BOOST_TEST(numbers[1] == 1);
  BOOST_TEST(numbers[2] == 3);
  BOOST_TEST(numbers[3] == 5);

  loseva::Sequence< int > empty;
  loseva::sort(empty, IntLess());
  BOOST_TEST(empty.empty());

  loseva::Sequence< int > single;
  single.push_back(42);
  loseva::sort(single, IntLess());
  BOOST_TEST(single.size() == 1u);
  BOOST_TEST(single[0] == 42);
}

BOOST_AUTO_TEST_CASE(xxhash_is_stable_and_distinguishes_keys)
{
  const loseva::StringXxHash hash;
  BOOST_TEST(hash("same") == hash("same"));
  BOOST_TEST(hash("same") != hash("other"));
  BOOST_TEST(hash("") == hash(""));
  BOOST_TEST(hash("a") != hash("b"));

  const unsigned char bytes[] = { 0, 1, 2, 3, 255 };
  BOOST_TEST(
    hash.hashBytes(bytes, sizeof(bytes))
    == hash.hashBytes(bytes, sizeof(bytes)));

  const loseva::EdgeKey first{ "ab", "c" };
  const loseva::EdgeKey second{ "a", "bc" };
  const loseva::EdgeKey firstCopy{ "ab", "c" };
  const loseva::PairStringXxHash edgeHash;
  BOOST_TEST(edgeHash(first) != edgeHash(second));
  BOOST_TEST(edgeHash(first) == edgeHash(firstCopy));
}

BOOST_AUTO_TEST_CASE(hash_table_basic_operations)
{
  StringTable table(23);
  table.add("one", 1);
  BOOST_TEST(table.has("one"));
  BOOST_TEST(!table.has("missing"));
  BOOST_TEST(table.at("one") == 1);

  table.insert_or_assign("one", 11);
  BOOST_TEST(table.at("one") == 11);
  BOOST_CHECK_THROW(table.add("one", 1), std::logic_error);

  const int dropped = table.drop("one");
  BOOST_TEST(dropped == 11);
  BOOST_TEST(!table.has("one"));
  BOOST_CHECK_THROW(table.drop("one"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(hash_table_at_missing_throws)
{
  StringTable table(8);
  BOOST_CHECK_THROW(table.at("nope"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(hash_table_insert_or_assign_new_key)
{
  StringTable table(16);
  table.insert_or_assign("key", 42);
  BOOST_TEST(table.has("key"));
  BOOST_TEST(table.at("key") == 42);
  table.insert_or_assign("key", 99);
  BOOST_TEST(table.at("key") == 99);
  BOOST_TEST(table.size() == 1u);
}

BOOST_AUTO_TEST_CASE(hash_table_tombstone_iterator_and_rehash)
{
  CollisionTable table(11);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.drop("b");
  BOOST_TEST(table.has("c"));
  table.add("d", 4);
  BOOST_TEST(table.has("d"));
  BOOST_TEST(table.at("d") == 4);

  std::size_t visited = 0;
  int sum = 0;
  for (CollisionTable::iterator it = table.begin(); it != table.end(); ++it) {
    BOOST_TEST(it->key() != "b");
    ++visited;
    sum += it->value();
  }
  BOOST_TEST(visited == 3u);
  BOOST_TEST(sum == 8);

  const CollisionTable & constTable = table;
  std::size_t constVisited = 0;
  for (CollisionTable::const_iterator it = constTable.begin(); 
       it != constTable.end(); ++it) {
    BOOST_TEST(it->key() != "b");
    ++constVisited;
  }
  BOOST_TEST(constVisited == visited);

  table.rehash(17);
  BOOST_TEST(table.has("a"));
  BOOST_TEST(table.has("c"));
  BOOST_TEST(table.has("d"));
  BOOST_TEST(!table.has("b"));

  table.add("e", 5);
  BOOST_TEST(table.has("e"));
}

BOOST_AUTO_TEST_CASE(hash_table_full_table_throws)
{
  CollisionTable table(2);
  table.add("a", 1);
  table.add("b", 2);
  BOOST_CHECK_THROW(table.add("c", 3), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(hash_table_copy_move_and_clear)
{
  CollisionTable original(17);
  original.add("a", 1);
  original.add("b", 2);

  CollisionTable copy(original);
  copy.insert_or_assign("a", 10);
  BOOST_TEST(original.at("a") == 1);
  BOOST_TEST(copy.at("a") == 10);

  CollisionTable assigned(17);
  assigned.add("x", 9);
  assigned = original;
  assigned.insert_or_assign("b", 20);
  BOOST_TEST(original.at("b") == 2);
  BOOST_TEST(assigned.at("b") == 20);

  CollisionTable moved(std::move(copy));
  BOOST_TEST(moved.has("a"));
  BOOST_TEST(moved.has("b"));
}
