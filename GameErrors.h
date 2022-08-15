#pragma once

#include <string>
#include <iostream>

namespace game
{
	enum class GameErrors
	{
		None = 0,
		GameMemoryAllocation,
		GameInvalidParameter
	};
	
	struct GameError
	{
		GameErrors lastErrorType = GameErrors::None;
		std::string lastErrorString = "None";
	};
	bool operator== (const GameError err, const bool flag);
	std::ostream& operator<< (std::ostream& stream, const GameErrors gerror);
}