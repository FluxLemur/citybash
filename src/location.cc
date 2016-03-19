#include <cmath>
#include <iostream>

#include "location.h"
#include "utils.h"

Location::Location(int x, int y) {
  x_ = x;
  y_ = y;
}
Location* Location::random_location(int width) {
  Location* l = new Location(Utils::random_int(width),
                             Utils::random_int(width));

  return l;
}

float Location::distance_between(Location l1, Location l2) {
  float dx = l1.x_ - l2.x_;
  float dy = l1.y_ - l2.y_;
  return std::sqrt(dx * dx + dy * dy);
}

std::ostream& operator<<(std::ostream &strm, const Location &l) {
  return strm << "(" << l.x_ << ", " << l.y_ << ")";
}

std::string Location::to_string() {
  return "(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
}
