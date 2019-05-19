#include "ScreenManager.h"

ScreenManager* ScreenManager::sInstance = nullptr;

ScreenManager* ScreenManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new ScreenManager();

	return sInstance;
}

void ScreenManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

ScreenManager::ScreenManager()
{
	mInput = InputManager::Instance();

	mCurrentScreen = start;

	mStartScreen = new StartScreen();
	mPlayScreen = nullptr;
	mSettingsScreen = new SettingsScreen();
}

ScreenManager::~ScreenManager()
{
	delete mStartScreen;
	delete mPlayScreen;
	delete mSettingsScreen;
}

void ScreenManager::Update()
{	
	switch (mCurrentScreen)
	{
	case start:
		mStartScreen->Update();

		if (mStartScreen->Selected())
		{
			switch (mStartScreen->SelectedOption())
			{
			case 1:
				mPlayScreen = new PlayScreen();
				mCurrentScreen = play;
				break;
			case 2:
				mCurrentScreen = settings;
				break;
			case 3:
				mCurrentScreen = quit;
				break;
			}
		}
		break;
	case play:
			mPlayScreen->Update();

			if (mPlayScreen->GameOver())
			{
				delete mPlayScreen;
				mPlayScreen = nullptr;

				mCurrentScreen = start;
			}
		break;
	case settings:
		mSettingsScreen->Update();

		if (mSettingsScreen->Exit())
			mCurrentScreen = start;
		break;
	}
}

void ScreenManager::Render()
{
	switch (mCurrentScreen)
	{
	case start:
		mStartScreen->Render();
		break;
	case play:
		mPlayScreen->Render();
		break;
	case settings:
		mSettingsScreen->Render();
		break;
	}
}