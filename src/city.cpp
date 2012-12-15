#include "city.h"
#include "utils.h"
#include "generator.h"

bool City::init(int w, int h, int _tw, int _th) {

    tw = _tw;
    th = _th;

    vector<vector<bool> > v = generateMap();
    map.load(v);


    sf::Vector2f coso = getRandomStreet();
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
    return !map.m[i][j].transitable();
}

bool City::occupedRect(sf::IntRect rect) {
    return occupedXY(rect.left           , rect.top) ||
           occupedXY(rect.left+rect.width, rect.top) ||
           occupedXY(rect.left+rect.width, rect.top+rect.height) ||
           occupedXY(rect.left           , rect.top+rect.height);
}

sf::Vector2f City::getRandomStreet() {

    int sizeX = map.tx;
    int sizeY = map.ty;

    sf::Vector2i pos(Utils::randomInt(0, sizeX-1), Utils::randomInt(0, sizeY-1));

    while (map.boolMatrix[pos.x][pos.y]) {
	pos.x = Utils::randomInt(0, sizeX-1);
	pos.y = Utils::randomInt(0, sizeY-1);
    }

    int dX = Utils::randomInt(-12, 28);
    int dY = Utils::randomInt(-12, 28);
    return sf::Vector2f(pos.x*64 + dX, pos.y*64 + dY);



/*
    while(1) {
	int y = rand()%(map.m[0].size()*64);
	int x = rand()%(map.m.size()*64);
	int w=32, h=32;
	sf::IntRect rect(x, y, w, h);
	std::cerr << x << " " << y << " " << w << " " << h << std::endl;
	if (!occupedRect(rect))
	    return sf::Vector2f(x+w/2., (y+h/2.));
    }
    */
}
