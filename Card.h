//David Julia 
//Lab 5 Card.h
#ifndef CARD_H
#define CARD_H
#include "stdafx.h"
#include <string>

using namespace std;

struct Card 
{
	
	enum Suit  {clubs, diamonds, hearts, spades};
	enum Rank {two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace};
	
	static const string SUIT_STR[4];
	static const string RANK_STR[13];

	Suit suit;
	Rank rank;
	bool isFaceUp_;

	Card(bool isFaceUp=true);
	Card(Suit s,Rank r,bool isFaceUp= true);
	
	string toString() const;
	bool operator<(const Card & other) const;
	bool operator==(const Card & other)const;
	bool Card::operator !=(const Card &other)const;
};
#endif
