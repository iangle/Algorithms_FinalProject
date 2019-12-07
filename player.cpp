#include "player.h"

std::pair<int, int>* RandomPlayer::get_move(Board curr_game)
{
	curr_game.print_board();

	std::vector<std::pair<int, int>> legal_moves = curr_game.get_legal_moves(this);

	if (legal_moves.size() == 0) {
		return nullptr;
	}

	std::random_device random_device;
	std::mt19937 engine{ random_device() };
	std::uniform_int_distribution<int> dist(0, legal_moves.size() - 1);
	std::pair<int, int> chosen_move = legal_moves[dist(engine)];


	//THIS MIGHT BE BROKEN
	//return &chosen_move;
	return &std::make_pair(chosen_move.first, chosen_move.second);

}
