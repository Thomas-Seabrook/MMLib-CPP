#include "SimplePlayer.h"

/*
 * MMLib-CPP: SimplePlayer.h
 * By Thomas Seabrook
 * Implementation of SimplePlayer.h
 */

SimplePlayer::SimplePlayer(std::string a_playerName, int startingELO)
{
	playerName = a_playerName;
	ELO = startingELO;
}


SimplePlayer::~SimplePlayer()
{
	
}

bool SimplePlayer::setELO(int a_ELO)
{
	//Sets the ELO to a given value
	//Returns true if the elo was modified
	
	switch(processMode)
	{
		case FORCE:
			ELO = a_ELO;
			return true;

		case REJECT:
			if (maxELO >= 0)
			{
				//There is a maximum set
				if (minELO >= 0)
				{
					//There is a minimum set
					if (a_ELO > minELO && a_ELO < maxELO)
					{
						ELO = a_ELO;
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					//Max set but no min
					if (a_ELO < minELO)
					{
						ELO = a_ELO;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else if (minELO >= 0)
			{
				//There is a minimum set but no maximum
				if (a_ELO > minELO)
				{
					ELO = a_ELO;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				//No min/max set
				ELO = a_ELO;
				return true;
			}

		case CUTOFF:
			if (maxELO >= 0)
			{
				//Maximum set
				if (minELO >= 0)
				{
					//Minimum set
					if (a_ELO > minELO && a_ELO < maxELO)
					{
						ELO = a_ELO;
						return true;
					}
					else if (a_ELO < minELO)
					{
						ELO = minELO;
						return true;
					}
					else if (a_ELO > maxELO)
					{
						ELO = maxELO;
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					//There is a maximum set but no minimum
					if (a_ELO < maxELO)
					{
						ELO = a_ELO;
						return true;
					}
					else
					{
						ELO = maxELO;
						return true;
					}
				}
			}
			else if (minELO >= 0)
			{
				//There is a minimum set but no maximum
				if (a_ELO > minELO)
				{
					ELO = a_ELO;
					return true;
				}
				else
				{
					ELO = minELO;
					return true;
				}
			}
			else
			{
				//No min/Max set to just process
				ELO = a_ELO;
				return true;
			}

		default:
			//How did you get here
			return false;
	}
}

bool SimplePlayer::changeELO(int amount)
{
	//Increases/decreases the elo by the given amount
	//Returns true if it has modified the ELO, regardless of if fully

	switch (processMode)
	{
	case FORCE:
		ELO += amount;
		return true;

	case REJECT:
		if (maxELO >= 0)
		{
			//There is a maximum set
			if (minELO >= 0)
			{
				//There is a minimum set
				if (ELO + amount > minELO && ELO + amount < maxELO)
				{
					ELO += amount;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				//There is a maximum set but no minimum
				if (ELO + amount < maxELO)
				{
					ELO += amount;
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else if (minELO >= 0)
		{
			//There is a minimum set but no maximum
			if (ELO + amount > minELO)
			{
				ELO += amount;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			//No min/Max set to just process
			ELO += amount;
			return true;
		}

	case CUTOFF:
		if (maxELO >= 0)
		{
			//There is a maximum set
			if (minELO >= 0)
			{
				//There is a minimum set
				if (ELO + amount > minELO && ELO + amount < maxELO)
				{
					ELO += amount;
					return true;
				}
				else if (ELO + amount < minELO)
				{
					ELO = minELO;
					return true;
				}
				else if (ELO + amount > maxELO)
				{
					ELO = maxELO;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				//There is a maximum set but no minimum
				if (ELO + amount < maxELO)
				{
					ELO += amount;
					return true;
				}
				else
				{
					ELO = maxELO;
					return true;
				}
			}
		}
		else if (minELO >= 0)
		{
			//There is a minimum set but no maximum
			if (ELO + amount > minELO)
			{
				ELO += amount;
				return true;
			}
			else
			{
				ELO = minELO;
				return true;
			}
		}
		else
		{
			//No min/Max set to just process
			ELO += amount;
			return true;
		}

	default:
		//You should never get here
		return false;
	}

}
