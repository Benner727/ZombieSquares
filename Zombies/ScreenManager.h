#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "StartScreen.h"
#include "PlayScreen.h"
#include "SettingsScreen.h"

class ScreenManager
{
private:
	enum SCREENS { start, play, settings, quit };

	static ScreenManager* sInstance;

	ScreenManager();
	~ScreenManager();

	InputManager* mInput;

	SCREENS mCurrentScreen;

	StartScreen* mStartScreen;
	PlayScreen* mPlayScreen;
	SettingsScreen* mSettingsScreen;

public:
	bool CloseGame() const { return mCurrentScreen == quit; }

	static ScreenManager* Instance();

	static void Release();

	void Update();
	void Render();
};

#endif