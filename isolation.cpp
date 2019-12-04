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
		return NULL;
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
	if ((0 <= move.first && move.first < height) && (0 <= move.second && move.second < width) && (board_state[indx] == BLANK)) {
		return true;
	}
	else {
		return false;
	}
}

std::vector<std::pair<int, int>> Board::get_blank_spaces()
{
	return std::vector<std::pair<int, int>>();
}

std::pair<int, int> Board::get_player_location(Player* p)
{
	return std::pair<int, int>();
}

std::vector<std::pair<int, int>> Board::get_legal_moves(Player* p)
{
	return std::vector<std::pair<int, int>>();
}

void Board::apply_move(std::pair<int, int> move)
{
}

bool Board::is_winner(Player* p)
{
	return false;
}

bool Board::is_loser(Player* p)
{
	return false;
}

float Board::utility(Player* p)
{
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

