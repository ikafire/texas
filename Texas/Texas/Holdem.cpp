#include <vector>
using std::vector;

#include <sstream>
using std::stringstream;

#include <algorithm>
#include "Holdem.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComPlayer.h"
#include "Deck.h"
#include "Card.h"
#include "PokerHand.h"

Holdem::Holdem() {
	pot = 0;
}

void Holdem::run() {
	//set parameters
	money budget;
	bool retry;
	do {
		retry = !setParams(budget);
	} while (retry);

	constructPlayers(budget);

	roundNum = 1;
	startGame();
}

void Holdem::constructPlayers(const money budget) {
	for (player_num n = 0; n < numOfPlayers; ++n) {
		if (n == humanPos) {
			playerList.push_back(new HumanPlayer(budget, allowAllIn, "You"));
		} else {
			stringstream sstream;
			sstream << "Com#" << n+1;
			playerList.push_back(new ComPlayer(budget, allowAllIn, sstream.str()));
		}
	}

	players = vector<Player*>(playerList);
	
	assert(players.size() == numOfPlayers);
	assert(playerList.size() == numOfPlayers);
}

void Holdem::startGame() {
	bool playAgain;
	do {
		do {
			if (preFlop()) break;
			if (flop()) break;
			if (turn()) break;
			if (river()) break;
			showDown();
		} while (false); //for easier flow control

		playAgain = askContinue();
		if (playAgain) {
			cleanUp();
			checkBroke();
		}
	} while (playAgain);

	gameOver();
}

vector<Player*> Holdem::highestHands(vector<Player*> &competitors) {
	assert(competitors.size() > 0);
	
	//pick the largest hand
	PokerHand highestHand = competitors.front()->getHand();
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

	//erase grouped competitors
	for (vector<Player*>::iterator iter = highests.begin(); iter != highests.end(); ++iter) {
		competitors.erase(std::find(competitors.begin(), competitors.end(), *iter));
	}

	return highests;
}

void Holdem::cleanUp() {
	deck.reset();
	community.clear();
	pot = 0;
	++roundNum;

	for (vector<Player*>::iterator iter = players.begin(); iter != players.end(); ++iter) {
		(*iter)->nextRound();
	}
}