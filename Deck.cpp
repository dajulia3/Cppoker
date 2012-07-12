//David Julia 
//Lab 4- Deck.cpp
#include "stdafx.h"
#include "Deck.h"
#include <cctype>
#include <ctime>
using namespace std;

ostream & operator<<(ostream &out, const Deck &deck)
{
	for(int i=0; i<deck.size();++i)
	{
		out<<deck.cards_[i].toString()<<endl;
	}
	return out;
}
Deck & operator<<(Deck& receiver, Deck& giver)
{
	for(size_t i=0; i<giver.cards_.size();++i)
	{
		receiver.add_card(giver.cards_[i]);
	}
	giver=Deck();
	return receiver;
}
Deck::Deck(char * fileName){	
	if(load(fileName)==0){
		reseed();
	}
	else{throw -1;}


}
Deck::Deck()
{
	reseed();
	cards_=vector<Card>();
}


void Deck::add_card(Card card)
{
	cards_.push_back(card);
}

int Deck::load(char * fileName)
{
	cards_=vector<Card>();
	ifstream ifs;	
	ifs.open(fileName);
	if(!ifs.is_open()){
		cout<<"Error opening file"<<endl;
		return -1;
	}	

	string line;
	string cardStr;

		while(ifs>>cardStr)
		{
			Card newCard=Card();
			if(cardStr.size()<4 && rankCard(newCard,cardStr)
				&&	suitCard(newCard,cardStr))//newCard is a valid card if this is true
			{
				cards_.push_back(newCard);
			}

			else cout<<"\nWarning: "+cardStr+" is not a valid card definition string"<<endl;;
		}
	
	ifs.close();
	return 0;
}
//prints the deck one card per line
void Deck::printDeck()
{
	for(size_t i=0;i<cards_.size();++i)
	{
		string cardStr=cards_[i].toString();
		cout<<cardStr<<endl;
	}

}
void Deck::reseed(){
 srand(static_cast<unsigned int>(time(0))); 
}

void Deck::shuffle(){
	for(int i=0;i<rand();++i)//shuffle random # of times
	{
		random_shuffle(cards_.begin(),cards_.end());
	}
}
int Deck::size() const{
	return cards_.size();
}


/*sets card's rank equal to the value specified in the 
*string.
*returns true if successful, false if it failed i.e. the string was invalid.
*this function is used in parsing the cards from the file
*/
bool Deck::rankCard(Card & card, string cardString)
{		
	switch(toupper(cardString[0]))
	{
	case '2': card.rank=Card::two;return true;break;
	case '3': card.rank=Card::three;return true;break;
	case '4': card.rank=Card::four;return true;break;
	case '5': card.rank=Card::five;return true;break;
	case '6': card.rank=Card::six;return true;break;
	case '7': card.rank=Card::seven;return true;break;
	case '8': card.rank=Card::eight;return true;break;
	case '9': card.rank=Card::nine;return true;break;
	case '1': 
		//if the second char isn't 0, then the value isn't  actually 10
		//so the card must be invalid and we return false.
		if(cardString[1]!='0') 
			return false;
		card.rank=Card::ten;return true;break;
	case 'J': card.rank=Card::jack;return true;break;
	case 'Q': card.rank=Card::queen;return true;break; 
	case 'K': card.rank=Card::king;return true;break;
	case 'A': card.rank=Card::ace;return true;break;
	}
	return false;
}

/*sets card's suit equal to the value specified in the 
*string.
*returns true if successful, false if it failed i.e. the string was invalid.
*this function is used in parsing the cards from the file
*/
bool Deck::suitCard(Card & card, string cardString)
{
	switch(toupper(cardString[cardString.size()-1]))
	{
	case 'H':card.suit=Card::hearts;return true;break;
	case 'C':card.suit=Card::clubs;return true;break;
	case'D':card.suit=Card::diamonds;return true;break;
	case 'S':card.suit=Card::spades;return true;break;
	}
	return false;

}


