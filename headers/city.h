#ifndef CITY_H
#define CITY_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "map.h"

class City {
  public:
    City() {}
    void init(int w, int h, int tw, int th);
    void render();
    int getW();
    int getH();
    int getTW();
    int getTH();
    bool occupedXY(int x, int y);
    bool occupedIJ(int i, int j);
    bool occupedRect(sf::IntRect rect);
  private:
    int tw, th;
    Map map;

};

#endif // CITY_H