#pragma once
#include <string>

/*
* MMLib-CPP: SimplePlayer.h
* By Thomas Seabrook
* A simple player class, containing only an ELO Value
* This is used in some simple matchmakers to be more efficient
*/

class SimplePlayer
{
public:
	SimplePlayer(std::string a_playerName, int startingELO = 1500);
	~SimplePlayer();

	enum ProcessModes
	{
		REJECT,
		CUTOFF,
		FORCE
	};

	//Getters/Setters
	void setMaxElo(int a_maxELO) { maxELO = a_maxELO; };
	void setMinElo(int a_minELO) { minELO = a_minELO; };
	void setProcessMode(ProcessModes a_processMode) { processMode = a_processMode; };
	void setMinElo(std::string a_playerName) { playerName = a_playerName; };

	int getELO() { return ELO; };
	int getMaxELO() { return maxELO; };
	int getMinELO() { return minELO; };
	std::string getPlayerName() { return playerName; };
	ProcessModes getProcessMode() { return processMode; };

	bool setELO(int a_ELO);
	bool changeELO(int a_ELO);

protected:
	int ELO = 1500;
	int maxELO = -1;
	int minELO = 0;
	std::string playerName;
	ProcessModes processMode = CUTOFF;
};

