#include "isolationGame.h"
#include <iostream>

int main() {
	Player* player1 = new RandomPlayer;
	Player* player2 = new RandomPlayer;

	Board game(player1, player2);

	game.play();


    return 0;
}