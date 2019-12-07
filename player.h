#ifndef PLAYER_H_
#define PLAYER_H_

#include <utility>
#include <random>

class Board;

class Player {
public:
	virtual std::pair<int, int> * get_move(Board curr_game) = 0;
};

class RandomPlayer : public Player {
public:
	std::pair<int, int> * get_move(Board curr_game);
};

#endif