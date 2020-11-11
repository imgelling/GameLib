#pragma once

class GameAnimationFrame
{
public:
	int id; // id of sprite in sprite sheet
	float timePerFrame; // time spent of this frame
};

// clean this up next time, or maybe off stream.
class GameAnimation
{
public:
	GameAnimation();
	~GameAnimation();
	void TimeUpdate(const float);
	void SetNumOfFrames(int);
	int ReturnFrame();
	GameAnimationFrame *frames;
	bool IsDone();
	void Looped(bool);
	void Restart();
private:
	double currentTime;
	int currentFrame;
	int maxFrames;
	bool done;
	bool loop;

};