#include "GameErrors.h"

namespace game
{
	GameError::operator bool() const
	{
		return  (lastErrorType != GameErrors::None);
	}
	std::ostream& operator<< (std::ostream& stream , const GameError error)
	{
		uint32_t errID = (uint32_t)error.lastErrorType;
		switch (errID)
		{
		case (uint32_t)GameErrors::None: stream << "GameErrorNone"; break;
		case (uint32_t)GameErrors::GameMemoryAllocation: stream << "GameErrorMemoryAllocation"; break;
		case (uint32_t)GameErrors::GameInvalidParameter: stream << "GameErrorInvalidParameters"; break;
		default: return stream << "GameErrorUnknown";
		}
		// Append the error text to general error code
		return stream << " : " << error.lastErrorString << "\n";
	}

}