#include "isolationGame.h"

move make_board_move(int a, int b) {
    move m;
    m.pair = std::make_pair(a, b);
    return m;
}

float naive_score(Board curr_game, Player * p)
{
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}


	float own_moves = float((curr_game.get_legal_moves(p)).size());
	float opp_moves = float((curr_game.get_legal_moves(curr_game.get_opponent(p))).size());

	return own_moves - 3 * opp_moves;
}

float center_score(Board curr_game, Player* p)
{
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}

	int w, h;
	w = curr_game.get_width() / 2;
	h = curr_game.get_height() / 2;

	move location = curr_game.get_player_location(p);

	return float( (h - location.pair.first) * (h - location.pair.first) + (w - location.pair.second) * (w - location.pair.second));
}

float improved_score(Board curr_game, Player* p)
{
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}


	float own_moves = float((curr_game.get_legal_moves(p)).size());
	float opp_moves = float((curr_game.get_legal_moves(curr_game.get_opponent(p))).size());

	return own_moves - opp_moves;
}

float open_move_score(Board curr_game, Player* p)
{
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}
	return float(curr_game.get_legal_moves(p).size());

}

//Constructor
Board::Board(Player* p1, Player* p2, int w, int h) {
	width = w;
	height = h;
	move_count = 0;
	player1 = p1;
	player2 = p2;
	active_player = p1;
	inactive_player = p2;

	board_state = std::vector<int>(width * height + 3, BLANK);

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

Board Board::forecast_move(move m) {
	Board new_board = *this;
	new_board.apply_move(m);
	return new_board;
}

bool Board::move_is_legal(move m) {
	int index = m.pair.first + (m.pair.second * height);
    return (0 <= m.pair.first && m.pair.first < height) && (0 <= m.pair.second && m.pair.second < width) &&
           (board_state[index] == BLANK);
}

std::vector<move> Board::get_blank_spaces()
{
    std::vector<move> blank_spaces;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (board_state[i + j * height] == BLANK) {
                blank_spaces.push_back(make_board_move(i, j));
            }
        }
    }
	return blank_spaces;
}

move Board::get_player_location(Player* p)
{
    int index;

    if (p == player1) {
        if (board_state[board_state.size() - 1] == NOT_MOVED) {
            return make_board_move(-1, -1);
        } else {
            index = board_state[board_state.size() - 1];
        }
    } else if (p == player2) {
        if (board_state[board_state.size() - 2] == NOT_MOVED) {
            return make_board_move(-1, -1);
        } else {
            index = board_state[board_state.size() - 2];
        }
    } else {
        std::cout << "Invalid player in get_player_location..." << std::endl;
    }

    int w = index / height;
    int h = index % height;

    return make_board_move(h, w);
}

std::vector<move> Board::get_legal_moves(Player* p  = nullptr)
{
    if (p == nullptr) {
        p = active_player;
    }
    return this->get_moves(this->get_player_location(p));
}

void Board::apply_move(move m)
{
    int index = m.pair.first + (m.pair.second * height);
    int last_move_index = int(active_player == player2) + 1;
    board_state[board_state.size() - last_move_index] = index;
    board_state[index] = 1;
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
	    if (p == inactive_player) {
	        return INFINITY;
	    } else if (p == active_player) {
	        return -INFINITY;
	    }
	}
    return 0.0f;
}

std::vector<move> Board::get_moves(move location)
{
	if (location.pair == NOT_MOVED_PAIR.pair) {
		return this->get_blank_spaces();
	}
	int row = location.pair.first;
	int col = location.pair.second;

	std::vector<move> valid_directions;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (j == 0 && i == 0) { //Moving in place is invalid
				continue;
			}
			else {
				valid_directions.push_back(make_board_move(i, j));
			}
		}
	}

	std::vector<move> valid_moves;

	for (auto & valid_direction : valid_directions) {

		int d_row, d_col;

		d_row = valid_direction.pair.first;
		d_col = valid_direction.pair.second;

		move curr_move = make_board_move(row + d_row, col + d_col);

		if (this->move_is_legal(curr_move)) {
			valid_moves.push_back(curr_move);
		}
	}

	return valid_moves;
}

void Board::print_board()
{
	std::cout << "BOARD STATE - " << "player 1 = " << this->player1->to_string() << " || player 2 = " << this->player2->to_string() << std::endl;
	std::cout << "======================" << std::endl;
	std::cout << this->to_string() << std::endl;
	std::cout << "======================" << std::endl;
}

std::string Board::to_string()
{
	int p1_loc = board_state[board_state.size() - 1];
	int p2_loc = board_state[board_state.size() - 2];

	std::stringstream ss;

	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			std::string temp;
			int index = i + (j * (this->height));
			int curr_state = board_state[index];
			if (curr_state == BLANK) {
				temp = '-';
			}
			else if (p1_loc == index) {
				temp = '1';
			}
			else if (p2_loc == index) {
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

std::pair<std::vector<move>, std::pair<Player*, std::string>> Board::play(bool print = false)
{
	std::vector<move> move_history;

	while (true) {

		if (print) {
			this->print_board();
		}

		std::vector<move> legal_player_moves = this->get_legal_moves();

		Board game_copy = *this;

		move curr_move = this->active_player->get_move(game_copy);

        /*
		if (curr_move == nullptr) {
			curr_move = &NOT_MOVED_PAIR;
		}
        */

		bool in_legal_moves = false;

		for (const auto & legal_player_move : legal_player_moves) {
			if (legal_player_move.pair == curr_move.pair) {
				in_legal_moves = true;
			}
		}

		if (!in_legal_moves) {
			if (legal_player_moves.empty()) {
				return std::make_pair(move_history, std::make_pair(this->inactive_player, "forfeit"));
			}
			return std::make_pair(move_history, std::make_pair(this->inactive_player, "illegal move"));
		}

		move_history.push_back(curr_move);

		this->apply_move(curr_move);
	}
}

// This is where the players decide to move
//
//

move RandomPlayer::get_move(Board curr_game)
{

    std::vector<move> legal_moves = curr_game.get_legal_moves(this);

    if (legal_moves.empty()) {
        return make_board_move(-1, -1);
    }

    std::random_device random_device;
    std::mt19937 engine{ random_device() };
    std::uniform_int_distribution<int> dist(0, legal_moves.size() - 1);
    move chosen_move = legal_moves[dist(engine)];

    return chosen_move;

}

std::string RandomPlayer::to_string()
{
	return std::string("RandomPlayer");
}

std::string GreedyPlayer::to_string()
{
	return std::string("GreedyPlayer");
}

move GreedyPlayer::get_move(Board curr_game)
{
	std::vector<move> legal_moves = curr_game.get_legal_moves(this);

	if (legal_moves.empty()) {
		return make_board_move(-1, -1);
	}

	std::vector<float> scores;

	for (unsigned i = 0; i < legal_moves.size(); i++) {
		move m = legal_moves[i];
		scores.push_back(this->score(curr_game.forecast_move(m)));
	}

	float max = *std::max_element(scores.begin(), scores.end());

	int index = 0;

	for (unsigned i = 0; i < scores.size(); i++) {
		if (scores[i] == max) {
			index = i;
		}
	}

	return legal_moves[index];

}

move MinmaxPlayer::get_move(Board curr_game)
{

	return this->min_max(curr_game, this->depth);
}

move MinmaxPlayer::min_max(Board curr_game, int depth)
{
	float best_score = -INFINITY;
	move best_move;

	std::vector<move> legal_moves = curr_game.get_legal_moves();

	for (unsigned i = 0; i < legal_moves.size(); i++) {
		move m = legal_moves[i];
		float min_value = this->min_value(curr_game.forecast_move(m), depth - 1);
		if (min_value > best_score) {
			best_score = min_value;
			best_move = m;
		}
	}

	return best_move;
}

float MinmaxPlayer::min_value(Board curr_game, int depth)
{
	if (this->terminal_state(curr_game, depth)) {
		return this->score(curr_game);
	}

	float score = INFINITY;

	std::vector<move> legal_moves = curr_game.get_legal_moves();

	for (unsigned i = 0; i < legal_moves.size(); i++) {
		move m = legal_moves[i];

		float max_value = this->max_value(curr_game.forecast_move(m), depth - 1);

		score = std::min(score, max_value);
	}

	return score;
}

float MinmaxPlayer::max_value(Board curr_game, int depth)
{
	if (this->terminal_state(curr_game, depth)) {
		return this->score(curr_game);
	}

	float score = -INFINITY;

	std::vector<move> legal_moves = curr_game.get_legal_moves();

	for (unsigned i = 0; i < legal_moves.size(); i++) {
		move m = legal_moves[i];

		float min_value = this->min_value(curr_game.forecast_move(m), depth - 1);

		score = std::max(score, min_value);
	}

	return score;

}

bool MinmaxPlayer::terminal_state(Board curr_game, int depth)
{
	if (depth == 0) {
		return true;
	}
	else if (curr_game.get_legal_moves().empty()) {
		return true;
	}
	return false;
}


std::string MinmaxPlayer::to_string()
{
	return std::string("MinmaxPlayer using " + this->get_score_fn());
}

move AlphaBetaPlayer::get_move(Board curr_game)
{
	return this->alpha_beta(curr_game, this->depth, -INFINITY, INFINITY);
}

move AlphaBetaPlayer::alpha_beta(Board curr_game, int depth, float alpha = -INFINITY, float beta = INFINITY)
{
	float best_score = -INFINITY;

	move best_move;

	std::vector<move> legal_moves = curr_game.get_legal_moves();

	for (unsigned i = 0; i < legal_moves.size(); i++) {

		move m = legal_moves[i];

		float min_value = this->min_value(curr_game.forecast_move(m), depth - 1, alpha, beta);

		if (min_value > best_score) {
			best_score = min_value;
			best_move = m;
		}

		if (best_score >= beta) {
			break;
		}

		alpha = std::max(alpha, best_score);
	}

	return best_move;
}

float AlphaBetaPlayer::min_value(Board curr_game, int depth, float alpha = -INFINITY, float beta = INFINITY)
{
	if (this->terminal_state(curr_game, depth)) {
		return this->score(curr_game);
	}

	float score = INFINITY;

	std::vector<move> legal_moves = curr_game.get_legal_moves();

	for (unsigned i = 0; i < legal_moves.size(); i++) {

		move m = legal_moves[i];

		float max_value = this->max_value(curr_game.forecast_move(m), depth - 1, alpha, beta);

		score = std::min(score, max_value);

		if (score <= alpha) {
			break;
		}

		beta = std::min(beta, score);

	}
	return score;
}

float AlphaBetaPlayer::max_value(Board curr_game, int depth, float alpha, float beta)
{
	if (this->terminal_state(curr_game, depth)) {
		return this->score(curr_game);
	}

	float score = -INFINITY;

	std::vector<move> legal_moves = curr_game.get_legal_moves();

	for (unsigned i = 0; i < legal_moves.size(); i++) {

		move m = legal_moves[i];

		float min_value = this->min_value(curr_game.forecast_move(m), depth - 1, alpha, beta);

		score = std::max(score, min_value);

		if (score >= beta) {
			break;
		}

		alpha = std::max(alpha, score);

	}
	return score;
}

bool AlphaBetaPlayer::terminal_state(Board curr_game, int depth)
{
	if (depth == 0) {
		return true;
	}
	else if (curr_game.get_legal_moves().empty()) {
		return true;
	}
	return false;
}


std::string AlphaBetaPlayer::to_string()
{
	return std::string("AlphaBetaPlayer using " + this->get_score_fn() + " with depth " + std::to_string(depth));
}

float Player::score(Board curr_game)
{
	if (this->get_score_fn() == "naive_score") {
		return naive_score(curr_game, this);
	}
	else if (this->get_score_fn() == "center_score") {
		return center_score(curr_game, this);
	}
	else if (this->get_score_fn() == "improved_score") {
		return improved_score(curr_game, this);
	}
	else if (this->get_score_fn() == "open_move_score") {
		return open_move_score(curr_game, this);
	}
	else {
		std::cout << "No score function declared..." << std::endl;
		return -INFINITY;
	}
}
