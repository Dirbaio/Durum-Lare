#include "city.h"
#include "utils.h"
#include "generator.h"

#include <cmath>

bool City::init(int w, int h) {
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
bool City::occupedXY(int x, int y) {
	//std::cerr << "XY: " << x << " " << y << std::endl;
    return occupedIJ(x/TILESIZE, y/TILESIZE);
}

bool City::validTile(int i, int j) {
	if (i < 0 || i >= map.tx ) return false;
	if (j < 0 || j >= map.ty ) return false;
	return true;
}
bool City::occupedIJ(int i, int j) {
	if(!validTile(i, j)) return true;
	return map.boolMatrix[i][j];
}

bool City::occupedRect(sf::IntRect rect) {
	return occupedXY(rect.left           , rect.top) ||
			occupedXY(rect.left+rect.width, rect.top) ||
			occupedXY(rect.left+rect.width, rect.top+rect.height) ||
			occupedXY(rect.left           , rect.top+rect.height);
}

sf::Vector2f City::getRandomStreet() {
    while(1) {
        int x = Utils::randomInt()%(map.m[0].size()*64);
        int y = Utils::randomInt()%(map.m.size()*64);
        int w=16, h=16;
        sf::IntRect rect(x, y, w, h);
        if (!occupedRect(rect)) return sf::Vector2f(x+w/2., y+h/2.);
    }
}

//!!! Esta funcion deberia devolver un tile de esplanada !!!1!!!1
//!!! Ahora no lo hace bien !!!!!!!!!!!!
sf::Vector2f City::getRandomClearing() {
    while(1) {
        int x = Utils::randomInt()%(map.m[0].size()*64);
        int y = Utils::randomInt()%(map.m.size()*64);
        int w=64, h=64;
        sf::IntRect rect(x, y, w, h);
        if (!occupedRect(rect)) return sf::Vector2f(x+w/2., y+h/2.);
    }
}

sf::Vector2i City::absoluteToTilePos(const sf::Vector2f &pos)
{
    return sf::Vector2i((int) std::floor(pos.x/TILESIZE), (int) std::floor(pos.y/TILESIZE));
}

int City::absoluteToTilePosX(float x)
{
    return (int) std::floor(x/TILESIZE);
}

int City::absoluteToTilePosY(float y)
{
    return (int) std::floor(y/TILESIZE);
}


float City::tileTopPos(int y)
{
    return float(y+1)*TILESIZE;
}

float City::tileBottomPos(int y)
{
    return float(y)*TILESIZE;
}

float City::tileLeftPos(int x)
{
    return float(x)*TILESIZE;
}

float City::tileRightPos(int x)
{
    return float(x+1)*TILESIZE;
}

int City::getTileAt(sf::Vector2f pt)
{
    int i = int(pt.x/TILESIZE);
    int j = int(pt.y/TILESIZE);
	if(!validTile(i, j)) return true;
	return map.m[i][j].tileNum;
}

//Dice si una persona en FROM ve a una persona en TO
bool City::visible(sf::Vector2f from, sf::Vector2f to)
{
    bool fromGrass = getTileAt(from) == 11;
    bool toGrass = getTileAt(to) == 11;
    
    //Desde fuera no se puede ver dentro..
	if(!fromGrass && toGrass) return false;
	float d = Utils::distance(from, to);
	if(fromGrass && toGrass && d > 36) return false;
    if(d > 180) return false;

	
    for(int i = 0; i <= 20; i++)
    {
        sf::Vector2f pt = (from*float(i) + to*float(20-i))/20.0f;
        //Si los dos dentro, ha de ser todo grass.
        if(fromGrass && getTileAt(pt) != 11)
        	return false;
        if(occupedXY(pt.x, pt.y))
            return false;
    }

    return true;
}
