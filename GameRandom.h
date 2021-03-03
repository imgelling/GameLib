#pragma once
#include <random>
#include <chrono>

class GameRandom
{
public:
	GameRandom();
	void SetSeed(const unsigned int);
	unsigned int GetSeed();
	unsigned int Rnd();
	unsigned int RndRange(const unsigned int, const unsigned int);
	~GameRandom();
private:
	unsigned int seed;
	std::mt19937 generator;

};

