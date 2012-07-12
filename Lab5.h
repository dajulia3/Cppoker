//David Julia 
//Lab 5 Lab5.h
#ifndef LAB5_H
#define LAB5_H
#include <vector>
#include <string>
#include "Deck.h"

using namespace std;

int usageMessage(string programName, string message, int errorCode);
int getCardsFromFile(vector<Card> & cards, char * fileName);
int printCards(const vector<Card> & cards);
int printHands(const vector<Card> & cards);
#endif