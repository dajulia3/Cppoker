//David Julia 
//Lab 5 Player.h
#ifndef PLAYER_H
#define PLAYER_H
#include "stdafx.h"
#include "Hand.h"
#include <string>
struct Player
{
	static const enum exceptions {corrupt_file=800};
	static const int initial_chip_count=20;
	string name_;
	unsigned int wins_;
	unsigned int losses_;
	unsigned int chips_;
	unsigned int cumBet_;//the amount the player has bet during this phase of betting (the amount they have actually put into the pot)
	bool isActive_;
	Hand hand_;
	
//initializes its wins and losses member variables both to 0
//(the hand member variable should be default constructed to be empty).
	Player(const char* playerName);

};

ostream & operator<<(ostream& out, const Player& p);
#endif