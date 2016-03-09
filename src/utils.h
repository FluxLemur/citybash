#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
  public:
    static bool starts_with(std::string s1, std::string s2);
    static void upper(std::string &str);
    static std::string& trim(std::string &s);
    static std::vector<std::string> upper_trimmed_split(const std::string &str);
    static bool is_number(const std::string &str);
    static std::string random_upper_alphanum(const int len);
};

#endif  // UTILS_H
