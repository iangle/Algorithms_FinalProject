#ifndef ISO_H_
#define ISO_H_

#include <vector>
#include "player.h"
#include <iostream>
#include <utility>

class Board {
	private:
		int BLANK = 0;
		int NOT_MOVED = -1;

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
		Board(Player * p1, Player * p2, int w = 7, int h = 7) {
			width = w;
			height = h;
			move_count = 0;
			player1 = p1;
			player2 = p2;
			active_player = p1;
			inactive_player = p2;

			board_state = std::vector<int>(BLANK, width * height + 3);

			board_state[board_state.size() - 1] = NOT_MOVED;
			board_state[board_state.size() - 2] = NOT_MOVED;
		}

		Player * get_active_player() {
			return this->active_player;
		}

		Player * get_inactive_player() {
			return this->inactive_player;
		}

		Player * get_opponent(Player * curr_player) {
			if (curr_player == active_player) {
				return this->active_player;
			} else if (curr_player == inactive_player) {
				return this->inactive_player;
			} else {
				std::cout << "'player' must be an object registered as a player in the current game" << std::endl;
				return NULL;
			}
		}

		//Copy constructor - not 100% sure if this works
		Board(const Board &b) {
			player1 = b.player1;
			player2 = b.player2;
			width = b.width;
			height = b.height;
			move_count = b.move_count;
			active_player = b.active_player;
			inactive_player = b.inactive_player;
			board_state = b.board_state;
		}

		Board forecast_move(std::pair<int, int> move) {

		}
};

#endif