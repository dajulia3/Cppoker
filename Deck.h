//David Julia 
//Lab 5 Deck.h
#ifndef DECK_H
#define DECK_H
#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Card.h"

using namespace std;
class Hand;
class Deck{
	friend ostream & operator<<(ostream &out, const Deck &deck);
	friend Card & operator<<(Hand &hand, Deck &deck);
	friend Deck & operator<<(Deck& receiver, Deck& giver);//moves all the cards in giver to receiver
public:
	Deck();
	Deck(char * fileName);
	int load(char * fileName);
	void add_card(Card card);
	void reseed();
	void shuffle();
	static bool rankCard(Card & card, string cardString);
	static bool suitCard(Card & card, string cardString);
	int size() const;
	void printDeck();
private:
	vector<Card> cards_;
	
	
};
ostream & operator<<(ostream &out, const Deck &deck);
Deck & operator<<(Deck& receiver, Deck& giver);
#endif