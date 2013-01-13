#include <cassert>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "Holdem.h"
#include "Player.h"
#include "GameStatus.h"
#include "Card.h"
#include "Stage.h"

bool Holdem::setParams(money &budget) {
	//set number of players
	do {
		cout << "Set the number of players (" << MIN_PLAYER << '-' << MAX_PLAYER << "): ";
		cin >> numOfPlayers;
	} while (numOfPlayers < MIN_PLAYER || numOfPlayers > MAX_PLAYER);

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
			return true;
		case 'n':
		case 'N':
			cout << endl;
			return false;
		default:
			break;
		}
	} while (true);

	throw new std::exception("ERROR: Holdem::setParams() ended in unexpected way");
}

void Holdem::betting(const player_num startPos, Stage stage, money minRaise) {
	player_num currentPos = startPos % numOfPlayers;
	player_num terminatePos = startPos % numOfPlayers;
	GameStatus status;
	money raise = 0;
	money pay = 0;
	Player::Action act;

	do {
		status.community = community;
		status.stage = stage;
		status.pot = pot;
		status.minRaise = minRaise;
		status.currentBet = minRaise;
		Player &player = players.at(currentPos);
		
		
		//Note: check whether the player can do certain action or not is Player's job.
		if ( !(player.isFolded() || player.isAllIn()) ) {
			act = player.generateAction(status, raise, pay);
			switch (act) {
			case Player::Check:
				break;
			case Player::Call:
				break;
			case Player::Raise:
				break;
			case Player::AllIn:
				break;
			case Player::Fold:
				break;
			default:
				throw new std::exception("invalid action");
			}
		}

		++currentPos;
		currentPos %= numOfPlayers;
	} while (currentPos % numOfPlayers != terminatePos % numOfPlayers);
}