#pragma once
#include <vector>
#include <iostream>
#include <math.h>

#include "SimplePlayer.h"

/*
* MMLib-Java: PureELOMatchmaker.h
* By Thomas Seabrook
* This is the core class for MMLib, and contains all the methods needed to interact with the matchmaker
* Implement either this or one of the derived classes to implement the matchmaker in your system
*/

/*
* Implementing this matchmaker will give you a Pure, One on One ELO System
* This would be suitable for use in something like chess or a DCG
*/

class PureELOMatchmaker
{
public:
	PureELOMatchmaker(bool a_debugOutputMode = false);
	~PureELOMatchmaker();

	//C++ doesn't support returning arrays like java so here's a shortcut
	struct game
	{
		int p1;
		int p2;
	};

	//Getters/Setters
	int getKFactor() { return kFactor; };
	SimplePlayer* getPlayer(int id);

	void setKFactor(int a_k) { kFactor = a_k; };

	int addPlayer(std::string name);
	int addPlayer(std::string name, int startingELO);
	
	void registerGame(int winnerID, int loserID);
	game formMatch(int maxELODifference);

	bool addPlayerToQueue(int id);
	bool removePlayerFromQueue(int id);

protected:
	//Helper function
	bool isPlayerInQueue(int id);


	//While true, the system will output all operations to the standard C++ console
	//False by default, pass it true in the constructor to turn this on
	bool debugOutputMode;

	std::vector<SimplePlayer*> playerVector;
	std::vector<int> playersInQueue;
	
	int noPlayers = 0;
	int kFactor = 0;
};

