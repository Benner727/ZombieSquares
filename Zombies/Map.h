#pragma once

#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "Tile.h"

class FOV;

class Map
{
public:
	Map();
	~Map();

	static const int mTileSize = 32;

private:
	Timer* mTimer;

	FOV* mFOV;
	GameEntity* mFocus;
	float mVisionTimer;

protected:
	int mWidth, mHeight;
	std::map<int, Tile*> mTiles;

	std::vector<Vector2> mSpawn;
	std::vector<Vector2> mZombieSpawns;
	std::vector<int> mBoxLocations;

	int mCurrentBox;

	void LoadMap();

public:
	bool IsSolid(int x, int y);
	bool IsDoor(int x, int y);
	bool IsWindow(int x, int y);
	bool IsBox(int x, int y);
	bool IsMachine(int x, int y);
	bool IsEmpty(int x, int y);
	bool IsBoundary(int x, int y);

	int IsGun(int x, int y);
	int IsPerk(int x, int y);

	bool Collides(PhysEntity* entity, int x, int y);

	void OpenDoor(int x, int y);
	void MoveBox();
	void EmptyPerk(int x, int y);

	int Width() const { return mWidth; }
	int Height() const { return mHeight; }

	Vector2 Spawn() const { return mSpawn[rand() % mSpawn.size()]; }
	std::vector<Vector2> ZombieSpawns() const { return mZombieSpawns; }

	void SetFocus(GameEntity* focus);
	bool InFOV(GameEntity* entity);

	void Update();
	void Render();
};