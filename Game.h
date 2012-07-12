//David Julia 
//Lab 5 Game.h
#ifndef GAME_H
#define GAME_H
#include "stdafx.h"
#include "Deck.h"
#include "Player.h"
#include <vector>

class Game
{
	
protected:
	static Game* game_;
	Deck deck_;
	Deck disDeck_;//the discard deck
	vector <Player*> players_;
	unsigned int pot_;
	size_t dealerPos_;
	unsigned int numBettingPlayers_;

//	virtual int before_turn(Player* p)=0;
	virtual int turn(Player* p)=0;
	virtual int after_turn(Player* p, unsigned int nonFolded);//nonFolded is the # of players that have not folded at this point in the game- passing this parameter simplifies many things
	
	virtual void discard_cards(Player* p);//helper method for before_turn for games that allow discarding of cards
	virtual int replace_discarded_cards(Player*p);
	virtual bool leave_game();//helper method for after_turn
	virtual void collect_cards();//helper method for after_turn
	virtual int deal_cards(unsigned int numCardsToDeal, bool areFaceUp);
	virtual void join_game();//helper method for after_turn
	virtual void score_round()=0;//helper method for after_turn
	virtual void take_bets();//has no limit on bets no limit games rock!
	virtual void addSingleDeckOfCards();//Adds 52 cards to the deck in ascending order-used to fill deck with as many cards as needed during construction of other games (ie if implemented blackjack may want 3 deck's worth of cards)
	virtual bool check_chips();//If any player has 0 chips at the end of the round, it asks them if they'd like to reset, if not kicks them out
	
public:
	
	static const enum exceptions {instance_not_available=300, game_already_started, unknown_game, no_game_in_progress, already_playing, too_many_players, invalid_player_ptr, io_error, all_players_left,ran_out_of_cards};
	
	Game();
	static Game* instance();
	static void start_game( const string& gameType);
	static void stop_game();
	virtual bool has_players();
	virtual void add_player(const string & name);//checks for proper number of players using max_players()
	
	Player* find_player(const string & name);

	virtual int play();// calls before_round, round and after_round stopping if there's an error
	virtual int before_round();
	virtual int do_turn();//calls each player's turn function, then calls take_bets
	virtual int round()=0;
	virtual int after_round();
	
	virtual void remove_player(Player* p);//removes the player from the game, saving his player data to a file. throws an invalid_player_ptr exception if p is null or not in the game
	virtual ~Game();
	
	
	
	virtual int hand_size()=0;//returns the game's hand size
	virtual unsigned int max_players()=0;//returns the maximum number of players the game supports (varies from game to game)
	virtual string name()=0;
private:
Game(const Game&);
void operator=(const Game&);
unsigned int check_or_bet(Player*  p);//helper method for take_bets
unsigned int call_raise_fold(Player * p, unsigned int curBet);//helper method for take_bets
};
#endif