#include <vector>
using std::vector;

#include <cassert>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "Holdem.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComPlayer.h"
#include "Deck.h"
#include "Card.h"

void Holdem::run() {
	//set parameters
	player_num humanPos;
	money budget;
	setParams(humanPos, budget);

	constructPlayers(humanPos, budget);

	roundNum = 1;
	startGame();
}

void Holdem::setParams(player_num &humanPos, money &budget) {
	//set number of players
	do {
		cout << "Set the number of players (" << MINPLAYER << '-' << MAXPLAYER << "): ";
		cin >> numOfPlayers;
	} while (numOfPlayers < MINPLAYER || numOfPlayers > MAXPLAYER);

	//set budget
	do {
		cout << "Set the budget for all players: ";
		cin >> budget;
	} while (budget < 2);

	//set small blind
	do {
		cout << "Set small blind (1-" << budget-1 << "): ";
		cin >> sBlind;
	} while (sBlind < 1 || sBlind >= budget);

	//set big blind
	do {
		cout << "Set big blind (" << sBlind+1 << '-' << budget << "): ";
		cin >> bBlind;
	} while (bBlind <= sBlind || bBlind > budget);

	//set dealer pos
	do {
		cout << "Set dealer's first position (1-" << numOfPlayers << "): ";
		cin >> dealer;
	} while (dealer < 1 || dealer > numOfPlayers);
	--dealer;	//start from 0 in program

	//set human player pos
	do {
		cout << "Set your position (1-" << numOfPlayers << "): ";
		cin >> humanPos;
	} while (humanPos < 1 || humanPos > numOfPlayers);
	--humanPos;	//start from 0 in program

	//set all-in
	bool retry;
	do {
		char allow;
		cout << "Allow ALL-IN? (y/n):";
		cin >> allow;
		switch (allow) {
		case 'y':
		case 'Y':
			allowAllIn = true;
			retry = false;
			break;
		case 'n':
		case 'N':
			allowAllIn = false;
			retry = false;
			break;
		default:
			retry = true;
			break;
		}
	} while (retry);

	cout << endl;
	cout << "Number of players: " << numOfPlayers << endl;
	cout << "Budget: " << budget << endl;
	cout << "Small blind: " << sBlind << endl;
	cout << "Big blind: " << bBlind << endl;
	cout << "Dealer's first position: " << dealer+1 << endl;
	cout << "Your position: " << humanPos+1 << endl;
	cout << "Allow all-in: " << (allowAllIn ? "Yes" : "No") << endl;
	do {
		cout << "Are these settings correct? (y/n): ";
		char ans;
		cin >> ans;
		switch (ans) {
		case 'y':
		case 'Y':
			cout << endl;
			return;
		case 'n':
		case 'N':
			cout << endl;
			setParams(humanPos, budget);
			return;
		default:
			break;
		}
	} while (true);

	assert(false); //the program shouldn't be able to run this line
}

void Holdem::constructPlayers(const player_num humanPos, const money budget) {
	for (player_num n=0; n<numOfPlayers; ++n) {
		if (n == humanPos) {
			HumanPlayer hp(budget);
			players.push_back(hp);
		} else {
			ComPlayer cp(budget);
			players.push_back(cp);
		}
	}
}

void Holdem::startGame() {

}