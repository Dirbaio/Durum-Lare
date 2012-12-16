#include "map.h"
#include "graphics_engine.h"
#include "defines.h"


#include <iostream>

static Tile tiles[] = {
    //                 URDL
    Tile(0, 0),     // 0000 Should never happen?
    Tile(6, 0),     // 0001
    Tile(6, 3),     // 0010
    Tile(7, 3),     // 0011
    Tile(6, 2),     // 0100
    Tile(0, 0),     // 0101
    Tile(7, 2),     // 0110
    Tile(2, 2),     // 0111
    Tile(6, 1),     // 1000
    Tile(7, 0),     // 1001
    Tile(0, 1),     // 1010
    Tile(2, 3),     // 1011
    Tile(7, 1),     // 1100
    Tile(2, 0),     // 1101
    Tile(2, 1),     // 1110
    Tile(3, 0),     // 1111
};

const int TILESIZE = 64;

void Map::load(vector<vector<bool> > v)
{


    tex.loadFromFile("img/tiles.png");

    tx = v.size();
    ty = v[0].size();

    m = vector<vector<Tile> > (tx, vector<Tile>(ty));

    //Asumo que hay 1 margen de todo casas.
    //SINO, el programa haria PUM! (eso es malo)
    for(int x = 0; x < tx; x++)
        for(int y = 0; y < ty; y++)
        {
            if(v[x][y]) //Casa!
            {
                m[x][y] = Tile(4, 0);
            }
            else //Calle!
            {
                bool up = !v[x][y-1];
                bool down = !v[x][y+1];
                bool left = !v[x-1][y];
                bool right = !v[x+1][y];

                int idx = left + down*2 + right*4 + up*8;
                m[x][y] = tiles[idx];
                if(m[x][y].tileNum == 0 && rand()%30 == 0)
                {
                    m[x][y].tileNum = 1;
                    if(rand()%2 == 0)
                        m[x][y].rot += 2;
                }
            }
        }

    //Poner pasocebras! Toguays.
    for(int x = 0; x < tx; x++)
        for(int y = 0; y < ty; y++)
        {
            if(m[x][y].tileNum == 0)
            {
                if(m[x][y].rot == 0) //Horizontal
                {
                    if(m[x-1][y].tileNum >= 2)
                        m[x][y] = Tile(1, 2);
                    else if(m[x+1][y].tileNum >= 2)
                        m[x][y] = Tile(1, 0);
                }
                else //Vertical
                {
                    if(m[x][y-1].tileNum >= 2)
                        m[x][y] = Tile(1, 3);
                    else if(m[x][y+1].tileNum >= 2)
                        m[x][y] = Tile(1, 1);
                }
            }
        }

    //Poner casas!!11!!!11!
    for(int x = 0; x < tx; x++)
        for(int y = 0; y < ty; y++)
        {
            Tile& t = m[x][y];
            if(t.tileNum == 4)
            {
                bool border = false;
                if(x == 0 || x == tx-1) border = true;
                if(y == 0 || y == ty-1) border = true;

                if(y != ty-1 && m[x][y+1].tileNum == 4 && rand()%6 == 0)
                {
                    t.tileNum = 12;
                    m[x][y+1].tileNum = 16;
                }
                else if(x != tx-1 && m[x+1][y].tileNum == 4 && rand()%4 == 0)
                {
                    t.tileNum = 9;
                    m[x+1][y].tileNum = 10;
                }
                else if(rand()%3 == 0 || border)
                    t.tileNum = 8;
                else if(rand()%3 == 0)
                    t.tileNum = 5;
            }
        }

    for(int x = 0; x < tx; x++)
        for(int y = 0; y < ty; y++)
        {
            Tile& t = m[x][y];
            t.s.setTexture(tex);
            int ttx = t.tileNum % 4;
            int tty = t.tileNum / 4;
            t.s.setTextureRect(sf::IntRect(ttx*TILESIZE, tty*TILESIZE*3/2+TILESIZE/2, TILESIZE, TILESIZE));
            t.s.setOrigin(TILESIZE/2, TILESIZE/2);
            t.s.setRotation(t.rot*90);
            t.s.setPosition((x+1)*TILESIZE - TILESIZE/2, (y+1)*TILESIZE - TILESIZE/2);

            t.s2.setTexture(tex);
            t.s2.setTextureRect(sf::IntRect(ttx*TILESIZE, tty*TILESIZE*3/2, TILESIZE, TILESIZE/2));
            t.s2.setOrigin(TILESIZE/2, TILESIZE/2);
            t.s2.setRotation(t.rot*90);
            t.s2.setPosition((x+1)*TILESIZE - TILESIZE/2, (y+1)*TILESIZE - TILESIZE);
        }

    boolMatrix = vector<vector<bool> > (tx, vector<bool> (ty, true));

    queue<pair<int, int> > q;

    //Calcular insolidificaciciones...
    //Con un befeese.
    for(int x = 0; x < tx; x++)
        for(int y = 0; y < ty; y++)
            if(!v[x][y]) q.push(pair<int, int> (x, y));

    while(!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        if(x < 0 || x >= tx) continue;
        if(y < 0 || y >= ty) continue;
        if(m[x][y].tileNum >= 8) continue;
        if(!boolMatrix[x][y]) continue;
        boolMatrix[x][y] = false;
        q.push(pair<int, int>(x+1, y));
        q.push(pair<int, int>(x-1, y));
        q.push(pair<int, int>(x, y+1));
        q.push(pair<int, int>(x, y-1));
    }
}


void Map::render()
{
    GraphEng* graphics = GraphEng::getInstance();

    for(int x = 0; x < tx; x++)
	for(int y = 0; y < ty; y++)
            //graphics->Draw(&m[x][y].s);
            App->draw(m[x][y].s);
}

void Map::renderTop()
{
    GraphEng* graphics = GraphEng::getInstance();

    for(int x = 0; x < tx; x++)
        for(int y = 0; y < ty; y++)
            if(m[x][y].tileNum >= 8)
                //graphics->Draw(&m[x][y].s2);
                App->draw(m[x][y].s2);

}
