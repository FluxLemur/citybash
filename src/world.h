/*
 * A World represents 3D space that has a cities at various locations.
 * It consists of a 2D rectangle, with edges that wrap around, forming
 * a torus.
 */

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "city.h"

class World {
  private:
    int width;
    std::vector<City*> cities;

  public:
    World(int width);
};

#endif // WORLD_H
