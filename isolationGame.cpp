/*Isaac Angle, Cameron Rutherford
This is the implamentation part of the isolationGame header file*/

#include "isolationGame.h"

//creates a move object that corresponds to the x and y 
//position denoted as a and b
move make_board_move(int a, int b) {
    move m;
    m.pair = std::make_pair(a, b);
    return m;
}

//a scoring system used by the algorithm that values the 
//opponents possible moves above its own
float naive_score(Board curr_game, Player * p)
{
	//if player lost
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	//if player won
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}

	//holds the players moves
	float own_moves = float((curr_game.get_legal_moves(p)).size());

	//holds the opposing players moves
	float opp_moves = float((curr_game.get_legal_moves(curr_game.get_opponent(p))).size());

	//returns a float that values the opponents moves
	//three times as much as the players moves
	return own_moves - 3 * opp_moves;
}

//a scoring system that is used by the algorthim to
//value being closer to the middle of the board
float center_score(Board curr_game, Player* p)
{
	//if player lost
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	//if player won
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}
	
	//used to compare the distance from the center
	int w, h;
	w = curr_game.get_width() / 2;
	h = curr_game.get_height() / 2;

	//get the players location
	move location = curr_game.get_player_location(p);

	//returns the comptation of the difference
	return float( (h - location.pair.first) * (h - location.pair.first) + (w - location.pair.second) * (w - location.pair.second));
}

//a scoring system that is used by the algorthim to
//value the players moves the same as the opposing players moves
float improved_score(Board curr_game, Player* p)
{
	// player lost
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	//if player won
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}

	//gets the players legal moves
	float own_moves = float((curr_game.get_legal_moves(p)).size());

	//gets the opposing players moves
	float opp_moves = float((curr_game.get_legal_moves(curr_game.get_opponent(p))).size());

	//returns the difference between the players and opposing players moves
	return own_moves - opp_moves;
}

//a scoring system that is used by the algorthim to
//value the legal moves that the player can choose
float open_move_score(Board curr_game, Player* p)
{
	//if player lost
	if (curr_game.is_loser(p)) {
		return -INFINITY;
	}
	//if player won
	else if (curr_game.is_winner(p)) {
		return INFINITY;
	}

	//returns the number of possible moves
	return float(curr_game.get_legal_moves(p).size());

}

//Constructor
//takes in the two players and the width and height of the board
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

//returns an active player or an inactive player 
//depending on whether or not it is the players turn
//takes in the current player
Player * Board::get_opponent(Player * curr_player) {
	//if active
	if (curr_player == active_player) {
		return this->active_player;
	}
	//if inactive
	else if (curr_player == inactive_player) {
		return this->inactive_player;
	}
	//else something went wrong
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

//forecasts the next move of the player
//takes in the players move
Board Board::forecast_move(move m) {
	Board new_board = *this;
	new_board.apply_move(m);
	return new_board;
}

//checks if the move that is passed in is legal
//if it is the function is true otherwise it is false
bool Board::move_is_legal(move m) {
	int index = m.pair.first + (m.pair.second * height);
    return (0 <= m.pair.first && m.pair.first < height) && (0 <= m.pair.second && m.pair.second < width) &&
           (board_state[index] == BLANK);
}

//gets the black spaces on the board and returns them
std::vector<move> Board::get_blank_spaces()
{
    std::vector<move> blank_spaces;

	//adds the blank spaces to the board
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (board_state[i + j * height] == BLANK) {
                blank_spaces.push_back(make_board_move(i, j));
            }
        }
    }

	//returns a vector of moves that contains the blank spaces
	return blank_spaces;
}

//gets the player current location
//takes in the player whose location is being returned
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

//gets the legal moves that a given player can make
std::vector<move> Board::get_legal_moves(Player* p  = nullptr)
{
    if (p == nullptr) {
        p = active_player;
    }
    return this->get_moves(this->get_player_location(p));
}

//apllies a move to the board, takes in the move that 
//the player wants to make
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

//gets the possible moves from the given location
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

//this function uses the toString() function to print edges and names around the board
void Board::print_board()
{
	std::cout << "BOARD STATE - " << "player 1 = " << this->player1->to_string() << " || player 2 = " << this->player2->to_string() << std::endl;
	std::cout << "======================" << std::endl;
	std::cout << this->to_string() << std::endl;
	std::cout << "======================" << std::endl;
}


//this function prints out the board
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
			//if mov not available put a -
			if (curr_state == BLANK) {
				temp = '-';
			}
			//if player ones loaction put a 1
			else if (p1_loc == index) {
				temp = '1';
			}
			//if player two's locations put a 2
			else if (p2_loc == index) {
				temp = '2';
			}
			//otherwise put an x because a player has already been there
			else {
				temp = 'X';
			}

			ss << std::setw(col_width) << temp;
		}
		ss << '\n';
	}

	return ss.str();
}

//this function plays the game, the user passes in a boolean
//that tells the function whether or not to print out the board.
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

//gives a name to the random player for printing purposes
std::string RandomPlayer::to_string()
{
	return std::string("RandomPlayer");
}

//gives a name to the greedy player for printing purposes
std::string GreedyPlayer::to_string()
{
	return std::string("GreedyPlayer");
}

//gets the move for the greedy player
//takes in the board of the current game
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

//gets the moves for the min max player
//takes in the board to the current game
move MinmaxPlayer::get_move(Board curr_game)
{

	return this->min_max(curr_game, this->depth);
}

//creates the algorithm that the min max player uses to move around the board
move MinmaxPlayer::min_max(Board curr_game, int depth)
{
	float best_score = -INFINITY;
	move best_move;

	std::vector<move> legal_moves = curr_game.get_legal_moves();

	//get the best move
	for (unsigned i = 0; i < legal_moves.size(); i++) {
		move m = legal_moves[i];
		float min_value = this->min_value(curr_game.forecast_move(m), depth - 1);
		if (min_value > best_score) {
			best_score = min_value;
			best_move = m;
		}
	}

	//return best move
	return best_move;
}

//gets the minimum value for the min max player
//used to compute the algorithm
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

//gets the maximum value for the min max player
//used to compute the algorithm
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

//returns the score used by the min max player
std::string MinmaxPlayer::to_string()
{
	return std::string("MinmaxPlayer using " + this->get_score_fn());
}

//gets the move for the aplha beta player
move AlphaBetaPlayer::get_move(Board curr_game)
{
	return this->alpha_beta(curr_game, this->depth, -INFINITY, INFINITY);
}

//computes the algorithm that the apla beta player uses to move around the board
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

//gets the minimum value for the alpha beta player
//used to compute the algorithm
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

//gets the maximum value for the alpha beta player
//used to compute the algorithm
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


//returns the scoring system that the alpha beta player is using
std::string AlphaBetaPlayer::to_string()
{
	return std::string("AlphaBetaPlayer using " + this->get_score_fn() + " with depth " + std::to_string(depth));
}

//gets the players scores from the given current game
float Player::score(Board curr_game)
{
	//naive score
	if (this->get_score_fn() == "naive_score") {
		return naive_score(curr_game, this);
	}
	//center score
	else if (this->get_score_fn() == "center_score") {
		return center_score(curr_game, this);
	}
	//improved score
	else if (this->get_score_fn() == "improved_score") {
		return improved_score(curr_game, this);
	}
	//open move score
	else if (this->get_score_fn() == "open_move_score") {
		return open_move_score(curr_game, this);
	}
	//just in case none of those are true
	else {
		std::cout << "No score function declared..." << std::endl;
		return -INFINITY;
	}
}
