#include "isolation.h"

//Constructor
Board::Board(Player* p1, Player* p2, int w = 7, int h = 7) {
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
	return std::vector<std::pair<int, int>>();
}

void Board::print_board()
{
}

void Board::play()
{
}

