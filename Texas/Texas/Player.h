#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"

typedef unsigned money;

class Player {
public:
	Player(const money budget);
protected:
	std::vector<Card> pocket;
	money wallet;
};

#endif