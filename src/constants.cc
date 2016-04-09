#include "constants.h"

const std::string Requests::JOIN = "JOIN";
const std::string Requests::WORLD = "WORLD";
const std::string Requests::CITY = "CITY";
const std::string Requests::SHORTCITY = "SHORTCITY";
const std::string Requests::COSTS = "COSTS";
const std::string Requests::UPGRADE = "UPGRADE";
const std::string Requests::ATTACK = "ATTACK";
const std::string Requests::TRAIN = "TRAIN";

const std::string Responses::INVALID = "INVALID REQUEST\n";
const std::string Responses::INVALID_PLAYER_KEY = "INVALID PLAYER KEY\n";
const std::string Responses::INVALID_TRAIN =
                  "INVALID TRAIN. "
                  "VALID: [player key] TRAIN [# soldiers]\n";
const std::string Responses::INVALID_ATTACK =
                  "INVALID ATTACK. "
                  "VALID: [player key] ATTACK [other city name] [# soldiers]\n";
const std::string Responses::NOT_IMPLEMENTED = "NOT IMPLEMENTED\n";
