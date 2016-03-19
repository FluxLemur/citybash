#ifndef LOCATION_H
#define LOCATION_H

#include <string>

class Location {
  private:
    int x_;
    int y_;
    friend std::ostream& operator<<(std::ostream&, const Location&);

  public:
    static Location* random_location(int width);
    static float distance_between(Location l1, Location l2);

    Location(int x, int y);
    std::string to_string();
};

#endif // LOCATION_H
