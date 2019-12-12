//
// Created by Cameron Rutherford on 8/12/2019.
//

#ifndef ALGORITHMS_FINAL_ISOLATION_GAME_H
#define ALGORITHMS_FINAL_ISOLATION_GAME_H

#include <iostream>
#include <ostream>
#include <utility>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>

struct move {
    std::pair<int, int>  pair;
};

move make_board_move(int a, int b);

class Board;

class Player {
private:
	std::string score_fn = "";
public:
    virtual move get_move(Board curr_game) = 0;
	virtual std::string to_string() = 0;
	float score(Board curr_game);
	void set_score_fn(std::string fn) { score_fn = fn; }
	std::string get_score_fn() { return score_fn; }
};

class RandomPlayer : public Player {
public:
    move get_move(Board curr_game) override;
	std::string to_string();
};

class GreedyPlayer : public Player {
public:
	move get_move(Board curr_game) override;
	std::string to_string();
};

class MinmaxPlayer : public Player {
private:
	int depth = 4;
public:
	move get_move(Board curr_game) override;
	move min_max(Board curr_game, int depth);
	float min_value(Board curr_game, int depth);
	float max_value(Board curr_game, int depth);
	bool terminal_state(Board curr_game, int depth);
	std::string to_string();
};

class AlphaBetaPlayer : public Player {
private:
	int depth = 6;
public:
	move get_move(Board curr_game) override;
	move alpha_beta(Board curr_game, int depth, float alpha, float beta);
	float min_value(Board curr_game, int depth, float alpha, float beta);
	float max_value(Board curr_game, int depth, float alpha, float beta);
	bool terminal_state(Board curr_game, int depth);
	std::string to_string();
};

class Board {
protected:
    int col_width = 5;

private:
    int BLANK = 0;
    int NOT_MOVED = -1;

    move NOT_MOVED_PAIR = make_board_move(-1, -1);

    int width;
    int height;

    int move_count;
    std::vector<int> board_state;

    Player * player1;
    Player * player2;

    Player * active_player;
    Player * inactive_player;

public:

	int get_width() { return width; }
	int get_height() { return height; }

    // Constructor
    Board(Player* p1, Player* p2, int w = 7, int h = 7);

    Player * get_active_player() { return this->active_player; }

    Player * get_inactive_player() { return this->inactive_player; }

    Player* get_opponent(Player* curr_player);

    //Copy constructor - not 100% sure if this works
    Board(const Board& b);

    Board forecast_move(move m);

    bool move_is_legal(move m);

    std::vector<move> get_blank_spaces();

    move get_player_location(Player* p);

    std::vector<move> get_legal_moves(Player * p);

    void apply_move(move m);

    bool is_winner(Player * p);

    bool is_loser(Player* p);

    float utility(Player * p);

    std::vector<move> get_moves(move location);

    void print_board();

    std::string to_string();

    std::pair<std::vector<move>, std::pair<Player*, std::string>> play(bool print);
};

float naive_score(Board curr_game, Player* p);

float center_score(Board curr_game, Player* p);

float improved_score(Board curr_game, Player* p);

float open_move_score(Board curr_game, Player* p);


#endif //ALGORITHMS_FINAL_ISOLATION_GAME_H
