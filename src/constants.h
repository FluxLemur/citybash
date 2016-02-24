#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <string>

const int PORT = 12345;

class Requests {
  public:
    static const std::string WORLD;
    static const std::string CITY;
};

/*
static const std::string COSTS = "COSTS";
static const std::string UPGRADE = "UPGRADE";
static const std::string ATTACK = "ATTACK";
static const std::string TRAIN = "TRAIN";
*/

class Responses {
  public:
    static const std::string INVALID;
    static const std::string NOT_IMPLEMENTED;
};

#endif  // CONSTANTS_H
