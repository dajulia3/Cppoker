//David Julia 
//Lab 5 Card.cpp
#include "stdafx.h";
#include "Card.h"
using namespace std;

const string  Card::SUIT_STR[4] ={"C", "D", "H", "S"};
const string  Card::RANK_STR[13]={"2", "3", "4", "5", "6", "7", 
						"8", "9", "10", "J", "Q", "K", "A"};

Card::Card(bool isFaceUp): suit(spades), rank(ace),isFaceUp_(isFaceUp)
{
}

Card::Card(Suit s, Rank r, bool isFaceUp): suit(s),rank(r) ,isFaceUp_(isFaceUp)
{}

string Card::toString() const
{
	return string(RANK_STR[rank]+SUIT_STR[suit]);
}

/* returns true if this card is less than the card
** to which it is compared.
** A card is compared first by rank, then by suit
*/

bool Card::operator <(const Card & other) const
{
	return (rank<other.rank ||(rank==other.rank && suit<other.suit));
}

//true iff the two cards have same rank and suit
bool Card::operator ==(const Card &other) const
{
	return (rank== other.rank && suit ==other.suit);
}

//true iff the two cards don't have the exact same rank and suit
bool Card::operator !=(const Card &other) const
{
	return (rank!=other.rank||suit!=other.suit);
}
