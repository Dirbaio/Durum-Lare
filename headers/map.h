#ifndef MAP_H
#define MAP_H

#include<vector>
#include<cstdlib>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

class Tile
{
public:
    int tileNum;
    int rot;
    sf::Sprite s;

    Tile() {}
    Tile(int a, int b)
    {
        tileNum = a;
        rot = b;
    }
    bool transitable() {
        return tileNum != 5;
    }
};

class Map
{
public:
    sf::Texture tex;
    vector<vector<Tile> > m;
    int tx, ty; //Tamaño x, y en tiles

    //Crea el mapa
    //Obtiene el vector de bools del generador de balaghi!
    Map(){}
    void load(vector<vector<bool> > v);

    void render();
};

#endif // MAP_H
