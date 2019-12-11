#include "isolationGame.h"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {

	Player* player1 = new MinmaxPlayer;
	Player* player2 = new GreedyPlayer;

	int p1wins, p2wins;

	p1wins = 0;
	p2wins = 0;

	srand(time(NULL));

	for (int i = 0; i < 5; i++) {

		Board game1(player1, player2);

		bool print = false;

		int x1 = rand() % 7;
		int y1 = rand() % 7;
		int x2 = rand() % 7;
		int y2 = rand() % 7;

		game1.apply_move(make_board_move(x1, y1));
		game1.apply_move(make_board_move(x2, y2));

		std::pair<std::vector<move>, std::pair<Player*, std::string>> result = game1.play(print);

		Player* winner = result.second.first;

		if (winner == player1) {
			p1wins++;
		}
		else if (winner == player2) {
			p2wins++;
		}
		else {
			std::cout << "No winner detected!" << std::endl;
		}

		//Play another game with the players swapping starting positions

		Board game2(player2, player1);

		game2.apply_move(make_board_move(x1, y1));
		game2.apply_move(make_board_move(x2, y2));

		result = game2.play(print);

		winner = result.second.first;

		if (winner == player2) {
			p2wins++;
		}
		else if (winner == player1) {
			p1wins++;
		}
		else {
			std::cout << "No winner detected!" << std::endl;
		}
	}

	std::cout << player1->to_string() << " won " << p1wins << " out of " << p1wins + p2wins << std::endl;
	std::cout << player2->to_string() << " won " << p2wins << " out of " << p1wins + p2wins << std::endl;

	std::cout << "Press any key to continue...";
	std::getchar();

    return 0;
}