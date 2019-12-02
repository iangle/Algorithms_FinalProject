#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
private:
public:
	virtual std::pair <int, int> get_move() = 0;
};

#endif