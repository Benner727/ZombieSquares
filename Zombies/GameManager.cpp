#include "GameManager.h"

GameManager* GameManager::sInstance = nullptr;

GameManager* GameManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new GameManager();

	return sInstance;
}

void GameManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

GameManager::GameManager()
{
	mQuit = false;

	mGraphics = Graphics::Instance();

	if (!Graphics::Initialized())
		mQuit = true;

	mAssetManager = AssetManager::Instance();

	mInputManager = InputManager::Instance();

	mAudioManager = AudioManager::Instance();

	mTimer = Timer::Instance();

	mPhysicsManager = PhysicsManager::Instance();
	mPhysicsManager->SetLayerCollisionMask(PhysEntity::CollisionLayers::Friendly, PhysEntity::CollisionFlags::Hostile | PhysEntity::CollisionFlags::HostileProjectiles | PhysEntity::CollisionFlags::PowerUp);
	mPhysicsManager->SetLayerCollisionMask(PhysEntity::CollisionLayers::FriendlyProjectiles, PhysEntity::CollisionFlags::Hostile);
	mPhysicsManager->SetLayerCollisionMask(PhysEntity::CollisionLayers::Hostile, PhysEntity::CollisionFlags::Friendly | PhysEntity::CollisionFlags::FriendlyProjectiles);
	mPhysicsManager->SetLayerCollisionMask(PhysEntity::CollisionLayers::HostileProjectiles, PhysEntity::CollisionFlags::Friendly);
	mPhysicsManager->SetLayerCollisionMask(PhysEntity::CollisionLayers::PowerUp, PhysEntity::CollisionFlags::Friendly);

	mScreenManager = ScreenManager::Instance();

	srand(time(nullptr));

	mCurrentTime = mLastTime = 0;
	mCurrentFrames = mLastFrames = 0;
	mFrameCounter = false;
	mFrameText = new Texture(std::to_string(mCurrentFrames), "1942.ttf", 24, { 255, 255, 255 });
	mFrameText->Pos(Vector2(20.0f, 20.0f));
	mFrameText->IgnoreCamera(true);
}


GameManager::~GameManager()
{
	ScreenManager::Release();
	mScreenManager = nullptr;

	PhysicsManager::Release();

	AssetManager::Release();
	mAssetManager = nullptr;

	AudioManager::Release();
	mAudioManager = nullptr;

	Graphics::Release();
	mGraphics = nullptr;

	InputManager::Release();
	mInputManager = nullptr;

	Timer::Release();
	mTimer = nullptr;
}

void GameManager::CountFrames()
{
	mCurrentTime = SDL_GetTicks();
	mCurrentFrames++;

	if (mCurrentTime > mLastTime + 1000.0f)
	{
		if (mCurrentFrames != mLastFrames)
		{
			delete mFrameText;
			mFrameText = new Texture(std::to_string(mCurrentFrames), "1942.ttf", 24, { 255, 255, 255 });
			mFrameText->Pos(Vector2(20.0f, 20.0f));
			mFrameText->IgnoreCamera(true);
		}

		mLastTime = mCurrentTime;
		mLastFrames = mCurrentFrames;
		mCurrentFrames = 0;
	}
}

void GameManager::EarlyUpdate()
{
	mInputManager->Update();
}

void GameManager::Update()
{
	mScreenManager->Update();

	if (mScreenManager->CloseGame())
		mQuit = true;
}

void GameManager::LateUpdate()
{
	mPhysicsManager->Update();

	mInputManager->UpdatePrevInput();
}

void GameManager::Render()
{
	mGraphics->ClearBackBuffer();

	//Render here
	mScreenManager->Render();

	if (mFrameCounter)
		mFrameText->Render();
	
	mGraphics->Render();
}

void GameManager::Run()
{
	while (!mQuit)
	{
		mTimer->Update();

		CountFrames();

		EarlyUpdate();

		while (SDL_PollEvent(&mEvent) != 0)
		{
			if (mEvent.type == SDL_QUIT)
				mQuit = true;
			else if (mEvent.type == SDL_KEYDOWN)
				if (mEvent.key.keysym.sym == SDLK_BACKQUOTE)
					mFrameCounter = !mFrameCounter;
		}

		//EarlyUpdate();
		Update();
		LateUpdate();
		Render();
	}
}