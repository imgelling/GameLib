#include "GameErrors.h"

namespace game
{
	// needs same thing as below
	std::ostream& operator<< (std::ostream& stream, const GameErrors gerror)
	{
		switch (gerror)
		{
		case GameErrors::None: return stream << "GameErrorNone";
		case GameErrors::GameMemoryAllocation: return stream << "GameErrorMemoryAllocation";
		case GameErrors::GameInvalidParameter: return stream << "GameErrorInvalidParameters";
		default: return stream << "Unknown Error";
		}
	}

	bool operator== (const GameErrors err, const bool flag)
	{
		if (err == GameErrors::None)
		{
			return false;
		}
		return true;
	}
}