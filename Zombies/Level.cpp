#include "Level.h"

#include "Zombie.h"
#include "Walker.h"
#include "Stalker.h"
#include "Charger.h"

std::mt19937 gen(time(nullptr));
std::uniform_int_distribution<int> dist(7, 26);
std::uniform_int_distribution<int> newSpawn(1, 3);
std::uniform_real_distribution<float> spawnUpdate(0.1, 0.5);

Level::Level()
{
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();

	mEvents = EventManager::Instance();
	mProjectiles = ProjectileManager::Instance();

	mLevel = 1;
	
	mSpawnsLeft = 5;
	mSpawnTimer = 0.0f;
	mLastSpawn = -1;

	mCurrentSpawn = 0;
	mSpawnUpdateTimer = 0.0f;

	mNearDoor = false;
	mNearMachine = false;
	mDoorText = new Texture("Press F to open door. (Cost 1500)", "1942.ttf", 24, { 255, 255, 240 });

	mNearBuyable = false;
	mBuyText = nullptr;

	mNewGameTimer = 2.0f;
	mNewGameText = nullptr;

	mNearBox = false;
	mBoxText = nullptr;
	mCooldownTimer = 0.0f;
	mBoxTimer = 0.0f;
	mGunIndex = -1;

	mNearMachine = false;
	mMachineText = new Texture("Press F upgrade gun. (Cost 15000)", "1942.ttf", 24, { 255, 255, 240 });

	mNearPerk = false;
	mPerkText = nullptr;

	mGameOverTimer = 3.0f;
	mGameOverText = new Texture("You died!", "1942.ttf", 48, { 112, 10, 10 });
	mGameOver = false;

	mMap = new Map();
	mPlayer = new Player(mMap);
	mPlayer->Pos(mMap->Spawn());

	mMap->SetFocus(mPlayer);

	InitPlayer();

	mInterface = new Interface(mPlayer);

	//Set active spawns for starting round.
	for (int i = 0; i < mMap->ZombieSpawns().size(); ++i)
	{
		mSpawnUpdateTimer = 0.0f;
		UpdateSpawns();
	}
}

Level::~Level()
{
	mEvents->Release();
	mProjectiles->Release();

	delete mDoorText;
	delete mBuyText;
	delete mBoxText;
	delete mPerkText;
	delete mMachineText;

	delete mNewGameText;
	delete mGameOverText;

	delete mMap;
	delete mPlayer;

	for (std::vector<Enemy*>::iterator it = mEnemies.begin(); it != mEnemies.end(); ++it)
		delete * it;

	for (std::vector<Body*>::iterator it = mBodies.begin(); it != mBodies.end(); ++it)
		delete * it;

	delete mInterface;

	delete mWeaponLoader;
}

void Level::InitPlayer()
{
	mWeaponLoader = new WeaponLoader(mMap);
	mPlayer->GiveWeapon(mWeaponLoader->GetWeapon(1));
	//mPlayer->GiveWeapon(mWeaponLoader->GetWeapon(rand() % 5 + 2));
}

void Level::UpdateSpawns()
{
	//Update spawns 10 times every second
	if (mSpawnUpdateTimer > 0.0f)
	{
		mSpawnUpdateTimer -= mTimer->DeltaTime();
		return;
	}
	else
		mSpawnUpdateTimer = spawnUpdate(gen);

	//Erase current spawn
	mActiveSpawns.erase(std::remove(mActiveSpawns.begin(), mActiveSpawns.end(), mMap->ZombieSpawns()[mCurrentSpawn]), mActiveSpawns.end());

	//Check to see if current spawn is in range
	PathFinder path(mMap);
	if (path.getCost(mPlayer->Pos(), mMap->ZombieSpawns()[mCurrentSpawn]) < 350)
		mActiveSpawns.push_back(mMap->ZombieSpawns()[mCurrentSpawn]);

	//Increment and loop current spawn spot
	mCurrentSpawn++;
	if (mCurrentSpawn >= mMap->ZombieSpawns().size())
		mCurrentSpawn = 0;
}

void Level::HandleSpawning()
{
	if (mSpawnsLeft > 0)
	{
		UpdateSpawns();

		if (mEnemies.size() < mLevel && mSpawnTimer > 1.0f)
			mSpawnTimer = newSpawn(gen);

		if (mSpawnTimer > 0.0f)
		{
			mSpawnTimer -= mTimer->DeltaTime();
			return;
		}
		else
			mSpawnTimer = newSpawn(gen) * 4.0f;

		if (mEnemies.size() < 28)
		{
			int spawnType = rand() % 100 + 1;
			int spawnChance[4] = { 100, 0, 0, 0 };
			
			if (mLevel > 4)
			{
				spawnChance[0] = 85;
				spawnChance[1] = 100;
				spawnChance[2] = 0;
				spawnChance[3] = 0;
			}
			if (mLevel > 9)
			{
				spawnChance[0] = 70;
				spawnChance[1] = 90;
				spawnChance[2] = 100;
				spawnChance[3] = 0;
			}
			if (mLevel > 14)
			{
				spawnChance[0] = 60;
				spawnChance[1] = 85;
				spawnChance[2] = 95;
				spawnChance[3] = 100;
			}
			if (mLevel > 19)
			{
				spawnChance[0] = 50;
				spawnChance[1] = 70;
				spawnChance[2] = 90;
				spawnChance[3] = 100;
			}

			//Choose any random spawn if there aren't atleast two available
			if (mActiveSpawns.size() < 2)
				mActiveSpawns = mMap->ZombieSpawns();

			Vector2 spawnSpot;
			do
			{
				spawnSpot = mActiveSpawns[rand() % mActiveSpawns.size()];
			} while (mLastSpawn == spawnSpot);

			if (spawnType <= spawnChance[0])
			{
				mEnemies.push_back(new Zombie(mMap, mPlayer));
				mEnemies.back()->Health(BASE_HEALTH + (HEALTH_SCALE * (mLevel / ROUND_SCALE)));
				mEnemies.back()->Pos(spawnSpot);
			}
			else if (spawnType <= spawnChance[1])
			{
				mEnemies.push_back(new Walker(mMap, mPlayer));
				mEnemies.back()->Health((BASE_HEALTH + (HEALTH_SCALE * (mLevel / ROUND_SCALE))) * 3.0f);
				mEnemies.back()->Pos(spawnSpot);
			}
			else if (spawnType <= spawnChance[2])
			{
				mEnemies.push_back(new Stalker(mMap, mPlayer));
				mEnemies.back()->Health((BASE_HEALTH + (HEALTH_SCALE * (mLevel / ROUND_SCALE))) * 0.75f);
				mEnemies.back()->Pos(spawnSpot);
			}
			else if (spawnType <= spawnChance[3])
			{
				mEnemies.push_back(new Charger(mMap, mPlayer));
				mEnemies.back()->Health((BASE_HEALTH + (HEALTH_SCALE * (mLevel / ROUND_SCALE))) * 7.0f);
				mEnemies.back()->Pos(spawnSpot);
			}

			mSpawnsLeft--;
		}
	}
}

void Level::HandleEnemies()
{
	HandleSpawning();

	for (std::vector<Enemy*>::iterator it = mEnemies.begin(); it != mEnemies.end();)
	{
		if ((*it)->Dead())
		{
			if (dynamic_cast<Zombie*>(*it))
				mPlayer->AddPoints(100);
			else if (dynamic_cast<Walker*>(*it))
				mPlayer->AddPoints(225);
			else if (dynamic_cast<Stalker*>(*it))
				mPlayer->AddPoints(175);
			else if (dynamic_cast<Charger*>(*it))
				mPlayer->AddPoints(500);

			mPlayer->AddKill();

			mBodies.push_back(new Body((*it)->Pos()));

			delete * it;
			it = mEnemies.erase(it);
		}
		else
		{
			(*it)->Update();
			++it;
		}
	}

	for (std::vector<Body*>::iterator it = mBodies.begin(); it != mBodies.end();)
	{
		if ((*it)->Dead())
		{
			delete * it;
			it = mBodies.erase(it);
		}
		else
		{
			(*it)->Update();
			++it;
		}
	}
}

void Level::HandleDoor()
{
	int x = mPlayer->Pos().x / mMap->mTileSize;
	int y = mPlayer->Pos().y / mMap->mTileSize;

	mNearDoor = false;

	if (mMap->IsDoor(x + 1, y) || mMap->IsDoor(x, y + 1)
		|| mMap->IsDoor(x - 1, y) || mMap->IsDoor(x, y - 1)
		|| mMap->IsDoor(x + 1, y + 1) || mMap->IsDoor(x + 1, y - 1)
		|| mMap->IsDoor(x - 1, y + 1) || mMap->IsDoor(x - 1, y - 1))
	{
		mNearDoor = true;
		mDoorText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.8f));
	}

	if (mNearDoor && mInput->KeyPressed(SDL_SCANCODE_F))
	{
		if (mPlayer->Points() >= 1500)
		{
			mPlayer->AddPoints(-1500);

			mMap->OpenDoor(x + 1, y); mMap->OpenDoor(x, y + 1);
			mMap->OpenDoor(x - 1, y); mMap->OpenDoor(x, y - 1);
			mMap->OpenDoor(x + 1, y + 1); mMap->OpenDoor(x + 1, y - 1);
			mMap->OpenDoor(x - 1, y + 1);  mMap->OpenDoor(x - 1, y - 1);
		}
	}
}

void Level::HandleBuying()
{
	int x = mPlayer->Pos().x / mMap->mTileSize;
	int y = mPlayer->Pos().y / mMap->mTileSize;

	mNearBuyable = false;

	delete mBuyText;
	mBuyText = nullptr;

	std::vector<std::pair<int, int>> dir = { {0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1} };
	int gunIndex = -1;
	for (const auto& p : dir)
	{
		if (mMap->IsGun(x + p.first, y + p.second) != -1)
		{
			gunIndex = mMap->IsGun(x + p.first, y + p.second);
			mNearBuyable = true;
			break;
		}
	}

	if (!mNearBuyable || gunIndex == -1)
		return;
	else if (mWeaponLoader->GetWeapon(gunIndex) == nullptr)
		return;

	if (mPlayer->HasGun(gunIndex))
	{
		mBuyText = new Texture("Press F to buy ammo. (Cost 650)", "1942.ttf", 24, { 255, 255, 240 });

		if (mInput->KeyPressed(SDL_SCANCODE_F))
		{
			if (mPlayer->Points() >= 650)
			{
				mPlayer->AddPoints(-650);
				mPlayer->GiveAmmo(gunIndex);
			}
		}
	}
	else
	{
		std::string message = "Press F to purchase ";
		message.append(mWeaponLoader->GetWeapon(gunIndex)->Name());
		mBuyText = new Texture(message + ". (Cost 1200)", "1942.ttf", 24, { 255, 255, 240 });

		if (mInput->KeyPressed(SDL_SCANCODE_F))
		{
			if (mPlayer->Points() >= 1200)
			{
				mPlayer->AddPoints(-1200);
				mPlayer->GiveWeapon(mWeaponLoader->GetWeapon(gunIndex));
			}
		}
	}

	if (mBuyText != nullptr) mBuyText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.8f));
}

void Level::HandleBox()
{
	mNearBox = false;
	delete mBoxText;
	mBoxText = nullptr;

	if (mCooldownTimer > 0.0f && mBoxTimer <= 0.0f)
	{
		mCooldownTimer -= mTimer->DeltaTime();

		if (mCooldownTimer <= 0.0f && !(rand() % 5))
			mMap->MoveBox();

		return;
	}
	
	if (mBoxTimer > 0.0f)
		mBoxTimer -= mTimer->DeltaTime();

	int x = mPlayer->Pos().x / mMap->mTileSize;
	int y = mPlayer->Pos().y / mMap->mTileSize;

	std::vector<std::pair<int, int>> dir = { {0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1} };
	for (const auto& p : dir)
	{
		if (mMap->IsBox(x + p.first, y + p.second))
		{
			mNearBox = true;
			break;
		}
	}

	if (!mNearBox)
		return;

	if (mBoxTimer <= 0.0f)
	{
		mBoxText = new Texture("Press F to open random box. (Cost 5000)", "1942.ttf", 24, { 255, 255, 240 });

		if (mInput->KeyPressed(SDL_SCANCODE_F))
		{
			if (mPlayer->Points() >= 5000)
			{
				mPlayer->AddPoints(-5000);
				
				do
				{
					mGunIndex = dist(gen);
				} while (mPlayer->HasGun(mGunIndex));

				mBoxTimer = 10.0f;
				mCooldownTimer = 3.0f;
			}
		}
	}
	else
	{
		mBoxText = new Texture("Press F to take " + mWeaponLoader->GetWeapon(mGunIndex)->Name() + ".", "1942.ttf", 24, { 255, 255, 240 });

		if (mInput->KeyPressed(SDL_SCANCODE_F))
		{
			mPlayer->GiveWeapon(mWeaponLoader->GetWeapon(mGunIndex));
			mBoxTimer = 0.0f;
		}
	}

	if (mBoxText != nullptr) mBoxText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.8f));
}

void Level::HandlePerks()
{
	int x = mPlayer->Pos().x / mMap->mTileSize;
	int y = mPlayer->Pos().y / mMap->mTileSize;

	mNearPerk = false;

	delete mPerkText;
	mPerkText = nullptr;

	std::vector<std::pair<int, int>> dir = { {0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1} };
	int perkIndex = -1;
	for (const auto& p : dir)
	{
		if (mMap->IsPerk(x + p.first, y + p.second) != -1)
		{
			perkIndex = mMap->IsPerk(x + p.first, y + p.second);
			mNearPerk = true;
			break;
		}
	}

	if (!mNearPerk || perkIndex == -1)
		return;
	else if (mPlayer->GetPerk(perkIndex))
		return;

	std::string perkName = "";
	switch (perkIndex)
	{
	case 0:
		perkName = "Tactical Reload";
		break;
	case 1:
		perkName = "Hollow Point Rounds";
		break;
	case 2:
		perkName = "High Capacity Mags";
		break;
	case 3:
		perkName = "Tenacity";
		break;
	}
	
	mPerkText = new Texture("Press F for " + perkName + ". (Cost 10000)", "1942.ttf", 24, { 255, 255, 240 });
	mPerkText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.8f));

	if (mNearPerk && mInput->KeyPressed(SDL_SCANCODE_F))
	{
		if (mPlayer->Points() >= 10000)
		{
			mPlayer->AddPoints(-10000);

			mMap->EmptyPerk(x + 1, y); mMap->EmptyPerk(x, y + 1);
			mMap->EmptyPerk(x - 1, y); mMap->EmptyPerk(x, y - 1);
			mMap->EmptyPerk(x + 1, y + 1); mMap->EmptyPerk(x + 1, y - 1);
			mMap->EmptyPerk(x - 1, y + 1);  mMap->EmptyPerk(x - 1, y - 1);

			mPlayer->SetPerk(perkIndex, true);
		}
	}
}

void Level::HandleMachine()
{
	int x = mPlayer->Pos().x / mMap->mTileSize;
	int y = mPlayer->Pos().y / mMap->mTileSize;

	mNearMachine = false;

	if (mPlayer->GetWeapon()->Upgraded())
		return;

	if (mMap->IsMachine(x + 1, y) || mMap->IsMachine(x, y + 1)
		|| mMap->IsMachine(x - 1, y) || mMap->IsMachine(x, y - 1)
		|| mMap->IsMachine(x + 1, y + 1) || mMap->IsMachine(x + 1, y - 1)
		|| mMap->IsMachine(x - 1, y + 1) || mMap->IsMachine(x - 1, y - 1))
	{
		mNearMachine = true;
		mMachineText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.8f));
	}

	if (mNearMachine && mInput->KeyPressed(SDL_SCANCODE_F))
	{
		if (mPlayer->Points() >= 15000)
		{
			mPlayer->AddPoints(-15000);
			
			mPlayer->GetWeapon()->Upgrade();
		}
	}
}

void Level::StartRound()
{
	//Start new round
	mSpawnsLeft = 5 + 3 * mLevel;
	
	mLevel++;
	mInterface->CurrentLevel(mLevel);
}

void Level::Update()
{
	mMap->Update();

	HandleEnemies();

	if (!mPlayer->Dead())
		mPlayer->Update();
	else
	{
		mGameOverTimer -= mTimer->DeltaTime();

		mGameOverText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.4f));

		if (mGameOverTimer <= 0.0f)
			mGameOver = true;
	}

	mProjectiles->Update();
	mEvents->Update();

	HandleDoor();
	HandleBuying();
	HandleBox();
	HandlePerks();
	HandleMachine();

	mInterface->Update();

	if (mNewGameTimer > 0.0f)
	{
		mNewGameTimer -= mTimer->DeltaTime();

		if (mNewGameText == nullptr)
		{
			mNewGameText = new Texture("Survive!", "1942.ttf", 48, { 112, 10, 10 });
			mNewGameText->Pos(Vector2(Graphics::Instance()->Camera()->x + Graphics::SCREEN_WIDTH * 0.5f, Graphics::Instance()->Camera()->y + Graphics::SCREEN_HEIGHT * 0.4f));
		}
	}
}

void Level::Render()
{
	mMap->Render();

	for (std::vector<Body*>::iterator it = mBodies.begin(); it != mBodies.end(); ++it)
	{
		if (mMap->InFOV(*it))
			(*it)->Render();
	}

	mEvents->Render();

	if (!mPlayer->Dead())
		mPlayer->Render();
	else
		mGameOverText->Render();

	if (mNewGameTimer > 0.0f)
	{
		if (mNewGameText) 
			mNewGameText->Render();
	}

	for (std::vector<Enemy*>::iterator it = mEnemies.begin(); it != mEnemies.end(); ++it)
	{
		if (mMap->InFOV(*it))
			(*it)->Render();
	}

	mProjectiles->Render();

	mInterface->Render();

	if (mNearDoor)
		mDoorText->Render();
	else if (mNearBuyable && mBuyText)
		mBuyText->Render();
	else if (mNearBox && mBoxText)
		mBoxText->Render();
	else if (mNearPerk && mPerkText)
		mPerkText->Render();
	else if (mNearMachine)
		mMachineText->Render();
}
