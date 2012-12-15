#ifndef MAP_H
#define MAP_H

#include<vector>
#include<cstdlib>

using namespace std;

class Tile
{
public:
    int tileNum;
    int rot;
    Tile(int a, int b)
    {
        tileNum = a;
        rot = b;
    }
};

class Map
{
public:
    vector<vector<Tile> > m;
    int tx, ty; //Tamaño x, y en tiles

    //Crea el mapa
    //Obtiene el vector de bools del generador de balaghi!
    Map(vector<vector<bool> > v);

    void render();
};

#endif // MAP_H
