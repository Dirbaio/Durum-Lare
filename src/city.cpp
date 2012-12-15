#include "city.h"
#include "utils.h"
#include "generator.h"

#include <cmath>

bool City::init(int w, int h, int _tw, int _th) {

	tw = _tw;
	th = _th;

	vector<vector<bool> > v = generateMap();
	map.load(v);
	return true;
}

void City::render() {
	map.render();
}
void City::renderTop() {
	map.renderTop();
}

int City::getW() {return map.m[0].size();}
int City::getH() {return map.m.size();}
int City::getTW() {return tw;}
int City::getTH() {return th;}
bool City::occupedXY(int x, int y) {
	//std::cerr << "XY: " << x << " " << y << std::endl;
	return occupedIJ(x/tw, y/th);
}

bool City::occupedIJ(int i, int j) {
	//std::cerr << "IJ: " << i << " " << j << " - " << !map.m[i][j].transitable() << std::endl;
	if (i < 0 || i >= map.tx ) return true;
	if (j < 0 || j >= map.ty ) return true;
	return !map.m[i][j].transitable();
}

bool City::occupedRect(sf::IntRect rect) {
	return occupedXY(rect.left           , rect.top) ||
			occupedXY(rect.left+rect.width, rect.top) ||
			occupedXY(rect.left+rect.width, rect.top+rect.height) ||
			occupedXY(rect.left           , rect.top+rect.height);
}

sf::Vector2f City::getRandomStreet() {
			while(1) {
		int x = rand()%(map.m[0].size()*32);
		int y = rand()%(map.m.size()*32);
		int w=32, h=32;
		sf::IntRect rect(x, y, w, h);
		//std::cerr << x << " " << y << " " << w << " " << h << std::endl;
		if (!occupedRect(rect))
			return sf::Vector2f(x+w/2., y+h/2.);
	}
}

sf::Vector2i City::absoluteToTilePos(const sf::Vector2f &pos)
{
	return sf::Vector2i((int) std::floor(pos.x/tw), (int) std::floor(pos.y/th));
}

int City::absoluteToTilePosX(float x)
{
	return (int) std::floor(x/tw);
}

int City::absoluteToTilePosY(float y)
{
	return (int) std::floor(y/th);
}

float City::tileTopPos(int y)
{
	return float(y+1)*th;
}

float City::tileBottomPos(int y)
{
	return float(y)*th;
}

float City::tileLeftPos(int x)
{
	return float(x)*tw;
}

float City::tileRightPos(int x)
{
	return float(x+1)*tw;
}
