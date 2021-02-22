#include "PureELOMatchmaker.h"



PureELOMatchmaker::PureELOMatchmaker(bool a_debugOutputMode)
{
	debugOutputMode = a_debugOutputMode;

	if (debugOutputMode)
	{
		std::cout << "[MMLib] Initialising System - Pure ELO" << std::endl;
	}
}


PureELOMatchmaker::~PureELOMatchmaker()
{
}

int PureELOMatchmaker::addPlayer(std::string name)
{
	//Add a new player. In this case, the starting elo is just the default (1500)
	//Returns the internal ID of the player
	SimplePlayer* player = new SimplePlayer(name);
	playerVector.push_back(player);

	if (debugOutputMode)
	{
		std::cout << "[MMLib] Added Player " << name << ", ELO " << std::to_string(player->getELO()) << std::endl;
	}

	noPlayers++;
	return noPlayers - 1;
}

int PureELOMatchmaker::addPlayer(std::string name, int startingELO)
{
	//Add a new player. In this case, the starting elo is set by the user
	//Returns the internal ID of the player
	SimplePlayer* player = new SimplePlayer(name, startingELO);
	playerVector.push_back(player);

	if (debugOutputMode)
	{
		std::cout << "[MMLib] Added Player " << name << ", ELO " << std::to_string(player->getELO()) << std::endl;
	}

	noPlayers++;
	return noPlayers - 1;
}

void PureELOMatchmaker::registerGame(int winnerID, int loserID)
{
	//Register a game between 2 players and recalculate their scores

	//Error checking
	if (winnerID >= playerVector.size() || winnerID < 0 || loserID >= playerVector.size() || loserID < 0)
	{
		std::cout << "[MMLib] Matchmaker Error Code 3: Player does not exist" << std::endl;
		return;
	}

	//Save the elo values to be easier
	int p1ELO = playerVector[winnerID]->getELO();
	int p2ELO = playerVector[loserID]->getELO();

	//Player 1's expected score
	float eA = 1 / (1 + pow(10, ((p2ELO - p1ELO) / 400)));

	//Player 2's expected score
	float eB = 1 / (1 + pow(10, ((p1ELO - p2ELO) / 400)));

	//Calculate the new scores
	int newP1Score = p1ELO + (int)(kFactor * (1 - eA));
	int newP2Score = p2ELO + (int)(kFactor * (0 - eB));

	if (debugOutputMode)
	{
		std::cout << "[MMLib] Match registered between " << playerVector[winnerID]->getPlayerName() + " and " + playerVector[loserID]->getPlayerName() << std::endl;
		std::cout << "[MMLib] " << playerVector[winnerID]->getPlayerName() << " New Score: " << newP1Score << std::endl;
		std::cout << "[MMLib] " << playerVector[loserID]->getPlayerName() + " New Score: " << newP2Score << std::endl;
	}

	//Set the new ELO values
	playerVector[winnerID]->setELO(newP1Score);
	playerVector[loserID]->setELO(newP2Score);

}

SimplePlayer* PureELOMatchmaker::getPlayer(int playerID)
{
	//Returns the player object associated with the given ID

	//Error checking
	if (playerID >= playerVector.size() || playerID < 0)
	{
		if (debugOutputMode)
		{
			std::cout << "[MMLib] Matchmaker Error Code 3: Player does not exist";
		}

		return new SimplePlayer("ERROR");
	}
	else
	{
		return playerVector[playerID];
	}
}

bool PureELOMatchmaker::addPlayerToQueue(int id)
{
	//Adds a player to queue

	//Check to see if the player exists
	if (id >= playerVector.size() || id < 0)
	{
		if (debugOutputMode)
		{
			std::cout << "[MMLib] Matchmaker Error Code 3: Player does not exist" << std::endl;
		}
		return false;
	}

	//Check to see if the player is in queue already
	if (isPlayerInQueue(id))
	{
		if (debugOutputMode)
		{
			std::cout << "[MMLib] Matchmaker Error Code 4: Attempted to add player to queue, but they were in queue already" << std::endl;
		}
		return false;
	}

	playersInQueue.push_back(id);

	if (debugOutputMode)
	{
		std::cout << "[MMLib] Player " + playerVector[id]->getPlayerName() + " joined queue" << std::endl;
	}

	return true;
}

bool PureELOMatchmaker::isPlayerInQueue(int id)
{
	//Helper function to replace ArrayList.contains
	if (playersInQueue.size() == 0)
	{
		//No players in queue
		return false;
	}

	for (int i = 0; i < playersInQueue.size(); i++)
	{
		if (playersInQueue[i] == id)
		{
			return true;
		}
	}

	return false;
}

bool PureELOMatchmaker::removePlayerFromQueue(int id)
{
	//Check to see if the player is in queue
	if (!isPlayerInQueue(id))
	{
		if (debugOutputMode)
		{
			std::cout << "[MMLib] Matchmaker Error Code 5: Attempted to remove player from queue, but they were not in queue" << std::endl;
		}

		return false;
	}

	int index = -1;
	//Loop through and remove the player
	for (int i = 0; i < playersInQueue.size(); i++)
	{
		if (playersInQueue[i] == id)
		{
			index = i;
		}
	}

	if (index == -1)
	{
		if (debugOutputMode)
		{
			std::cout << "[MMLib] Matchmaker Error Code 3: Player does not exist" << std::endl;
		}
		return false;
	}

	playersInQueue.erase(playersInQueue.begin() + index);

	if (debugOutputMode)
	{
		std::cout << "[MMLib] Player " + playerVector[id]->getPlayerName() + " left queue" << std::endl;
	}

	return true;

}

PureELOMatchmaker::game PureELOMatchmaker::formMatch(int maxELODifference)
{
	game error;
	error.p1 = -1;
	error.p2 = -1;

	//Loop through the players in queue and attempt to find a pair that satisfies the match elo difference
	if (debugOutputMode) 
	{
		std::cout << "[MMLib] Attempting to resolve match with differential " << std::to_string(maxELODifference) << std::endl;
	}

	if (playersInQueue.size() <= 1) 
	{
		std::cout << "[MMLib] Matchmaker Error Code 1: Too few players to create match" << std::endl;;
		return error;
	}

	for (int i = 0; i < playersInQueue.size(); i++) 
	{
		for (int j = 1; j < playersInQueue.size(); j++) 
		{
			//Dont check if they're the same
			if (i != j) 
			{
				//Get the difference in elos
				int eloDiff = abs(playerVector[playersInQueue[i]]->getELO() - playerVector[playersInQueue[j]]->getELO());
				if (eloDiff <= maxELODifference) 
				{
					if (debugOutputMode) 
					{
						std::cout << "[MMLib] Match created between player " << playerVector[playersInQueue[i]]->getPlayerName() << " and player " << playerVector[playersInQueue[j]]->getPlayerName() << std::endl;
					}

					removePlayerFromQueue(i);
					removePlayerFromQueue(j);

					game g;
					g.p1 = i;
					g.p2 = j;

					return g;
				}
			}
		}
	}

	if (debugOutputMode) 
	{
		std::cout << "[MMLib] Matchmaker Error Code 2: Could not resolve match with differential " << std::to_string(maxELODifference);
	}
	return error;
}
