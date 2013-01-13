#include <vector>
using std::vector;

#include "Holdem.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComPlayer.h"
#include "Deck.h"
#include "Card.h"
#include "GameStatus.h"

void Holdem::run() {
	//set parameters
	money budget;
	bool retry;
	do {
		retry = setParams(budget);
	} while (retry);

	constructPlayers(budget);

	roundNum = 1;
	startGame();
}

void Holdem::constructPlayers(const money budget) {
	for (player_num n=0; n<numOfPlayers; ++n) {
		if (n == humanPos) {
			HumanPlayer hp(budget, allowAllIn);
			players.push_back(hp);
		} else {
			ComPlayer cp(budget, allowAllIn);
			players.push_back(cp);
		}
	}
}

void Holdem::startGame() {
	bool playAgain;
	do {
		//TODO: check if human player broke
		preFlop();
		flop();
		turn();
		river();
		playAgain = showDown();
		if (playAgain) ++roundNum;
	} while (playAgain);
}