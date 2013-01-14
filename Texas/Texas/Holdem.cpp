#include <vector>
using std::vector;

#include "Holdem.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComPlayer.h"
#include "Deck.h"
#include "Card.h"
#include "PokerHand.h"

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
			HumanPlayer hp(budget, allowAllIn, "You");
			players.push_back(hp);
		} else {
			ComPlayer cp(budget, allowAllIn, "Com#"+n);
			players.push_back(cp);
		}
	}

	assert(players.size() == numOfPlayers);
}

void Holdem::startGame() {
	bool playAgain;
	do {
		//TODO: check if human player broke
		//TODO: check for early end after each stage (1 player remain)
		preFlop();
		flop();
		turn();
		river();
		playAgain = showDown();
		if (playAgain) {
			checkBroke();
			cleanUp();
		}
	} while (playAgain);
}

vector<Player*> Holdem::highestHands(vector<Player*> &competitors) {
	assert(competitors.size() > 0);
	
	//pick the largest hand
	PokerHand highestHand = competitors.at(0)->getHand();
	PokerHand tmpHand;
	for (player_num num = 1; num != competitors.size(); ++num) {
		tmpHand = competitors.at(num)->getHand();
		if (highestHand < tmpHand) {
			highestHand = tmpHand;
		}
	}

	//put all players with the highest hand in a group
	vector<Player*> highests;
	for (player_num num = 0; num != competitors.size(); ++num) {
		if (highestHand == competitors.at(num)->getHand()) {
			highests.push_back(competitors.at(num));
		}
	}

	return highests;
}

void Holdem::cleanUp() {
	deck.reset();
	community.clear();
	pot = 0;
	dealer = (dealer+1)%numOfPlayers;
	++roundNum;

	for (vector<Player>::iterator iter = players.begin(); iter != players.end(); ++iter) {
		iter->nextRound();
	}
}