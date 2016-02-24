
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

void test_string_processing() {
  std::string str = "";
  std::vector<std::string> split_str = Utils::upper_trimmed_split(str, ' ');
  assert(split_str.size() == 0);

  str = "a";
  split_str = Utils::upper_trimmed_split(str, ' ');
  assert(split_str.size() == 1);
  assert(split_str[0] == "A");

  str = "\nTeSt \n";
  split_str = Utils::upper_trimmed_split(str, ' ');
  assert(split_str.size() == 1);
  assert(split_str[0] == "TEST");

  str = "Attack city1 105\n";
  split_str = Utils::upper_trimmed_split(str, ' ');
  assert(split_str.size() == 3);
  assert(split_str[0] == "ATTACK");
  assert(split_str[1] == "CITY1");
  assert(split_str[2] == "105");

  str = " train 4000\n";
  split_str = Utils::upper_trimmed_split(str, ' ');
  assert(split_str.size() == 2);
  assert(split_str[0] == "TRAIN");
  assert(split_str[1] == "4000");
}

int main() {
  test_string_processing();

  std::cout << "All tests pass!\n";
  return 0;
}
