#pragma once

#include "Player.h"

class Interface : public GameEntity
{
public:
	Interface(Player* player);
	~Interface();

private:
	InputManager* mInput;

	Player* mPlayer;
	
	Texture* mCursor;

	int mLevel;
	Texture* mLevelText;

	int mHealth;
	Texture* mHealthText;

	int mShield;
	std::vector<Texture*> mShieldBar;

	std::vector<int> mPerks;
	std::vector<Texture*> mPerksIcon;

	std::string mGun;
	int mClip;
	int mAmmo;
	std::vector<Texture*> mGunText;

	int mPoints;
	int mKills;
	Texture* mScoreText;

	Texture* mDoublePointsText;

	void CurrentHealth();
	void CurrentShield();
	void CurrentPerks();
	void CurrentAmmo();
	void CurrentScore();

public:
	void CurrentLevel(int level);

	void Update();
	void Render();
};

