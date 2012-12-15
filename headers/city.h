#ifndef CITY_H
#define CITY_H

#include <vector>
#include <SFML/Graphics.hpp>

typedef std::vector<bool> VB;
typedef std::vector<VB> Map;

class City {
  public:
    City(int w, int h, int tw, int th);
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
