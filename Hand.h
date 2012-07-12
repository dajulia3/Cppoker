//David Julia 
//Lab 5 Hand.h
#ifndef HAND_H
#define HAND_H
#include "stdafx.h"
#include "Card.h"
#include <vector>

using namespace std;
class Deck;
class Hand
{
	
public:
	friend bool poker_rank(const Hand & h1, const Hand & h2);
	static const enum HandRank  {unranked,pair, two_pair,three_of_a_kind,straight,flush,full_house,four_of_a_kind, straight_flush};
	static const string HAND_RANK_STR[9];
	static const enum exceptions {index_out_of_bounds=100};

	friend ostream & operator<<(ostream &out, const Hand &hand);
	friend Card & operator<<(Hand & hand, Deck & deck);
	Hand();
	Hand(const Hand& h);
	Hand(vector<Card> cards);
	Hand operator=(const Hand & other);
	
	void remove_card(size_t i);
	Card operator[](size_t i) const;
	int  size() const;
	bool operator<(const Hand & other)const;
	string as_string() const;
	string as_string_faceing() const;
	string getRankString();
	
	
	
private:
	HandRank rank_;
	vector<Card> cards_;
	vector<Card::Rank> highCardRanks_;
	void rankHand();
};

ostream &  operator<<(ostream &out, const Hand &hand);
Card & operator<<(Hand & hand, Deck & deck);
bool poker_rank(const Hand & h1, const Hand & h2);
bool areEqualRank( const Hand& h1, const Hand & h2);
#endif

