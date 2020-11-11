#include "GameAnimation.h"


GameAnimation::GameAnimation()

{
    currentTime = 0.0;
    currentFrame = 0;
    frames = 0;
    loop = false;
    done = false;
}

GameAnimation::~GameAnimation()
{
	delete[] frames;
	frames = 0;
}

void GameAnimation::SetNumOfFrames(int num)
{
	if (num > 0) frames = new GameAnimationFrame[num];
	maxFrames = num;
}
void GameAnimation::Looped(bool looped)
{
	loop = looped;
}
bool GameAnimation::IsDone()
{
	return done;
}
void GameAnimation::Restart()
{
	currentTime = 0;
	currentFrame = 0;
	done = false;
}

void GameAnimation::TimeUpdate(const float msElapsed)
{
	if (frames == 0) return;
	if (done) return;
	currentTime += msElapsed;
	if (currentTime >= frames[currentFrame].timePerFrame)
	{
		currentTime -= frames[currentFrame].timePerFrame;
		currentFrame++;
		if (currentFrame == maxFrames)
		{
			if (loop) currentFrame = 0;
			else done = true;
		}
	}
}

int GameAnimation::ReturnFrame()
{
	if (frames == 0) return 0;
	if (done) return 2;
	return frames[currentFrame].id;
}
