
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

#include "location.h"
#include "utils.h"

void test_string_processing();
void test_locations();
void distance_approx(Location l1, Location l2, float target);

void test_string_processing() {
  std::string str = "";
  std::vector<std::string> split_str = Utils::upper_trimmed_split(str);
  assert(split_str.size() == 0);

  str = "a";
  split_str = Utils::upper_trimmed_split(str);
  assert(split_str.size() == 1);
  assert(split_str[0] == "A");

  str = "\nTeSt \n";
  split_str = Utils::upper_trimmed_split(str);
  assert(split_str.size() == 1);
  assert(split_str[0] == "TEST");

  str = "Attack city1 105\n";
  split_str = Utils::upper_trimmed_split(str);
  assert(split_str.size() == 3);
  assert(split_str[0] == "ATTACK");
  assert(split_str[1] == "CITY1");
  assert(split_str[2] == "105");

  str = " train 4000\n";
  split_str = Utils::upper_trimmed_split(str);
  assert(split_str.size() == 2);
  assert(split_str[0] == "TRAIN");
  assert(split_str[1] == "4000");
}

void distance_approx(Location l1, Location l2, float target) {
  assert(Location::distance_between(l1, l2) - target < 0.00001);
}

void test_locations() {
  Location::Location l1 = Location(0, 0);
  Location::Location l2 = Location(0, 0);
  distance_approx(l1, l2, 0);

  l2 = Location(0, 5);
  distance_approx(l1, l2, 5.0);

  l2 = Location(5, 0);
  distance_approx(l1, l2, 5.0);

  l2 = Location(3, 4);
  distance_approx(l1, l2, 5.0);
}

int main() {
  test_string_processing();
  test_locations();

  std::cout << "All tests pass!\n";
  return 0;
}
