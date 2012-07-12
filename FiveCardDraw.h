//David Julia 
//Lab 5 FiveCardDraw.h
#ifndef FIVECARDDRAW_H
#define FIVECARDDRAW_H
#include "stdafx.h"
#include "Game.h"
using namespace std;
class FiveCardDraw: public Game
{
	static const unsigned int HAND_SIZE_=5;
	static const unsigned int MAX_PLAYERS_= 10; 
	static const string GAME_NAME_;

protected:
	virtual int before_turn(Player* p);
	virtual int turn(Player* p);
	//virtual int after_turn(Player* p, unsigned int nonFolded);//nonFolded is the # of players that have not folded at this point in the game- passing this parameter simplifies many things
	virtual void score_round();//helper method for after_turn, implements abstract method in Game

public:
	FiveCardDraw();
	virtual int before_round();
	virtual int round();
virtual int hand_size();
	virtual unsigned int max_players();
	virtual string name();
	
};
bool poker_rank_pptrs( Player* p1, Player* p2);
#endif