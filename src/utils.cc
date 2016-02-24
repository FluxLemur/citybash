#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

#include "utils.h"

bool Utils::starts_with(std::string s1, std::string s2) {
  if (s1.size() > s2.size()) {
    return !s1.compare(0, s2.size(), s2);
  }
  return !s2.compare(0, s1.size(), s1);
}

/* http://stackoverflow.com/a/236803/5610569
 */
static std::vector<std::string>& split_helper(const std::string &s, char delim,
                                      std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> Utils::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split_helper(s, delim, elems);
    return elems;
}

/* http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
// trim from start
static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
std::string& Utils::trim(std::string &s) {
  return ltrim(rtrim(s));
}

/* http://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
 */
void Utils::upper(std::string &str) {
  std::transform(str.begin(), str.end(),str.begin(), ::toupper);
}
