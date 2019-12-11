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
public:
    virtual move get_move(Board curr_game) = 0;
	virtual std::string to_string() = 0;
};

class RandomPlayer : public Player {
public:
    move get_move(Board curr_game) override;
	std::string to_string();
};


class GreedyPlayer : public Player {
public:
	move get_move(Board curr_game) override;
	float score(Board curr_game);
	std::string to_string();
};

class MinmaxPlayer : public Player {
private:
	int depth = 5;
public:
	move get_move(Board curr_game) override;
	move min_max(Board curr_game, int depth);
	float min_value(Board curr_game, int depth);
	float max_value(Board curr_game, int depth);
	bool terminal_state(Board curr_game, int depth);
	float score(Board curr_game);
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


#endif //ALGORITHMS_FINAL_ISOLATION_GAME_H
