#pragma once

#include "Interface.h"
#include "Enemy.h"
#include "WeaponLoader.h"
#include "EventManager.h"

static const int BASE_HEALTH = 125;
static const int HEALTH_SCALE = 35;
static const int ROUND_SCALE = 5;

class Body : public GameEntity
{
public:
	Body(Vector2 pos) {
		mTimer = Timer::Instance();

		mSprite = new Texture("CorpseSprite.png");
		mSprite->Parent(this);

		Pos(pos);

		mLifeTimer = 15.0f;
	}
	~Body() {
		delete mSprite;
	}

private:
	Timer* mTimer;

	Texture* mSprite;
	float mLifeTimer;

public:
	bool Dead() { return mLifeTimer <= 0.0f; }

	void Update() { mLifeTimer -= mTimer->DeltaTime(); }
	void Render() { mSprite->Render(); }
};

class Level : public GameEntity
{
public:
	Level();
	~Level();

private:
	Timer* mTimer;
	InputManager* mInput;

	Map* mMap;

	Player* mPlayer;
	std::vector<Enemy*> mEnemies;	
	std::vector<Body*> mBodies;

	EventManager* mEvents;
	ProjectileManager* mProjectiles;

	Interface* mInterface;

	WeaponLoader* mWeaponLoader;

	int mLevel;

	int mSpawnsLeft;
	float mSpawnTimer;
	Vector2 mLastSpawn;

	int mCurrentSpawn;
	float mSpawnUpdateTimer;
	std::vector<Vector2> mActiveSpawns;

	bool mNearDoor;
	Texture* mDoorText;

	bool mNearBuyable;
	Texture* mBuyText;

	bool mNearBox;
	Texture* mBoxText;
	float mCooldownTimer;
	float mBoxTimer;
	int mGunIndex;

	bool mNearMachine;
	Texture* mMachineText;

	bool mNearPerk;
	Texture* mPerkText;

	float mNewGameTimer;
	Texture* mNewGameText;

	float mGameOverTimer;
	Texture* mGameOverText;
	bool mGameOver;

	void InitPlayer();
	void UpdateSpawns();
	void HandleSpawning();
	void HandleEnemies();
	void HandleDoor();
	void HandleBuying();
	void HandleBox();
	void HandlePerks();
	void HandleMachine();

public:
	void StartRound();
	bool RoundOver() const { return !(mEnemies.size() || mSpawnsLeft); }

	bool GameOver() const { return mGameOver; }

	void Update();
	void Render();
};