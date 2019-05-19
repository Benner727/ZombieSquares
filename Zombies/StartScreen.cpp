#include "StartScreen.h"

StartScreen::StartScreen()
{
	mAudio = AudioManager::Instance();
	mInput = InputManager::Instance();
	mTimer = Timer::Instance();

	mMap = new Map();
	mMap->SetFocus(this);
	mHorizontal = rand() % 2;
	mVertical = rand() % 2;

	mTitleText = new Texture("Zombie Squares", "DoubleFeature20.ttf", 72, { 112, 10, 10 });
	mTitleText->Parent(this);
	mTitleText->Pos(Vector2(0.0f, -125.0f));

	mPlayText = new Texture("Play Game", "DoubleFeature20.ttf", 32, { 112, 10, 10 });
	mPlayText->Parent(this);
	mPlayText->Pos(Vector2(0.0f, 50.0f));

	mSettingsText = new Texture("Settings", "DoubleFeature20.ttf", 32, { 112, 10, 10 });
	mSettingsText->Parent(this);
	mSettingsText->Pos(Vector2(0.0f, 125.0f));
	
	mQuitText = new Texture("Quit", "DoubleFeature20.ttf", 32, { 112, 10, 10 });
	mQuitText->Parent(this);
	mQuitText->Pos(Vector2(0.0f, 200.0f));

	mVersionText = new Texture("Alpha 1.3", "1942.ttf", 16, { 255, 255, 240 });
	mVersionText->Parent(this);
	mVersionText->Pos(Vector2(325.0f, 245.0f));

	mBlinkTimer = 0.0f;
	mBlinkInterval = 0.5f;
	mTextVisible = true;

	mSelectedOption = 1;
	mSelected = false;

	Pos(Vector2(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f));

	mAudio->PlayMusic("42b4b.ogg");
}

StartScreen::~StartScreen()
{
	delete mMap;

	delete mTitleText;
	delete mPlayText;
	delete mSettingsText;
	delete mQuitText;
	delete mVersionText;

	mAudio->PauseMusic();
}

void StartScreen::HandleOptions()
{
	if (mInput->KeyPressed(SDL_SCANCODE_UP))
		mSelectedOption--;
	else if (mInput->KeyPressed(SDL_SCANCODE_DOWN))
		mSelectedOption++;

	if (mSelectedOption > 3)
		mSelectedOption = 1;
	else if (mSelectedOption < 1)
		mSelectedOption = 3;

	if (mInput->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
	{
		if (mInput->MousePos().x + Graphics::Instance()->Camera()->x > mPlayText->Pos().x - mPlayText->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().x + Graphics::Instance()->Camera()->x < mPlayText->Pos().x + mPlayText->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y > mPlayText->Pos().y - mPlayText->ScaledDimensions().y * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y < mPlayText->Pos().y + mPlayText->ScaledDimensions().y * 0.5f)
		{
			mSelectedOption = 1;
			mSelected = true;
		}
		else if (mInput->MousePos().x + Graphics::Instance()->Camera()->x > mSettingsText->Pos().x - mSettingsText->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().x + Graphics::Instance()->Camera()->x < mSettingsText->Pos().x + mSettingsText->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y > mSettingsText->Pos().y - mSettingsText->ScaledDimensions().y * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y < mSettingsText->Pos().y + mSettingsText->ScaledDimensions().y * 0.5f)
		{
			mSelectedOption = 2;
			mSelected = true;
		}
		else if (mInput->MousePos().x + Graphics::Instance()->Camera()->x > mQuitText->Pos().x - mQuitText->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().x + Graphics::Instance()->Camera()->x < mQuitText->Pos().x + mQuitText->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y > mQuitText->Pos().y - mQuitText->ScaledDimensions().y * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y < mQuitText->Pos().y + mQuitText->ScaledDimensions().y * 0.5f)
		{
			mSelectedOption = 3;
			mSelected = true;
		}
	}
	
	if (mInput->KeyPressed(SDL_SCANCODE_RETURN))
		mSelected = true;
}

void StartScreen::Update()
{
	Translate(VEC2_RIGHT * (mHorizontal ? -1.0f : 1.0f));
	Translate(VEC2_UP * (mVertical ? -1.0f : 1.0f));

	if (Pos().x < Graphics::SCREEN_WIDTH * 0.5f || Pos().x > (mMap->Width() * mMap->mTileSize) - Graphics::SCREEN_WIDTH * 0.5f)
		mHorizontal = !mHorizontal;

	if (Pos().y < Graphics::SCREEN_HEIGHT * 0.5f || Pos().y >(mMap->Height() * mMap->mTileSize) - Graphics::SCREEN_HEIGHT * 0.5f)
		mVertical = !mVertical;

	Graphics::Instance()->Camera(Pos());

	HandleOptions();

	mBlinkTimer += mTimer->DeltaTime();
	if (mBlinkTimer >= mBlinkInterval)
	{
		mTextVisible = !mTextVisible;
		mBlinkTimer = 0.0f;
	}
}

void StartScreen::Render()
{
	mMap->Render();

	mTitleText->Render();

	if (mTextVisible || mSelectedOption != 1)
		mPlayText->Render();

	if (mTextVisible || mSelectedOption != 2)
		mSettingsText->Render();
	
	if (mTextVisible || mSelectedOption != 3)
		mQuitText->Render();

	mVersionText->Render();
}
