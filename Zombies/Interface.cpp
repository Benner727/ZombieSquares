#include "Interface.h"

Interface::Interface(Player* player)
	: mPlayer(player)
{
	mInput = InputManager::Instance();

	SDL_ShowCursor(SDL_DISABLE);
	mCursor = new Texture("CrosshairSprite.png");
	mCursor->Scale(Vector2(0.3f, 0.3f));

	mLevel = -1;
	mLevelText = nullptr;
	CurrentLevel(1);

	mHealth = -1;
	mHealthText = nullptr;
	CurrentHealth();

	mShield = -1;
	CurrentShield();

	mGun = "";
	mClip = -1;
	mAmmo = -1;
	CurrentAmmo();

	mPoints = -1;
	mKills = -1;
	mScoreText = nullptr;
	CurrentScore();

	mDoublePointsText = new Texture("Double Points", "1942.ttf", 28, { 34,139,34 });
	mDoublePointsText->Parent(this);
	mDoublePointsText->Pos(Vector2(Graphics::SCREEN_WIDTH * 0.5f, mHealthText->ScaledDimensions().y));
}

Interface::~Interface()
{
	delete mLevelText;
	delete mHealthText;
	delete mScoreText;
	delete mDoublePointsText;

	for (std::vector<Texture*>::iterator it = mGunText.begin(); it != mGunText.end(); ++it)
		delete * it;

	for (std::vector<Texture*>::iterator it = mShieldBar.begin(); it != mShieldBar.end(); ++it)
		delete * it;

	for (std::vector<Texture*>::iterator it = mPerksIcon.begin(); it != mPerksIcon.end(); ++it)
		delete * it;

	delete mCursor;
	Graphics::Instance()->Camera(VEC2_ZERO);
	SDL_ShowCursor(SDL_ENABLE);
}

void Interface::CurrentHealth()
{
	if (mHealth != mPlayer->Health())
	{
		mHealth = mPlayer->Health();

		delete mHealthText;
		mHealthText = new Texture("HP:" + std::to_string(mHealth), "1942.ttf", 24, { 0, 139, 139 });
		mHealthText->Parent(this);
		mHealthText->Pos(Vector2(mHealthText->ScaledDimensions().x * 0.6f, mHealthText->ScaledDimensions().y * 0.75f));
	}
}

void Interface::CurrentShield()
{
	if (mShield != mPlayer->Shield())
	{
		mShield = mPlayer->Shield();

		for (std::vector<Texture*>::iterator it = mShieldBar.begin(); it != mShieldBar.end();)
		{
			delete * it;
			it = mShieldBar.erase(it);
		}
		
		for (int i = 0; i < 12; ++i)
		{
			if (i < (int)(12 * ((float)mShield / (mPlayer->GetPerk(3) ? 75.0f : 50.0f))))
				mShieldBar.push_back(new Texture("ShieldSprite.png"));
			else
				mShieldBar.push_back(new Texture("BarSprite.png"));
			mShieldBar.back()->Parent(this);
			mShieldBar.back()->Pos(Vector2(mShieldBar.back()->ScaledDimensions().x * (i + 1) + 5.0f, mHealthText->ScaledDimensions().y * 1.25f + mShieldBar.back()->ScaledDimensions().y));
		}
	}
}

void Interface::CurrentPerks()
{
	if (mPerks.size() > 3)
		return;

	for (int i = 0; i < 4; ++i)
	{
		if (mPlayer->GetPerk(i) && std::find(mPerks.begin(), mPerks.end(), i) == mPerks.end())
		{
			mPerks.push_back(i);

			switch (i)
			{
			case 0:
				mPerksIcon.push_back(new Texture("Map/ReloadPerkSprite.png"));
				break;
			case 1:
				mPerksIcon.push_back(new Texture("Map/DamagePerkSprite.png"));
				break;
			case 2:
				mPerksIcon.push_back(new Texture("Map/MagazinePerkSprite.png"));
				break;
			case 3:
				mPerksIcon.push_back(new Texture("Map/HealthPerkSprite.png"));
				break;
			}

			mPerksIcon.back()->Scale(Vector2(0.25f, 0.25f));
			mPerksIcon.back()->Parent(this);
			mPerksIcon.back()->Pos(Vector2(16.0f + 12.0f * (mPerks.size() - 1), mHealthText->ScaledDimensions().y * 1.25f + mShieldBar.back()->ScaledDimensions().y * 3.0f));
		}
	}

}

void Interface::CurrentAmmo()
{
	if (mGun != mPlayer->WeaponName() || mClip != mPlayer->Clip() || mAmmo != mPlayer->Ammo())
	{
		mGun = mPlayer->WeaponName();
		mClip = mPlayer->Clip();
		mAmmo = mPlayer->Ammo();

		for (std::vector<Texture*>::iterator it = mGunText.begin(); it != mGunText.end();)
		{
			delete * it;
			it = mGunText.erase(it);
		}

		std::string message = mGun + ": " + std::to_string(mClip) + " | " + std::to_string(mAmmo);
		for (int i = 0; i < message.size(); ++i)
		{
			mGunText.push_back(new Texture(std::string(1, message[i]), "1942.ttf", 22, { 224, 224, 224 }));
			mGunText.back()->Parent(this);
			mGunText.back()->Pos(Vector2(25.0f + 14.0f * i, Graphics::SCREEN_HEIGHT - (25.0f * 0.6f)));
		}
	}
}

void Interface::CurrentScore()
{
	if (mPoints != mPlayer->Points() || mKills != mPlayer->Kills())
	{
		mPoints = mPlayer->Points();
		mKills = mPlayer->Kills();

		delete mScoreText;
		mScoreText = new Texture("Points:" + std::to_string(mPoints) + "   Kills:" + std::to_string(mKills), "1942.ttf", 24, { 207, 181, 59 });
		mScoreText->Parent(this);
		mScoreText->Pos(Vector2(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.35f));
	}
}

void Interface::CurrentLevel(int level)
{
	if (mLevel != level)
	{
		mLevel = level;

		delete mLevelText;
		mLevelText = new Texture("Round " + std::to_string(mLevel), "DoubleFeature20.ttf", 32, { 112, 10, 10 });
		mLevelText->Parent(this);
		mLevelText->Pos(Vector2(Graphics::SCREEN_WIDTH - (mLevelText->ScaledDimensions().x * 0.6f), Graphics::SCREEN_HEIGHT - (mLevelText->ScaledDimensions().y * 0.6f)));
	}
}

void Interface::Update()
{
	Pos(Vector2(Graphics::Instance()->Camera()->x, Graphics::Instance()->Camera()->y));

	CurrentHealth();
	CurrentShield();
	CurrentPerks();
	CurrentAmmo();
	CurrentScore();

	mCursor->Pos(InputManager::Instance()->MousePos() + Vector2(Graphics::Instance()->Camera()->x, Graphics::Instance()->Camera()->y));
	//if (InputManager::Instance()->MouseButtonPressed(InputManager::MOUSE_BUTTON::left)) mCursor->FlashRed();
}

void Interface::Render()
{
	mLevelText->Render();
	mHealthText->Render();

	for (const auto& ammo : mGunText)
		ammo->Render();

	for (const auto& bar : mShieldBar)
		bar->Render();

	for (const auto& perk : mPerksIcon)
		perk->Render();

	if (mPlayer->DoublePoints())
		mDoublePointsText->Render();

	if (mInput->KeyDown(SDL_SCANCODE_TAB)) mScoreText->Render();

	mCursor->Render();
}
