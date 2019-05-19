#pragma once

#include "Map.h"
#include "InputManager.h"
#include "AudioManager.h"

class StartScreen : public GameEntity
{
public:
	StartScreen();
	~StartScreen();

private:
	AudioManager* mAudio;
	InputManager* mInput;
	Timer* mTimer;

	Map* mMap;
	bool mHorizontal;
	bool mVertical;

	Texture* mTitleText;
	Texture* mPlayText;
	Texture* mSettingsText;
	Texture* mQuitText;
	Texture* mVersionText;

	float mBlinkTimer;
	float mBlinkInterval;
	bool mTextVisible;

	int mSelectedOption;
	bool mSelected;

	void HandleOptions();

public:
	int SelectedOption() const { return mSelectedOption; }
	bool Selected() { if (mSelected) { mSelected = false; return true; } else return false; }

	void Update();
	void Render();
};

