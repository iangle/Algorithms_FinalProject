#include "isolation.h"

//Constructor
Board::Board(Player* p1, Player* p2, int w, int h) {
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

Player * Board::get_opponent(Player * curr_player) {
	if (curr_player == active_player) {
		return this->active_player;
	}
	else if (curr_player == inactive_player) {
		return this->inactive_player;
	}
	else {
		std::cout << "'player' must be an object registered as a player in the current game" << std::endl;
		return nullptr;
	}
}

//Copy constructor
Board::Board(const Board &b){
	player1 = b.player1;
	player2 = b.player2;
	width = b.width;
	height = b.height;
	move_count = b.move_count;
	active_player = b.active_player;
	inactive_player = b.inactive_player;
	board_state = b.board_state;
}

Board Board::forecast_move(std::pair<int, int> move) {
	Board new_board = *this;
	new_board.apply_move(move);
	return new_board;
}

bool Board::move_is_legal(std::pair<int, int> move) {
	int indx = move.first + (move.second * height);
    return (0 <= move.first && move.first < height) && (0 <= move.second && move.second < width) &&
           (board_state[indx] == BLANK);
}

std::vector<std::pair<int, int>> Board::get_blank_spaces()
{
    std::vector<std::pair<int, int>> blank_spaces;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (board_state[i + j * height] == BLANK) {
                blank_spaces.push_back(std::make_pair(i, j));
            }
        }
    }
	return blank_spaces;
}

std::pair<int, int> Board::get_player_location(Player* p)
{
    int indx;

    if (p == player1) {
        if (board_state[board_state.size() - 1] == NOT_MOVED) {
            return std::make_pair(-1, -1);
        } else {
            indx = board_state[board_state.size() - 1];
        }
    } else if (p == player2) {
        if (board_state[board_state.size() - 2] == NOT_MOVED) {
            return std::make_pair(-1, -1);
        } else {
            indx = board_state[board_state.size() - 2];
        }
    } else {
        std::cout << "Invalid player in get_player_location..." << std::endl;
    }

    int w = indx / height;
    int h = indx % height;

    return std::make_pair(h, w);
}

std::vector<std::pair<int, int>> Board::get_legal_moves(Player* p  = nullptr)
{
    if (p == nullptr) {
        p = active_player;
    }
    return this->get_moves(this->get_player_location(p));
}

void Board::apply_move(std::pair<int, int> move)
{
    int indx = move.first + (move.second * height);
    int last_move_indx = int(active_player == player2) + 1;
    board_state[board_state.size() - last_move_indx] = indx;
    board_state[indx] = 1;
    board_state[board_state.size() - 3] = (1 - board_state[board_state.size() - 3]); //Flips between 0 and 1
    Player * temp = active_player;
    active_player = inactive_player;
    inactive_player = temp;
    move_count++;
}

bool Board::is_winner(Player* p) //Returns true if the player is the inactive player and the active player has no legal moves.
{
    if (p == inactive_player) {
        if ((this->get_legal_moves(active_player)).empty()) {
            return true;
        }
    }
	return false;
}

bool Board::is_loser(Player* p) //Returns true if p is the active player and if they have no legal moves
{
    if (p == active_player) {
        if ((this->get_legal_moves(active_player)).empty()) {
            return true;
        }
    }
	return false;
}

float Board::utility(Player* p)
{
	if ((this->get_legal_moves(active_player)).empty()) {
	    if (p = inactive_player) {
	        return INFINITY;
	    } else if (p = active_player) {
	        return -INFINITY;
	    }
	}
    return 0.0f;
}

std::vector<std::pair<int, int>> Board::get_moves(std::pair<int, int> location)
{
	if (location == NOT_MOVED_PAIR) {
		return this->get_blank_spaces();
	}
	int row = location.first;
	int col = location.second;

	std::vector<std::pair<int, int>> valid_directions;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (j == 0 && i == 0) { //Moving in place is invalid
				continue;
			}
			else {
				valid_directions.push_back(std::make_pair(i, j));
			}
		}
	}

	std::vector<std::pair<int, int>> valid_moves;

	for (int i = 0; i < valid_directions.size(); i++) {

		int d_row, d_col;

		d_row = valid_directions[i].first;
		d_col = valid_directions[i].second;

		std::pair<int, int> curr_move = std::make_pair(row + d_row, col + d_col);

		if (this->move_is_legal(curr_move)) {
			valid_moves.push_back(curr_move);
		}
	}

	return valid_moves;
}

void Board::print_board()
{
	std::cout << "BOARD STATE" << std::endl;
	std::cout << std::string("=", 20) << std::endl;
	std::cout << this->to_string() << std::endl;
	std::cout << std::string("=", 20) << std::endl;
}

std::string Board::to_string()
{
	int p1_loc = board_state[board_state.size() - 1];
	int p2_loc = board_state[board_state.size() - 2];

	std::stringstream ss;

	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			std::string temp;
			int indx = i + (j * (this->height));
			int curr_state = board_state[indx];
			if (curr_state == BLANK) {
				temp = '-';
			}
			else if (p1_loc == indx) {
				temp = '1';
			}
			else if (p2_loc == indx) {
				temp = '2';
			}
			else {
				temp = 'X';
			}

			ss << std::setw(col_width) << temp;
		}
		ss << '\n';
	}

	return ss.str();
}

std::pair<std::vector<std::pair<int, int>>, std::pair<Player*, std::string>> Board::play()
{
	std::vector<std::pair<int, int>> move_history;

	while (true) {

		std::vector<std::pair<int, int>> legal_player_moves = this->get_legal_moves();

		Board game_copy = *this;

		std::pair<int, int> * curr_move = this->active_player->get_move(game_copy);

		if (curr_move = nullptr) {
			curr_move = &NOT_MOVED_PAIR;
		}

		bool in_legal_moves = false;

		for (int i = 0; i < legal_player_moves.size(); i++) {
			if (legal_player_moves[i] == *curr_move) {
				in_legal_moves = true;
			}
		}

		if (!in_legal_moves) {
			if (legal_player_moves.size() > 0) {
				return std::make_pair(move_history, std::make_pair(this->inactive_player, "forfeit"));
			}
			return std::make_pair(move_history, std::make_pair(this->inactive_player, "illegal move"));
		}

		move_history.push_back(*curr_move);

		this->apply_move(*curr_move);
	}
}

