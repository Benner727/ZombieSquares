#pragma once

#include "Texture.h"
#include "InputManager.h"

class SettingsScreen : public GameEntity
{
public:
	SettingsScreen();
	~SettingsScreen();

private:
	InputManager* mInput;

	Texture* mTitleText;

	Texture* mExitSprite;
	bool mExit;

public:
	bool Exit() { if (mExit) { mExit = false; return true; } else return false; }

	void Update();
	void Render();
};