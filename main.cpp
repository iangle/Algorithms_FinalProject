#include "isolationGame.h"
#include <iostream>
#include <cstdio>

int main() {
	Player* player1 = new RandomPlayer;
	Player* player2 = new RandomPlayer;

	Board game(player1, player2);

	bool print = true;

	game.apply_move(make_board_move(0, 2));
	game.apply_move(make_board_move(6, 3));

	game.play(print);

	std::cout << "Press any key to continue...";
	std::getchar();

    return 0;
}