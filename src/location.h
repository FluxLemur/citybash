#ifndef LOCATION_H
#define LOCATION_H

class Location {
  private:
    int x_;
    int y_;
    friend std::ostream& operator<<(std::ostream&, const Location&);

  public:
    Location(int x, int y);
    static Location* random_location(int width);
    static float distance_between(Location l1, Location l2);
};

#endif // LOCATION_H
