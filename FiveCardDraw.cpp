//David Julia 
//Lab 5 FiveCardDraw.cpp
#include "stdafx.h"
#include "FiveCardDraw.h"
#include "Hand.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include<algorithm>
using namespace std;

const string FiveCardDraw::GAME_NAME_("Five Card Draw");//static (non-integral) member definition

FiveCardDraw::FiveCardDraw()
{
	addSingleDeckOfCards();
}

int FiveCardDraw::before_turn(Player* p)
{
	discard_cards(p);
	return replace_discarded_cards(p);
	
}
int FiveCardDraw::turn(Player* p)
{
	return 0;
}
 //after_turn(Player* p, unsigned int nonFolded);//nonFolded is the # of players that have not folded at this point in the game- passing this parameter simplifies many things
	
int FiveCardDraw::before_round()
{
	int temp = Game::before_round();
	//5 card draw only
	deal_cards(5,true);//for FiveCardDraw must deal 5 cards

	unsigned int nonFolded=0;
	for(size_t i=0; i<players_.size(); i++) 
		if((*players_[i]).isActive_) nonFolded++;

	
	if(nonFolded>1||players_.size()==1)//only ask them to discard cards if everyone other than them didn't fold or if there is only one player
	{
		for(size_t i=0; i<players_.size(); i++) //call discard_cards in the same order as above
		{
			if((*players_[(i+dealerPos_+1)%players_.size()]).isActive_)
				before_turn((players_[ (i+dealerPos_+1)%players_.size()]));
		}
	}
	return temp;
}

int FiveCardDraw::round()
{
	
	do_turn();
	return 0;
	/*unsigned int nonFolded=0;
	for(size_t i=0; i<players_.size(); i++) 
		if((*players_[i]).isActive_) nonFolded++;
	if(nonFolded>1||players_.size()==1)//if there is only one player, let him play the round alone
	{
		for(size_t i=0; i<players_.size(); i++)
		{
		
			int turnRes=turn(players_[ (i+dealerPos_+1)%players_.size()]);
			if(turnRes!=0) return turnRes;
			cout<<endl;
		}
	}
		else cout<<"All other players have folded to the last bet"<<endl;
	for(size_t i=0; i<players_.size(); i++) 
		after_turn(players_[ (i+dealerPos_+1)%players_.size()],nonFolded);
		
	
	if(numBettingPlayers_<2)
	{
		cout<<"No one can bet, skipping betting round"<<endl;
		return 0;//everyone but one person folded in the first betting phase
	}
	cout<<"The next betting round has now began"<<endl;
	take_bets();
	
	return 0;*/
}

bool poker_rank_pptrs( Player* p1, Player* p2)
{
	if(p1==0||p2==0)
		throw Game::invalid_player_ptr;
	return poker_rank((*p1).hand_,(*p2).hand_);

}
void FiveCardDraw::score_round()
{
	vector<Player*> tPlayers= vector<Player*>(players_);
	sort(tPlayers.begin(),tPlayers.end(),poker_rank_pptrs);
	Hand winningHand= (*tPlayers[0]).hand_;//last player has the/a winning hand
	int numWinners=0;
	for(size_t i=0;i<tPlayers.size();++i) //loop to count the number of winners (to know how many people need to split the pot if necessary)
		if(areEqualRank(winningHand,(*tPlayers[i]).hand_))
			numWinners++;
	cout<<"\n\n****Score Board****\n------------------------"<<endl;

	unsigned int nonFolded=0;
	for(size_t i=0; i<players_.size(); i++) 
		if((*players_[i]).isActive_) nonFolded++;//must know how many people folded so we can know whether or not to display winner's hand
	for(size_t i=0;i<tPlayers.size();++i)
	{
		if(areEqualRank(winningHand,(*tPlayers[i]).hand_))//see the areEqualRank function defined in Hand.cpp
		{
			if(numWinners>1)(*tPlayers[i]).chips_+=(pot_/numWinners);//if there's more than one winner, divide the pot up evenly among them
			else//otherwise just give the one winner the pot (avoids truncation due to integer division)
				(*tPlayers[i]).chips_+=pot_;
			(*tPlayers[i]).wins_++;//increment wins for all players with hand of equal rank to the winning hand
		}
		else (*tPlayers[i]).losses_++;//increment losses for all players who have a lower ranked hand

		cout<<(*tPlayers[i]).name_<<" Wins: "<<(*tPlayers[i]).wins_<<" Losses: "<<(*tPlayers[i]).losses_<<" Chips: "<<(*tPlayers[i]).chips_<<endl;
		if((*tPlayers[i]).isActive_&&nonFolded>1)cout<<"\tHand: "<<(*tPlayers[i]).hand_.as_string()<<endl;
		else if ((*tPlayers[i]).isActive_&&nonFolded==1)
			cout<<"Everyone else folded, no need to show hand"<<endl;
		else cout<<"Folded before showing hand"<<endl;
	}

}

int FiveCardDraw::hand_size()
{
	return HAND_SIZE_;
}

unsigned int FiveCardDraw::max_players()
{
	return MAX_PLAYERS_;
}

string FiveCardDraw::name()
{
	return GAME_NAME_;
}