//David Julia 
//Lab 5 SevenCardStud.cpp
#include "stdafx.h"
#include "SevenCardStud.h"
#include "combination.h"

const string SevenCardStud::GAME_NAME_="Seven Card Stud";//static member definition

SevenCardStud::SevenCardStud():roundNum_(0)
{
	addSingleDeckOfCards();
}

int SevenCardStud::before_turn(Player* p)
{
	return 0;
}
	
int SevenCardStud::turn(Player* p)
{
	cout<<"-----"<<(*p).name_<<"'s turn-----"<<endl;
	cout<<"Other players' hands as you can see them:"<<endl;
	for(size_t i=0; i<players_.size();++i)
	{
		if(players_[i]!=p)cout<<(*players_[i]).name_<<(*players_[i]).hand_.as_string_faceing()<<endl;
	}

	cout<<"Your hand: "<<(*p).hand_.as_string()<<endl;

	return 0;
}

int SevenCardStud::round()
{
	take_bets();
	cout<<"Round 1"<<endl;
	deal_cards(2,true);//2 faceup
	deal_cards(1,false);;//deal the facedown card
	for(size_t i=0; i<players_.size(); i++) //call turn in order of dealing
			turn(players_[ (i+dealerPos_+1)%players_.size()]);
	take_bets();

	for(int i=2;i<5;++i)
	{
		cout<<"Round "<<i<<endl;
		deal_cards(1,true);//1 faceup

	for(size_t i=0; i<players_.size(); i++) //call turn in order of dealing
			turn(players_[ (i+dealerPos_+1)%players_.size()]);
	
	take_bets();
	}

	cout<<"Round 5"<<endl;
	deal_cards(1,false);//1 facedown
for(size_t i=0; i<players_.size(); i++) //call turn in order of dealing
			turn(players_[ (i+dealerPos_+1)%players_.size()]);
	
	take_bets();
	
	score_round();
	return 0;
}

void SevenCardStud::score_round()
{
	vector<Player*> tPlayers= vector<Player*>(players_);
	sort(tPlayers.begin(),tPlayers.end(),scs_poker_rank_pptrs);
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

bool scs_poker_rank_pptrs( Player* p1, Player* p2)//I can't get this to work with combos... I've tried forever
{

	//Hand vector<Card>;
	//while(next_combination(n.begin(),n.end(),r.begin(),r.end() ));
	if(p1==0||p2==0)
		throw Game::invalid_player_ptr;
	return poker_rank((*p1).hand_,(*p2).hand_);

}
int SevenCardStud::hand_size()
{
	return HAND_SIZE_;
}
unsigned int SevenCardStud::max_players()
{
	return MAX_PLAYERS_;
}

string SevenCardStud::name()
{
	return GAME_NAME_;
}