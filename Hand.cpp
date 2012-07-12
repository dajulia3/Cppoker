//David Julia 
//Lab 5 Hand.cpp
#include "stdafx.h"
#include <algorithm>
#include "Hand.h"
#include "Deck.h"
#include "Card.h"
using namespace std;

string const Hand::HAND_RANK_STR[9] ={"unranked","pair","two pair","three of a kind","straight","flush","full house","four of a kind", "straight flush"};
ostream & operator<<(ostream &out, const Hand &hand)
{
	for(size_t i=0;i<hand.cards_.size();++i)
	{
		out<<hand.cards_[i].toString()<<" ";
	}
	out<<endl;
	return out;
}
Card & operator<<(Hand & hand, Deck & deck) //nonmember friend function deals a card from the deck to this hand
{
	Card& temp = deck.cards_.back();
	hand.cards_.push_back(temp);
	deck.cards_.pop_back();
	sort(hand.cards_.begin(),hand.cards_.end());
	if(hand.size()==5)hand.rankHand();//if it has 5 cards, we can rank the hand.
	return temp;
}
Hand::Hand():rank_(unranked)
{
	cards_=vector<Card>();
	highCardRanks_= vector<Card::Rank>();
}
Hand::Hand(const Hand& h)
{
	cards_=vector<Card>(h.cards_);
	rank_=h.rank_;
	highCardRanks_= vector<Card::Rank>(h.highCardRanks_);
		
}

//mutators
Hand Hand::operator=(const Hand & other)
{
	
	if(this!=&other)
	{
		cards_.clear();
		rank_=other.rank_;
		highCardRanks_.clear();
		cards_=vector<Card>(other.cards_);
		highCardRanks_= vector<Card::Rank>(other.highCardRanks_);
		
	}
	return *this;
}

void Hand::remove_card(size_t i)
{
	if(i>cards_.size()||i<0)
		throw Hand::index_out_of_bounds;
	cards_.erase(cards_.begin()+i);
	
}

//accessors
Card Hand::operator[](size_t i) const
{
	if(i>=cards_.size()||i<0)
		throw Hand::index_out_of_bounds;

	return cards_[i];
}

int  Hand::size()const
{
	return cards_.size();
}

bool  Hand::operator<(const Hand & other)const
{
	int s=size();
	if(other.size()<size()){ s=other.size();}

	for(int i=0; i<s;++i)
	{
		if(cards_[i]<other.cards_[i])
			return true;
		else if(other.cards_[i]==cards_[i])
			continue;
		else return false;

	}
	return true;//it's a prefix of the other

}
string Hand::as_string() const
{
	string handStr;
	for(size_t i=0;i<cards_.size();++i)
	{
		handStr+=cards_[i].toString()+" ";
	}
	return handStr;
}
string Hand::as_string_faceing() const
{
	string handStr="";
	for(size_t i=0;i<cards_.size();++i)
	{
		if(cards_[i].isFaceUp_)handStr+=cards_[i].toString();
		else handStr+="*";
		handStr+=" ";
	}
	return handStr;
}
string Hand::getRankString() 
{
	rankHand();
	if(rank_==unranked)//if it's unranked call it "____ high" 
		return Card::RANK_STR[highCardRanks_[0]]+" high";
	
	return HAND_RANK_STR[rank_];
}


//returns true if (and only if) the first hand object ranks higher than the second hand object
//Based on rankings given by prof. Gill under lab2 extra credit
bool poker_rank(const Hand & h1, const Hand & h2)
{
	if(h1.rank_>h2.rank_)
		return true;
	else if (h1.rank_==h2.rank_)
	{
		
		for(size_t i=0; i<h1.highCardRanks_.size();++i)
		{
			
			if(h1.highCardRanks_[i]==h2.highCardRanks_[i])
				continue;
			else return h1.highCardRanks_[i]>h2.highCardRanks_[i];
		}
	}
	return false;
}

void Hand::rankHand()
{	
	highCardRanks_.clear();
//note: matchCount is one less than the number of cards in the same-rank set
int match1Count=0;
int match2Count=0;
bool flush=true;
bool straight=true;
//can have at most 2 same-rank (aka match) sets (2 pair, F house)
int match1Index=-1;
int match2Index=-1;
for(int i=0; i<4;++i){
	//equality of next card not the only condition to increment match1Count,
	// if we did that, it would count X X Y Z Z as 4 of a kind
	if ((cards_[i].rank==cards_[i+1].rank)&&(match1Index==i||match1Index==-1))
	{
		match1Count++;
		match1Index=i+1;
	}
	if(cards_[i].rank!=((cards_[i+1].rank)-1))
		straight=false;
	if(cards_[i].suit!=cards_[i+1].suit)
		flush=false;
}
if(match1Count>0){
	
	if(match1Count==3)
		rank_=four_of_a_kind;
	else {
		for(int i=match1Index;i<4;++i)
			if (cards_[i].rank==cards_[i+1].rank)
			{
				match2Count++;
				match2Index=i+1;
			}
		if(match1Count==2){
			if(match2Count==1)
				rank_=full_house;
			else rank_=three_of_a_kind;
		}
		if(match1Count==1){
			if(match2Count==1)
				rank_=two_pair;
			else if(match2Count==2)
				rank_=full_house;
			else rank_=pair;
		}


	}
	
}
else if(straight&&flush) rank_=straight_flush;
else if(straight) rank_=Hand::straight;
else if(flush) rank_=Hand::flush;
else rank_=unranked;

//Now we deal with the high card stuff
//if there are "matches" add the matched cards first by order of which has more matches
//ie the ranks of the 3 matching cards matter more than those of the two matching cards
//in a full house
	if(match1Count>match2Count|| 
		((match1Count==match2Count&& match1Count>0&&match2Count>0)
		&&(cards_[match1Index].rank>cards_[match2Index].rank)))
	{
		for(int i=0;i<match1Count;++i)
			highCardRanks_.push_back(cards_[match1Index].rank);
		if(match2Count>0)
			for(int i=0;i<match2Count;++i)
				highCardRanks_.push_back(cards_[match2Index].rank);
	}
	else if(match2Count>match1Count || 
			((match1Count==match2Count&& match1Count>0&&match2Count>0)
			&&(cards_[match2Index].rank>cards_[match1Index].rank))){
		for(int i=0;i<match2Count;++i)
			highCardRanks_.push_back(cards_[match2Index].rank);
		for(int i=0;i<match1Count;++i)
			highCardRanks_.push_back(cards_[match1Index].rank);
	}
	if (rank_==unranked||straight||flush)//we just go by high card if unranked, straight or flush
	{
		for(size_t i=0;i<cards_.size();++i)
			highCardRanks_.push_back(cards_[cards_.size()-i-1].rank);
	}
if((rank_==two_pair&& match2Index==3) || ( rank_==four_of_a_kind && match1Index==3))
	highCardRanks_.push_back(cards_[4].rank);
else if(rank_==two_pair&&match1Index==1&&match2Index==4)// XX Y ZZ
	highCardRanks_.push_back(cards_[2].rank);
else if((rank_==two_pair&&match1Index==2)||(rank_==four_of_a_kind&&match1Index==4))
	highCardRanks_.push_back(cards_[0].rank);

	//done dealing with high card stuff
}

bool areEqualRank( const Hand& h1, const Hand & h2)
{
	return(!poker_rank(h1,h2)&&!poker_rank(h2,h1));//if neither are true, then given that it provides a strict weak ordering, they must be equal
}