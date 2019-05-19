#include "PlayScreen.h"

PlayScreen::PlayScreen()
{
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();
	mTimer = Timer::Instance();

	mLevel = new Level();

	mPaused = false;
	mPauseText = nullptr;

	mNewRound = false;

	mCooldownText = nullptr;

	mAudio->PlayMusic("dungeon002.ogg");
}

PlayScreen::~PlayScreen()
{
	delete mLevel;

	delete mPauseText;

	delete mCooldownText;

	mAudio->PauseMusic();
}

void PlayScreen::Pause()
{
	if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE))
	{
		mPaused = !mPaused;

		if (mPaused)
		{
			mPauseText = new Texture("PAUSE", "1942.ttf", 52, { 32, 32, 32 });
			mPauseText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.25f));
		}
		else
		{
			delete mPauseText;
			mPauseText = nullptr;
		}
	}
}

void PlayScreen::HandleLevel()
{
	if (!mNewRound && mLevel->RoundOver())
	{
		mNewRound = true;
		mRoundTimer = 15.0f;
		
		mCooldownText = new Texture(std::to_string((int)mRoundTimer), "DoubleFeature20.ttf", 32, { 112, 10, 10 });
	}

	if (mRoundTimer <= 0.0f && mNewRound || (mInput->KeyPressed(SDL_SCANCODE_RETURN) && mNewRound))
	{
		delete mCooldownText;
		mCooldownText = nullptr;

		mNewRound = false;
		mLevel->StartRound();
	}
	else if (mRoundTimer > 0.0f)
	{
		int time = mRoundTimer;

		mRoundTimer -= mTimer->DeltaTime();

		if (time != (int)mRoundTimer)
		{
			delete mCooldownText;
			mCooldownText = new Texture(std::to_string((int)mRoundTimer), "DoubleFeature20.ttf", 32, { 112, 10, 10 });
		}
	}

	if (mNewRound&& mCooldownText)
	{
		mCooldownText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH - mCooldownText->ScaledDimensions().x,
			Graphics::Instance()->Camera()->y + mCooldownText->ScaledDimensions().y));
	}

	mLevel->Update();
}

bool PlayScreen::GameOver()
{
	return mLevel->GameOver();
}

void PlayScreen::Update()
{
	Pause();

	if (!mPaused)
		HandleLevel();
}

void PlayScreen::Render()
{
	mLevel->Render();

	if (mNewRound && mCooldownText)
		mCooldownText->Render();

	if (mPaused)
		mPauseText->Render();
}
