#include "city.h"

#include "generator.h"

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

int City::getW() {return map.m[0].size();}
int City::getH() {return map.m.size();}
int City::getTW() {return tw;}
int City::getTH() {return th;}
bool City::occupedXY(int x, int y) {
    return occupedIJ(y/th, x/tw);
}

bool City::occupedIJ(int i, int j) {
    return map.m[i][j].transitable();
}

bool City::occupedRect(sf::IntRect rect) {
    return occupedXY(rect.left           , rect.top) ||
           occupedXY(rect.left+rect.width, rect.top) ||
           occupedXY(rect.left+rect.width, rect.top+rect.height) ||
           occupedXY(rect.left           , rect.top+rect.height);
}


