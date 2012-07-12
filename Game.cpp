//David Julia 
//Lab 5 Game.cpp
#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

Game* Game::game_=0;
Game::Game(): pot_(0), dealerPos_(0){}
Game* Game::instance()
{
	if(Game::game_==0) throw Game::instance_not_available;
	return Game::game_;
}



void Game::start_game( const string& gameType)
{
	if(Game::game_!=0) throw Game::game_already_started;
	else if(gameType=="FiveCardDraw")
	{
		cout<<"Welcome to David's Five Card Draw\n---------------------------------"<<endl;
		FiveCardDraw* fiveCard= new FiveCardDraw();
		Game::game_= fiveCard;
	}
	else if(gameType=="SevenCardStud")
	{
		cout<<"Welcome to David's Seven Card Stud\n---------------------------------"<<endl;
		SevenCardStud* sevenCard= new SevenCardStud();
		Game::game_= sevenCard;
		//TODO: Fix once SevenCardStud is implemented
	}
	else throw Game::unknown_game;

}
void Game::stop_game()
{
	if(Game::game_==0) {cout<<"GAME STOPGAME ==0!!!"<<endl;
	throw Game::no_game_in_progress; }
	else
	{
		delete Game::game_;
		Game::game_=0;
	}
}
bool Game::has_players()
{
	return !players_.empty();
}

void Game::add_player(const string & name)
{


	if(players_.size()>=max_players())
		throw Game::too_many_players;

	else if(find_player(name)==0)
	{
		Player *p;
		p = new Player(name.c_str());
		players_.push_back(p);
	}
}

Player* Game::find_player(const string & name)
{
	for(size_t i=0; i<players_.size();++i)
	{
		if((*players_[i]).name_==name) 
			return players_[i];
	}

	return 0;
}

int Game::after_turn(Player* p, unsigned int nonFolded)
{
	
	if(((*p).isActive_&&nonFolded>1) || ((*p).isActive_&&players_.size()==1))
		cout<<(*p).name_<<": "<<((*p).hand_.as_string())<<" ("<<(*p).hand_.getRankString()<<")"<<endl;
	else if((*p).isActive_&&nonFolded==1) cout<<(*p).name_<<": I'm not showing my cards, you guys folded!"<<endl;
	else cout<<(*p).name_<<": "<<"Folded"<<endl;
	return 0;
}

int Game::play()
{

	int temp=0;
	temp = before_round();
	if(temp!=0)
	{
		Game::stop_game();
		if(temp==Game::all_players_left)
			return 0;
		return temp;
	}
	temp=round();
	if(temp!=0)
	{
		Game::stop_game();
		return temp;
	}
	temp=after_round();
	if(temp!=0)
	{
		Game::stop_game();
		return temp;
	}
	return temp;
}
int Game::before_round()//No cards are dealt, just sets up the round to begin with dealerPos_ and collecting antes
{
	pot_=0;
	if(check_chips())//check that they're not starting with players with 0 chips
	{
				cout<<"All players have left the game"<<endl<<"Thank you for playing David's Poker Game"<<endl;
				return Game::all_players_left;//code that all players left, end the game
	}
	numBettingPlayers_=players_.size();//All players are currently active
	cout<<"Collecting antes for all players!"<<endl;
	for(size_t i=0; i<players_.size(); i++)
	{
		(*players_[i]).chips_--;//collect the players' antes
		pot_++;
	}

	deck_.shuffle();
	cout<<"The first betting round has now began"<<endl;
	take_bets();
	
	
	if(dealerPos_>players_.size()-1)
	{
		dealerPos_=0;
	}
	cout<<(*players_[dealerPos_]).name_<<" is dealing"<<endl;

	return 0;
}

int Game::do_turn()//calls each player's turn function, then calls take_bets
{
 
	unsigned int nonFolded=0;
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
	
	return 0;
}

int Game::after_round()
{
	score_round();
	for(size_t i=0; i<players_.size();i++)
	{
		(*players_[i]).isActive_=true;
	}
	collect_cards();
	if(leave_game())//if there are no more players
	{
		join_game();
		cout<<"\nNew Round\n-------------------------------"<<endl;
	}

	if(dealerPos_>=players_.size()-1)//if there are no players this will just reset the dealerPos_ to zero which is fine
		dealerPos_=0;
	else dealerPos_++;

	return 0;
}

unsigned int Game::check_or_bet(Player* p)//returns the amount of chips the player put in the pot ie 0 if they check, 1 or more otherwise
{
	unsigned int betAmt=0;
	if((*p).chips_==0){//if they have no chips left, they can't bet so we make them check automatically
		cout<<(*p).name_<<", you have no more chips left, you may only check."<<endl<<(*p).name_<<" checks."<<endl;
		return betAmt;
	}

	string response;
	cout<<(*p).name_<<", you have "<<(*p).chips_<<" chips. Do you wish to check or bet? (c/b)"<<endl;
	while(true){
		cin>>response;

		if(response=="c"||response=="C"||response=="check"){
			cout<<(*p).name_<<" checks."<<endl;
			return betAmt;
			//Do nothing so it will go to the next player
		}
		else if (response=="b"||response=="B"||response=="bet")
		{
			while (betAmt<1||betAmt>(*p).chips_)
			{
				cout<<"You have "<<(*p).chips_<< " chips. How much would you like to bet? (1-"<<(*p).chips_<<")"<<endl; 
				cin>>betAmt;
				if(betAmt<1||betAmt>(*p).chips_)cout<<"Invalid bet, you must enter an amount between 1 and "<<(*p).chips_<<"."<<endl;
			}
			cout<<(*p).name_<<" bets "<<betAmt<<"chips."<<endl;

			(*p).chips_-=betAmt;//take from the player
			(*p).cumBet_+=betAmt;//update their cumulative bet for this round
			pot_+=betAmt;//put in the pot
			return betAmt;
		}
		else cout<<"Invalid response, please be sure to enter a valid choice."<<endl;
	}

}

unsigned int Game::call_raise_fold(Player *p, unsigned int amountToCall)//returns the amount raised if they raise, 0 if they call or fold
{
	string response;
	bool canRaise= (amountToCall<(*p).chips_);
	while(true)//will break this loop by returning upon processing valid input
	{
		cout<<(*p).name_<<", you have "<<(*p).chips_<<" chips and the amount to call is "<<amountToCall<<endl;
		if(canRaise)//the bet on the table is between 1 and the player's number of chips... so he can also raise
		{
			cout<<"Do you wish to call, raise, or fold? (c/r/f)"<<endl;
		}	
		else cout<<"do you wish to call or fold?"<<" (c/f)"<<endl;
		cin>>response;

		if(response=="c"||response=="C"||response=="call")
		{
			cout<<(*p).name_<<" calls."<<endl;
			(*p).cumBet_+=amountToCall;//update their cumBet to the normal amount assuming they have enough chips to "fully" call- will have to change this if do side pots
			if(amountToCall>(*p).chips_)
			{
				cout<<(*p).name_<<", by calling, you have ran out of chips, you cannot participate in betting anymore"<<endl;
				pot_+=(*p).chips_;//can call, but doesn't have the full amount so he gives all remaining chips
				(*p).chips_=0;
				return 0;//he called so return 0 (didn't raise)
				//TODO:Change this if doing side pots
			}
			else
			{
				pot_+=amountToCall;
				(*p).chips_-=amountToCall;
				return 0;//if he has enough chips, he just has to "pay" amountToCall
			}
		}
		else if((response=="r"||response=="R"||response=="raise")&& canRaise)
		{
			unsigned int raiseAmt=0;
			cout<<"You have "<<(*p).chips_<< "chips and the amount you would need just to call is "<<amountToCall<<" chips. How much in addition to this would you like to raise? ("<<"1-"<<(*p).chips_-amountToCall<<")"<<endl; 
			cin>>raiseAmt;
			if(raiseAmt<1||raiseAmt>(*p).chips_-amountToCall) cout<<"Invalid amount. You must enter an amount between 1 and "<<(*p).chips_-amountToCall<<"."<<endl;
			else //valid amount to raise was entered
			{
				cout<<(*p).name_<<" raises "<<raiseAmt<<"."<<endl;
				(*p).cumBet_+=(raiseAmt+amountToCall);//update the player's cumulative bet
				(*p).chips_-=(raiseAmt+amountToCall);//transfer the amount raised + the amount needed just to call from the player to the pot
				pot_+=(raiseAmt+amountToCall);
				return raiseAmt;//return the amount raised
			}

		}
		else if((response=="f"||response=="F"||response=="fold"))
		{
			(*p).isActive_=false;//player folds so they're no longer active in this hand
			numBettingPlayers_--;
			cout<<(*p).name_<<" folds."<<endl;
			return 0;//if they fold, they didn't raise anything
		}
		else cout<<"Invalid response, please be sure to enter a valid choice."<<endl;

	}

}

void Game::take_bets()
{
	for(size_t i=0; i<players_.size(); i++) //reset each player's cumulative bet for this betting round to zero
		(*players_[i]).cumBet_=0;

	unsigned int curBet=0;//initialized to zero since no one has placed a bet thus far
	cout<<"Current pot is "<<pot_<<" chips"<<endl;
	//Betting starts to the dealer's left and subsequently goes clockwise( ie same direction as dealing)
	while(numBettingPlayers_>1)//keep rotating around until all bets are called or everyone checked or folded
	{
		for(size_t i=0; i<players_.size(); i++) //deal the cards starting with 1 after the dealer then wrapping around
		{
			Player & curP=(*players_[(i+dealerPos_+1)%players_.size()]);//i+dealerPos_+1 because in poker, betting always starts to the dealer's left, %player size wraps it
			if ((curP.isActive_))//if the player is still active in this hand ie hasn't folded
			{
				/*Begin code for prompting user to enter choice*/
				//string response; 
				//unsigned int betAmt=0;//the amount that will eventually be transferred to the pot from the current player
				if(curBet==0)//no one has placed a bet yet thus they can either check or bet
				{
					curBet = check_or_bet(&curP);//set the current bet to the amount the player put into the pot (0 if they checked)
					if(i==players_.size()-1&&curBet==0) return;//no bets were placed this round- exit the loop/method
				}
				else//there has been a bet placed
				{
					unsigned int amountToCall=curBet-curP.cumBet_;//the amount it takes to call the current bet
					if(curP.chips_<=0)
					{
						cout<<curP.name_<<", you have ran out of chips, you cannot participate in betting"<<endl;
						numBettingPlayers_--;//They are not counted as "active" but the Player's state itself is still active so they will still play through the round and won't be counted as folded
					}
					else if(amountToCall==0)//everyone has called this player's bet... end the betting round
					{
						cout<<"This round of betting is over.\n"<<endl;
						return;
					}

					else{
						curBet+=call_raise_fold(&curP, amountToCall);//increment curBet by the amount raised
					}
				}
			}
		}
	}
}
void Game::addSingleDeckOfCards()
{
	for(int i=0; i<13;++i)
	{
		for(int j=0;j<4;++j)
			deck_.add_card(Card(Card::Suit(j), Card::Rank(i)));
	}
}

bool Game::check_chips()
{
	for(size_t i=0; i<players_.size();++i)
	{
		if((*players_[i]).chips_==0)
		{	
			cout<<(*players_[i]).name_<<", you ran out of chips"<<endl;
			cout<<"if you do not reset your chip count you will be removed from the game."<<endl;
			cout<<"would you like to reset your chip count to the initial amount of "<<Player::initial_chip_count<<"? (y/n)"<<endl;

			string response; 
			cin>>response;

			if(response=="n" ||response=="no")
			{
				remove_player((players_[i]));
				i--;//removed a player so size will be one less, don't want to skip checking a player's chip count
			}
			else if(response=="y"||response=="yes")
			{
				(*players_[i]).chips_=Player::initial_chip_count;
			}
			else //invalid input, was not y, n, yes or no
			{
				cout<<"You must enter either \"y\", \"n\",\"yes\",or \"no\""<<endl;
				i--;//so that it loops through again asking the same player for a yes or no answer
			}
		}
	}

	return(players_.empty());//if there are no more players left, we return true, so that the game knows to terminate
}
void Game::remove_player(Player* p)//must pass the function a non-null pointer, otherwise it throws an invalid_player_ptr exception
{
	bool isValid=false;

	if(p!=0)//true if it's a valid player eg if found using findplayer, the player actually exists when this is true
	{
		if(players_[dealerPos_]>p)dealerPos_--;//Fixes bug of incorrect dealer increment when players leave

		ofstream ofs;
		try{
			ofs.open((*p).name_.c_str(),ios::out);

			if(ofs.is_open())
			{
				ofs<<*p;
				ofs.flush();
				ofs.close();
			}

		}
		catch(...)
		{
			cout<<"error outputting player data during removal"<<endl;
			if(ofs.is_open())ofs.close();
			throw Game::io_error;
		}

		for( vector<Player*>::iterator it = players_.begin(); it != players_.end();it++ )
			if( (*it)==p )
			{
				delete * it;  
				players_.erase(it);
				isValid=true;
				break;
			}
	}

	if(!isValid)//The function was passed a null ptr or a ptr to a Player that is not actually in the game
		throw Game::invalid_player_ptr;
}

Game::~Game()
{
	for(size_t i=0; i<players_.size();++i)
		delete players_[i];
}

void Game::discard_cards(Player* p)//used in games where the players are allowed to discard cards
{
	unsigned int length=40;
	istringstream reader;
	vector<Card> toDiscard;
	bool isValid=false;

	cout<<(*p).name_<<": "<<((*p).hand_.as_string())<<endl;
	cout<<"\n"<<(*p).name_<<", enter the cards you wish to discard separated by spaces\n eg AS 6H \n(Enter \"n\" if you do not wish to discard any)\n"<<endl;
	int loopCount=0;
	char input[50];
	cin.getline(input,length);
	while(!isValid)
	{	
		isValid=true;
		loopCount++;
		//char input[50];
		cin.clear();
		if(input[0]==0)cin.getline(input, length);
		reader.clear();
		reader.str(input);
		string temp; //this is fine as an int since chars can be read to ints (they're just represented as their values and then cast)
		while(reader>>temp)
		{
			if(temp=="n"&&toDiscard.size()==0)//if they don't want to discard any cards and n is the only thing input
				return;//then it is a valid input, don't discard any cards.

			Card tempCard;
			if(Deck::rankCard(tempCard,temp)&&Deck::suitCard(tempCard,temp)&&(toDiscard.size()<=(*p).hand_.size()))//if the Card they input is  a valid card and there haven't been too many things input
			{
				toDiscard.push_back(tempCard);
			}
			else //invalid card string input
			{
				isValid=false;
				//break;
			}
		}

		int validCount=0;//it could be the case that they entered only cards that aren't in their hand			
		if(isValid)
		{
			for(size_t i=0;i<toDiscard.size();++i)
			{	
				for(int j=0;j<(*p).hand_.size();j++)
				{
					if((*p).hand_[j]==toDiscard[i])//the card is not only valid, but is in their hand
					{
						(*p).hand_.remove_card(j);
						disDeck_.add_card(toDiscard[i]);
						validCount++;
					}
				}
			}
		}
		if(loopCount>1&&validCount==0||!isValid)
		{
			cout<<"One or more cards entered were invalid or are not in your hand. Please reenter the cards you wish to discard."<<endl;
			isValid=false;
		}
	}

	cout<<endl;
}

int Game::replace_discarded_cards(Player *p)
{
	while((*p).hand_.size()<hand_size())
	{
		if(deck_.size()>0)
			(*p).hand_<<deck_;
		else if(disDeck_.size()>0)
		{
			disDeck_.shuffle();
			(*p).hand_<<disDeck_;
		}
		else return Game::ran_out_of_cards; //it is abnormal to run out of all cards- failure
	}
}

bool Game::leave_game()//Returns false if all players have left, true otherwise
{
	istringstream iss;
	string response;
	while(true)//if they say no, we return so this isn't actually an infinite loop!
	{
		cout<<"Would any players like to leave the game (y/n or yes/no)?"<<endl;
		char input[301];
		cin.getline(input, 40);
		iss.clear();
		iss.str(input);

		iss>>response;
		if(response=="n" ||response=="no")
			return true;
		else if(response=="y"||response=="yes")
		{
			cout<<"Please enter the names of the players who wish to leave\n(type a space in between each name)";
			cin.getline(input, 300);
			iss.clear();
			iss.str(input);
			string pName="";

			while(iss>>pName)//go through the player names inputted
			{
				Player* p= find_player(pName);
				if(p==0) //Notifies user of invalid player names, but still removes valid ones
					cout<<"\""<<pName<<"\" "<<"is not a valid player name"<<endl; 

				else {
					remove_player(p);//if it was a valid player name
					cout<<pName<<" has left the game"<<endl;
				}

			}

		}

		if(players_.empty())//if there are no more players left, we're done asking people if they want to leave
		{
			cout<<"All players have left the game"<<endl<<"Thank you for playing David's Poker Game"<<endl;
			return false;
		}
	}
	return true;
}

void Game::collect_cards()
{

	for(vector<Player*>::iterator it=players_.begin(); it<players_.end();it++)
	{
		while((**it).hand_.size()!=0)
		{
			deck_.add_card((**it).hand_[0]);
			(**it).hand_.remove_card(0);
		}
	}
	deck_<<disDeck_;//Friend extraction operator to deck defined in Deck.cpp- transfers contents of righthand deck to left hand deck
}

int Game::deal_cards(unsigned int numCardsToDeal,bool areFaceUp)
{
	/*	Old way of dealing for FiveCardDraw
	//Note that hand_size() returns the correct hand size for this type of poker game ie 5 for five card draw, 7 for seven card stud
	while((*(players_[dealerPos_])).hand_.size()<hand_size()) //while the dealer doesn't have correct # of cards keep dealing (dealer is always the last person to get necessary # of cards)
	{
		if(deck_.size()<1) return Game::ran_out_of_cards; //abnormal to not have enough cards... this SHOULD never happen since we make sure not to add too many players but I'd rather be too careful than not enough.
		for(size_t i=0; i<players_.size(); i++) //deal the cards starting with 1 after the dealer then wrapping around
		{
			if (((*players_[ (i+dealerPos_+1)%players_.size()]).hand_.size())<hand_size())
				(*players_[ (i+dealerPos_+1)%players_.size()]).hand_<<deck_;

		}
	}*/
	for(unsigned int j=0;j<numCardsToDeal;j++)
	{
		if(deck_.size()<1) return Game::ran_out_of_cards; //abnormal to not have enough cards... this SHOULD never happen since we make sure not to add too many players but I'd rather be too careful than not enough.
		for(size_t i=0; i<players_.size(); i++) //deal the cards starting with 1 after the dealer then wrapping around
		{
			Player & curP= (*players_[ (i+dealerPos_+1)%players_.size()]);
			(curP.hand_<<deck_).isFaceUp_=areFaceUp;//deals the cards face up or face down as specified
		}
	}
	return 0;

}

void Game::join_game()
{
	istringstream iss;
	string response="";
	while(players_.size()<max_players())
	{
		cout<<"Would any players like to join the game (y/n or yes/no)?"<<endl;
		char input[301];
		cin.getline(input, 40);
		iss.clear();
		iss.str(input);

		iss>>response;
		if(response=="n" ||response=="no")
			return;
		else if(response=="y"||response=="yes")
		{
			cout<<"Please enter the names of the players who wish to join\n(type a space in between each name)";
			cin.getline(input, 300);
			iss.clear();
			iss.str(input);
			string pName="";
			while(iss>>pName)//go through the player names inputted
			{
				if(players_.size()>=max_players())break;//should never be greater than, but if it's equal break

				try{

					if(find_player(pName)==0)
					{
						add_player(pName);
						cout<<pName<<" has joined the game"<<endl;
					}
					else cout<<pName<<" was not added, "<<pName<< " is already playing"<<endl;
				}
				catch(Game::exceptions x)
				{
					if(x==Game::already_playing)
						cout<<pName<<" was not added, "<<pName<< "is already playing"<<endl;
				}
				catch(...){}

			}
		}
	}
	cout<<"No more players may join, the maximum number of players for "<<name()<<" is ten"<<endl; //correctly, this will only print if they try to add too many since we return if they say "n" or "no"
	return;
}


