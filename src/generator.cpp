#include "generator.h"

struct Calle {
	int y1, y2;
	int x1, x2;
	Calle(int x1, int x2, int y1, int y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}
	Calle(){}
};

vector<vector<bool> > generateMap()
{
    srand(time(NULL));
	int tx, ty;
	tx = rand()%30 + 30;
	ty = tx + rand()%11 - 5;
    //tx-=(tx%4);
    //ty-=(ty%4);
	vector<vector<bool> > v (ty+2, vector<bool> (tx+2, true));
	int ctv = 1;
	int cth = 1;
    int hsize = tx/3 + rand()%9 - 4;
    int vsize = ty/3 + rand()%9 - 4;
	int midx = tx/2;
    midx-=(midx%4);
	int midy = ty/2;
    midy-=(midy%4);
	vector<Calle> callesV (1, Calle(midx, midx, ty/4 + rand()%4, (ty*3)/4 - rand()%4)), callesH (1, Calle(tx/4 + rand()%4, (tx*3)/4 - rand()%4, midy, midy));
	
	while (cth <= hsize && ctv <= vsize) {
		Calle ant;
        int l1, l2, p, rnd, d, x, y;

		ant = callesV[ctv-1];						// HORIZONTALES
		l1 = ant.y1;
		l2 = ant.y2;
		p = ant.x1;
		rnd = rand()%(l2-l1) + l1;
        rnd-=(rnd%4);
		x = y = p;
        d = rand()%9 + 9;
        for (int i = 0; i < d; i++) {
            int pos = x-1;
            if (pos < 0) break;
            x = pos;
        }
        d = rand()%9 + 9;
        for (int i = 0; i < d; i++) {
            int pos = y+1;
            if (pos >= tx) break;
            y = pos;
        }
		callesH.push_back(Calle(x, y, rnd, rnd));
		
		ant = callesH[cth-1];						// VERTICALES
		l1 = ant.x1;
		l2 = ant.x2;
		p = ant.y1;
		rnd = rand()%(l2-l1) + l1;
        rnd-=(rnd%4);
		x = y = p;
        d = rand()%9 + 9;
        for (int i = 0; i < d; i++) {
            int pos = x-1;
            if (pos < 0) break;
            x = pos;
        }
        d = rand()%9 + 9;
        for (int i = 0; i < d; i++) {
            int pos = y+1;
            if (pos >= ty) break;
            y = pos;
        }
		callesV.push_back(Calle(rnd, rnd, x, y));
		
		ctv++;
		cth++;
	}
	
	for (int i = 0; i < ctv; i++) {
		int p = callesV[i].x1, l1 = callesV[i].y1, l2 = callesV[i].y2;
		for (int j = l1; j <= l2; j++) v[j+1][p+1] = false;
	}
	for (int i = 0; i < cth; i++) {
		int p = callesH[i].y1, l1 = callesH[i].x1, l2 = callesH[i].x2;
		for (int j = l1; j <= l2; j++) v[p+1][j+1] = false;
	}
	
    return v;
}
