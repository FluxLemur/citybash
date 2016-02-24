#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
  public:
    static bool starts_with(std::string s1, std::string s2);
    static std::vector<std::string> split(const std::string &s, char delimiter);
    static std::string &trim(std::string &s);
    static void upper(std::string &s);
};

#endif  // UTILS_H
