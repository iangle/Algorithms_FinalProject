#ifndef ISO_H_
#define ISO_H_

#include <vector>
#include "player.h"

using namespace std;

class Board {
	private:
		Player * player1;
		Player * player2;
		int width;
		int height;
		int BLANK = 0;
		int NOT_MOVED = -1; //????
		int move_count = 0;

	public:
		Board(Player * p1, Player * p2, int w = 7, int h = 7) {

		}


};

#endif