//David Julia 
//Lab 5 SevenCardStud.h
#ifndef SEVENCARDSTUD_H
#define SEVENCARDSTUD_H
#include "stdafx.h"
#include "Game.h"
class SevenCardStud: public Game
{
	
	static const int HAND_SIZE_=7;
	static const unsigned int MAX_PLAYERS_=7;//52 card deck so can only have 7 players max
	static const string GAME_NAME_;
protected:
	unsigned int roundNum_;
	virtual void score_round();

public:
	SevenCardStud();

	virtual int before_turn(Player* p);
	virtual int turn(Player*p);
	virtual int round();
	virtual int hand_size();
	virtual unsigned int max_players();//returns the maximum number of players the game supports (varies from game to game)
	virtual string name();
};
bool scs_poker_rank_pptrs( Player* p1, Player* p2);

#endif