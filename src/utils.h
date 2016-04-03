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

    /* Returns 0 if stoi fails */
    static int safe_stoi(const std::string &str);

    /* Return a random integer in the range [min, max) */
    static int random_int(const int max);
};

#endif  // UTILS_H
