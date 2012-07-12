//David Julia 
//Lab 5 Player.cpp
#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <fstream>
using namespace std;
Player::Player( const char* playerName): name_(playerName), wins_(0), losses_(0), chips_(Player::initial_chip_count), isActive_(true), cumBet_(0)
{
	try{
		
	ifstream fin= ifstream(playerName);
	char tName[100];
	fin>>tName;
	
	if(string(tName)!=string(playerName))
		throw Player::corrupt_file;
	fin>>wins_;
	fin>>losses_;
	fin>>chips_;
	cout<<name_<<" has "<<chips_<<" chips"<<endl;
	fin.close();
	}
	catch(...)
	{
		//Doesn't do anything... I think this is the behavior they wanted
		//since this leaves it in the default state
		
	}

}

/*
Uses the ostream to print out the player's name, 
number of wins, and number of losses
with spaces between them
*/
ostream & operator<<(ostream& out, const Player& p)
{
	out<<p.name_<<' '<<p.wins_<<' '<<p.losses_<<' '<<p.chips_;
	return out;
}