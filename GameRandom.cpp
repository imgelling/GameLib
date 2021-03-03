#include "GameRandom.h"

GameRandom::GameRandom()
{
	std::random_device rd;
	seed = rd() ^
		(
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() +
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
		);
	generator.seed(seed);
}

void GameRandom::SetSeed(const unsigned int seed)
{
	this->seed = seed;
	generator.seed(seed);
}

unsigned int GameRandom::GetSeed()
{
	return seed;
}

unsigned int GameRandom::Rnd()
{
	return generator();
}

unsigned int GameRandom::RndRange(const unsigned int min, const unsigned int max)
{
	std::uniform_int_distribution<unsigned> distrib(min, max);
	return distrib(generator);
}

GameRandom::~GameRandom()
{
}
