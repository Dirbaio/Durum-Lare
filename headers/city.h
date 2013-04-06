#ifndef CITY_H
#define CITY_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "map.h"

#define TILESIZE 64
class City {
public:
	City() {}

    bool init(int w, int h);
	void render();
	void renderTop();

    int getW();
    int getH();
	bool validTile(int i, int j);
	bool occupedXY(int x, int y);
	bool occupedIJ(int i, int j);
	bool occupedRect(sf::IntRect rect);
	sf::Vector2f getRandomStreet();
        sf::Vector2f getRandomClearing();

	sf::Vector2i absoluteToTilePos(const sf::Vector2f &pos);
	int absoluteToTilePosX(float x);
	int absoluteToTilePosY(float y);

	float tileTopPos(int y);
	float tileBottomPos(int y);
	float tileLeftPos(int x);
	float tileRightPos(int x);
	int getTileAt(sf::Vector2f pt);
    bool visible(sf::Vector2f from, sf::Vector2f to);
private:
	Map map;
};

#endif // CITY_H
