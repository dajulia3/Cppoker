//David Julia 
//Lab 5 Lab5.cpp
#include "stdafx.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream> 
#include <sstream>
#include <cctype>
#include <algorithm>
#include "Hand.h"
#include "Lab5.h"
#include "Game.h"
using namespace std; 

int main(int argc, char * argv[])
{
	Game* game;
	
	if(argc<=2)return usageMessage(argv[0],"must have at least two arguments ",2);

	if((string(argv[1])!="FiveCardDraw")&&(string(argv[1])!="SevenCardStud")) 
		return usageMessage(argv[0], "\nUnknown game type specified, as of now, only FiveCardDraw and SevenCardStud are supported",302);
	try
	{
		Game::start_game(string(argv[1]));
		game=Game::instance();
		
		for(int i=2; i<argc;++i)
			(*game).add_player(argv[i]);
		
		int temp=0;
		while((*game).has_players())
		{
			temp=(*game).play();
			if(temp!=0)
			{
				(*game).stop_game();
				return temp;
			}
		}
		Game::stop_game(); //after the while loop there are no more players, so stop the game!
		
	}
	catch(Game::exceptions ex)
	{
		if(game!=0)Game::stop_game();
		switch(ex){
			case Game::unknown_game :
				return usageMessage(argv[0], "\nUnknown game type specified, as of now, only FiveCardDraw and SevenCardStud are supported",ex);
			default: 
				return ex;
		}
	}
	catch(int x)
	{
		if(game!=0)Game::stop_game();
		return x;
	}
	catch(...){if(game!=0)Game::stop_game();}

	return 0;
}
int usageMessage(string programName, string message, int errorCode)
{
	string usageStr= programName+" Invalid program arguments.\nMust specify the game type and player name "+ message+"\n";
	string examples= "\tUsage: \""+string(programName)+" Game_Type Player_Name(s)"+ "\n"+
					"\t Eg: "+ string(programName)+ " FiveCardDraw"+ " DavidJulia "+ "YoMama";

	cout<<usageStr<< endl;
	cout<<examples<<endl;
	return errorCode;
}



