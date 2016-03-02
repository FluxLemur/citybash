#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <string>

// exit time destructor warnings:
// http://stackoverflow.com/q/14335494/5610569
#define CR_DEFINE_STATIC_LOCAL(type, name, arguments) \
  static type& name = *new type arguments

const int PORT = 12345;

class Requests {
  public:
    static const std::string WORLD;
    static const std::string CITY;
    static const std::string COSTS;
    static const std::string UPGRADE;
    static const std::string ATTACK;
    static const std::string TRAIN;
};


class Responses {
  public:
    static const std::string INVALID;
    static const std::string INVALID_CITY_HASH;
    static const std::string INVALID_TRAIN;
    static const std::string INVALID_ATTACK;
    static const std::string NOT_IMPLEMENTED;
};

#endif  // CONSTANTS_H
