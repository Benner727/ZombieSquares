#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <ctime>

#include "AnimatedTexture.h"
#include "AudioManager.h"
#include "ScreenManager.h"
#include "PhysicsManager.h"

class GameManager
{
private:
	static GameManager* sInstance;

	static const int FRAME_RATE = 60;
	
	bool mQuit;
	Graphics* mGraphics;
	AssetManager* mAssetManager;
	InputManager* mInputManager;
	AudioManager* mAudioManager;
	PhysicsManager* mPhysicsManager;

	Timer* mTimer;

	SDL_Event mEvent;

	ScreenManager* mScreenManager;

	Uint32 mCurrentTime, mLastTime;
	int mCurrentFrames, mLastFrames;
	bool mFrameCounter;
	Texture* mFrameText;

	GameManager();
	~GameManager();

	void CountFrames();

	void EarlyUpdate();
	void Update();
	void LateUpdate();

	void Render();

public:
	static GameManager* Instance();
	static void Release();

	void Run();
};

#endif