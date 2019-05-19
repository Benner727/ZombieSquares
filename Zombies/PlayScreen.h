#pragma once

#include "Level.h"

class PlayScreen
{
public:
	PlayScreen();
	~PlayScreen();

private:
	InputManager* mInput;
	AudioManager* mAudio;
	Timer* mTimer;

	Level* mLevel;

	bool mPaused;
	Texture* mPauseText;

	bool mNewRound;
	float mRoundTimer;
	Texture* mCooldownText;

	void Pause();
	void HandleLevel();

public:
	bool GameOver();

	void Update();
	void Render();
};

