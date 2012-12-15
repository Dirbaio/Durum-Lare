#include "map.h"

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

Map::Map(vector<vector<bool> > v)
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
                m[x][y] = Tile(4, rand()%4);
            }
            else //Calle!
            {
                bool up = v[x][y-1];
                bool down = v[x][y+1];
                bool left = v[x-1][y];
                bool right = v[x+1][y];

                int idx = left + down*2 + right*4 + up*8;
                m[x][y] = tiles[idx];
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
                    if(m[x-1][y].tileNum != 0)
                        m[x][y] = Tile(1, 2);
                    else if(m[x+1][y].tileNum != 0)
                        m[x][y] = Tile(1, 0);
                }
                else //Vertical
                {
                    if(m[x][y-1].tileNum != 0)
                        m[x][y] = Tile(1, 3);
                    else if(m[x][y+1].tileNum != 0)
                        m[x][y] = Tile(1, 1);
                }
            }
        }
}

const int TILESIZE = 64;

void Map::render()
{
    Sprite s;
    s.setTexture(tex);

    for(int x = 0; x < tx; x++)
        for(int y = 0; y < ty; y++)
        {
            int ttx = m[x][y].tileNum % 4;
            int tty = m[x][y].tileNum / 4;
            s.setTextureRect(IntRect(ttx*TILESIZE, tty*TILESIZE, TILESIZE, TILESIZE));
            s.setPosition(x*TILESIZE, y*TILESIZE);
            s.setRotation(m[x][y].rot*90);
            s.draw();
        }
}
