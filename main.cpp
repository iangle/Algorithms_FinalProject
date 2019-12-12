#include "isolationGame.h"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

std::vector<std::string> valid_score_functions;

/*
int NUM_MATCHES = 10;

int main() {

	bool print = false;

	srand(int(time(NULL)));

	valid_score_functions.push_back("naive_score");
	valid_score_functions.push_back("center_score");
	valid_score_functions.push_back("improved_score");
	valid_score_functions.push_back("open_move_score");

	std::vector<Player*> players;

	players.push_back(new GreedyPlayer);
	players[players.size() - 1]->set_score_fn("open_move_score");

	players.push_back(new RandomPlayer);

	
	for (unsigned i = 0; i < valid_score_functions.size(); i++) {
		players.push_back(new AlphaBetaPlayer);
		players[players.size() - 1]->set_score_fn(valid_score_functions[i]);
	}
	

	for (unsigned i = 0; i < players.size(); i++) {

		Player* competitor = players[i];

		int competitor_wins = 0;

		std::vector<Player*> opponents;

		for (unsigned j = 0; j < players.size(); j++) {
			if (i == j) {
				continue;
			}
			else {
				opponents.push_back(players[j]);
			}
		}

		for (unsigned j = 0; j < opponents.size(); j++) {

			for (unsigned k = 0; k < unsigned(NUM_MATCHES); k++) {

				Player* opponent = opponents[j];

				Board game1(competitor, opponent);

				Board game2(opponent, competitor);

				int x1 = rand() % 7;
				int y1 = rand() % 7;
				int x2 = rand() % 7;
				int y2 = rand() % 7;

				game1.apply_move(make_board_move(x1, y1));
				game1.apply_move(make_board_move(x2, y2));

				game2.apply_move(make_board_move(x1, y1));
				game2.apply_move(make_board_move(x2, y2));

				std::pair<std::vector<move>, std::pair<Player*, std::string>> result1 = game1.play(print);
				std::pair<std::vector<move>, std::pair<Player*, std::string>> result2 = game2.play(print);

				Player* winner1 = result1.second.first;
				Player* winner2 = result2.second.first;

				if (winner1 == competitor) {
					competitor_wins++;
				}

				if (winner2 == competitor) {
					competitor_wins++;
				}
			}

		}

		std::cout << competitor->to_string() << " won " << competitor_wins << " out of " << NUM_MATCHES * opponents.size() * 2 << " = " << (float(competitor_wins) / float(NUM_MATCHES * opponents.size() * 2)) * 100 << "%\n";

	}

	std::cout << "Press any key to continue...";
	std::getchar();

	return 0;
}
*/


int main() {

	valid_score_functions.push_back("naive_score");
	valid_score_functions.push_back("center_score");
	valid_score_functions.push_back("improved_score");
	valid_score_functions.push_back("open_move_score");

	Player* player1 = new AlphaBetaPlayer;
	Player* player2 = new RandomPlayer;

	player1->set_score_fn(valid_score_functions[1]);
	//player2->set_score_fn(valid_score_functions[3]);

	int p1wins, p2wins;

	p1wins = 0;
	p2wins = 0;

	srand(int(time(NULL)));

	for (int i = 0; i < 25; i++) {

		Board game1(player1, player2);

		bool print = true;

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