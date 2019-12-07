#ifndef ISO_H_
#define ISO_H_

#include <vector>
#include "player.h"
#include <iostream>
#include <utility>
#include <string>
#include <sstream>
#include <iomanip>


class Board {
	protected:
		int col_width = 5;

	private:
		int BLANK = 0;
		int NOT_MOVED = -1;

		std::pair<int, int> NOT_MOVED_PAIR = std::make_pair(-1, -1);

		int width;
		int height;

		int move_count;
		std::vector<int> board_state;

		Player * player1;
		Player * player2;

		Player * active_player;
		Player * inactive_player;

	public:
		// Constructor
		Board(Player* p1, Player* p2, int w = 7, int h = 7);

		Player * get_active_player() { return this->active_player; }

		Player * get_inactive_player() { return this->inactive_player; }

		Player* get_opponent(Player* curr_player);

		//Copy constructor - not 100% sure if this works
		Board(const Board& b);

		Board forecast_move(std::pair<int, int> move);

		bool move_is_legal(std::pair<int, int> move);

		std::vector<std::pair<int, int>> get_blank_spaces();

		std::pair<int, int> get_player_location(Player* p);

		std::vector<std::pair<int, int>> get_legal_moves(Player * p);

		void apply_move(std::pair<int, int> move);

		bool is_winner(Player * p);

		bool is_loser(Player* p);

		float utility(Player * p);

		std::vector<std::pair<int, int>> get_moves(std::pair<int, int> location);

		void print_board();

		std::string to_string();

		std::pair<std::vector<std::pair<int, int>>, std::pair<Player*, std::string>> play();
};

#endif