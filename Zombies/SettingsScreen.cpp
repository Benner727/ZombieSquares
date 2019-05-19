#include "SettingsScreen.h"

SettingsScreen::SettingsScreen()
{
	mInput = InputManager::Instance();

	mTitleText = new Texture("Settings", "1942.ttf", 64, { 250, 250, 250 });
	mTitleText->Parent(this);
	mTitleText->Pos(Vector2(0.0f, -200.0f));

	mExitSprite = new Texture("ExitSprite.png");
	mExitSprite->Parent(this);
	mExitSprite->Pos(Vector2(mExitSprite->ScaledDimensions().x - Graphics::SCREEN_WIDTH * 0.5f, mExitSprite->ScaledDimensions().y - Graphics::SCREEN_HEIGHT * 0.5f));

	mExit = false;

	Pos(Vector2(Graphics::SCREEN_WIDTH, Graphics::SCREEN_HEIGHT));
}

SettingsScreen::~SettingsScreen()
{
	delete mTitleText;

	delete mExitSprite;
}


void SettingsScreen::Update()
{
	Graphics::Instance()->Camera(Pos());

	if (mInput->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
	{
		if (mInput->MousePos().x + Graphics::Instance()->Camera()->x > mExitSprite->Pos().x - mExitSprite->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().x + Graphics::Instance()->Camera()->x < mExitSprite->Pos().x + mExitSprite->ScaledDimensions().x * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y > mExitSprite->Pos().y - mExitSprite->ScaledDimensions().y * 0.5f &&
			mInput->MousePos().y + Graphics::Instance()->Camera()->y < mExitSprite->Pos().y + mExitSprite->ScaledDimensions().y * 0.5f)
			mExit = true;
	}
	else if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE))
		mExit = true;
}

void SettingsScreen::Render()
{
	mTitleText->Render();

	mExitSprite->Render();
}