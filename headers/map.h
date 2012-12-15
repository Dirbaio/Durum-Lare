#ifndef MAP_H
#define MAP_H

#include<vector>
#include<iostream>
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
    sf::Sprite s2;

    Tile() {}
    Tile(int a, int b)
    {
        tileNum = a;
        rot = b;
    }

};

class Map
{
public:
    sf::Texture tex;
    vector<vector<Tile> > m;

    vector< vector<bool> > boolMatrix;

    int tx, ty; //Tamaño x, y en tiles

    //Crea el mapa
    //Obtiene el vector de bools del generador de balaghi!
    Map(){}
    void load(vector<vector<bool> > v);

    void render();
    void renderTop();
};

#endif // MAP_H
