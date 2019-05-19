#include "Map.h"

#include "FOV.h"

Map::Map()
{
	mTimer = Timer::Instance();

	mWidth = 0;
	mHeight = 0;

	mCurrentBox = -1;

	LoadMap();

	mFOV = new FOV(this);
	mFocus = nullptr;
	mVisionTimer = 0.0f;

	Graphics::Instance()->SetLimit(Vector2(mWidth* mTileSize, mHeight* mTileSize));
}

Map::~Map()
{
	for (auto tile : mTiles)
	{
		if (tile.second != nullptr)
			delete tile.second;
	}
	mTiles.clear();

	delete mFOV;
}

void Map::LoadMap()
{
	std::string fullPath = basePath;
	fullPath.append("Assets/Map/map.txt");
	std::ifstream infile(fullPath);
	if (!infile) std::cerr << "map.txt could not be opened!" << std::endl;

	int x = 0;
	int y = 0;

	std::vector<int> guns = { 2, 3, 4, 5, 6 };
	std::vector<int> perks = { 0, 1, 2, 3 };

	std::string line;
	while (std::getline(infile, line))
	{
		//std::istringstream iss(line);

		for (const auto& c : line)
		{
			switch (c)
			{
			case 'S': //Player spawn
				mSpawn.push_back(Vector2(x * mTileSize + mTileSize * 0.5f, y * mTileSize + mTileSize * 0.5f));
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/FloorSprite.png"), TileType::Empty, x, y);
				break;
			case '#': //Wall
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/WallSprite.png"), TileType::Solid, x, y);
				break;
			case '.': //Inside ground
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/FloorSprite.png"), TileType::Empty, x, y);
				break;
			case '~': //Outside ground
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/GroundSprite.png"), TileType::Empty, x, y);
				break;
			case 'D': //Door
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/DoorSprite.png"), TileType::Door, x, y);
				break;
			case 'W': //Window
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/WindowSprite.png"), TileType::Window, x, y);
				break;
			case 'X': //Zombie Spawn
				mZombieSpawns.push_back(Vector2(x * mTileSize + mTileSize * 0.5f, y * mTileSize + mTileSize * 0.5f));
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/HoleSprite.png"), TileType::Empty, x, y);
				break;
			case 'G': //Wall Gun
				if (guns.size() > 0)
				{
					int id = rand() % guns.size();
					mTiles[x + y * mWidth] = new Tile(new Texture("Map/WallGunSprite.png"), TileType::WallGun, x, y, guns[id]);
					guns.erase(guns.begin() + id);
				}
				else
					mTiles[x + y * mWidth] = new Tile(new Texture("Map/WallSprite.png"), TileType::Solid, x, y);
				break;
			case 'B': //Box
				mBoxLocations.push_back(x + y * mWidth);
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/LockedBoxSprite.png"), TileType::Box, x, y);
				break;
			case 'M': //Machine
				mTiles[x + y * mWidth] = new Tile(new Texture("Map/MachineSprite.png"), TileType::Machine, x, y);
				break;
			case 'P': //Perk

				if (perks.size() > 0)
				{
					int id = rand() % perks.size();
					switch (perks[id])
					{
					case 0: //Reload 33% faster
						mTiles[x + y * mWidth] = new Tile(new Texture("Map/ReloadPerkSprite.png"), TileType::Perk, x, y, 0);
						break;
					case 1: //25% increased bullet damage
						mTiles[x + y * mWidth] = new Tile(new Texture("Map/DamagePerkSprite.png"), TileType::Perk, x, y, 1);
						break;
					case 2: //50% larger magazine sizes
						mTiles[x + y * mWidth] = new Tile(new Texture("Map/MagazinePerkSprite.png"), TileType::Perk, x, y, 2);
						break;
					case 3: //25% increased armor and health
						mTiles[x + y * mWidth] = new Tile(new Texture("Map/HealthPerkSprite.png"), TileType::Perk, x, y, 3);
						break;
					}
					perks.erase(perks.begin() + id);
				}
				else
					mTiles[x + y * mWidth] = new Tile(new Texture("Map/WallSprite.png"), TileType::Solid, x, y);
				break;
			}

			x++;

			if (x > mWidth) mWidth = x;
		}

		x = 0;
		y++;

		if (y > mHeight) mHeight = y;
	}

	MoveBox();
}

bool Map::IsSolid(int x, int y)
{
	if (IsBoundary(x, y))
		return true;

	if (mTiles[x + y * mWidth] != nullptr)
		return (mTiles[x + y * mWidth]->GetType() == TileType::Solid || mTiles[x + y * mWidth]->GetType() == TileType::Door
			|| mTiles[x + y * mWidth]->GetType() == TileType::WallGun || mTiles[x + y * mWidth]->GetType() == TileType::Box
			|| mTiles[x + y * mWidth]->GetType() == TileType::Machine || mTiles[x + y * mWidth]->GetType() == TileType::Perk);

	return false;
}

bool Map::IsDoor(int x, int y)
{
	if (IsBoundary(x, y))
		return false;

	if (mTiles[x + y * mWidth] != nullptr)
		return (mTiles[x + y * mWidth]->GetType() == TileType::Door);

	return false;
}

bool Map::IsWindow(int x, int y)
{
	if (IsBoundary(x, y))
		return false;

	if (mTiles[x + y * mWidth] != nullptr)
		return (mTiles[x + y * mWidth]->GetType() == TileType::Window);

	return false;
}

bool Map::IsBox(int x, int y)
{
	if (IsBoundary(x, y))
		return false;

	if (mTiles[x + y * mWidth] != nullptr)
		return (mTiles[x + y * mWidth]->GetType() == TileType::Box) && (x + y * mWidth) == mBoxLocations[mCurrentBox];

	return false;
}

bool Map::IsMachine(int x, int y)
{
	if (IsBoundary(x, y))
		return false;

	if (mTiles[x + y * mWidth] != nullptr)
		return (mTiles[x + y * mWidth]->GetType() == TileType::Machine);

	return false;
}

bool Map::IsEmpty(int x, int y)
{
	if (IsBoundary(x, y))
		return false;

	if (mTiles[x + y * mWidth] != nullptr)
		return (mTiles[x + y * mWidth]->GetType() == TileType::Empty);

	return true;
}

bool Map::IsBoundary(int x, int y)
{
	if (x < 0 || y < 0)
		return true;

	if (x >= mWidth || y >= mHeight)
		return true;

	return false;
}

int Map::IsGun(int x, int y)
{
	if (IsBoundary(x, y))
		return -1;

	if (mTiles[x + y * mWidth] != nullptr)
	{
		if (mTiles[x + y * mWidth]->GetType() == TileType::WallGun)
			return mTiles[x + y * mWidth]->GunIndex();
	}

	return -1;
}

int Map::IsPerk(int x, int y)
{
	if (IsBoundary(x, y))
		return -1;

	if (mTiles[x + y * mWidth] != nullptr)
	{
		if (mTiles[x + y * mWidth]->GetType() == TileType::Perk)
			return mTiles[x + y * mWidth]->GunIndex();
	}

	return -1;
}

bool Map::Collides(PhysEntity * entity, int x, int y)
{
	if (IsBoundary(x, y))
		return true;

	return (mTiles[x + y * mWidth]->CheckCollision(entity));
}

void Map::OpenDoor(int x, int y)
{
	if (IsDoor(x, y))
	{
		delete mTiles[x + y * mWidth];
		mTiles[x + y * mWidth] = new Tile(new Texture("Map/FloorSprite.png"), TileType::Empty, x, y);
	}
}

void Map::MoveBox()
{
	if (mBoxLocations.size())
	{
		int x, y;

		if (mCurrentBox != -1)
		{
			y = mBoxLocations[mCurrentBox] / mWidth;
			x = mBoxLocations[mCurrentBox] - y * mWidth;

			delete mTiles[mBoxLocations[mCurrentBox]];
			mTiles[mBoxLocations[mCurrentBox]] = new Tile(new Texture("Map/LockedBoxSprite.png"), TileType::Box, x, y);
		}

		int prevLocation = mCurrentBox;
		do
		{
			mCurrentBox = rand() % mBoxLocations.size();			
		} while (mCurrentBox == prevLocation && mBoxLocations.size() > 1);

		y = mBoxLocations[mCurrentBox] / mWidth;
		x = mBoxLocations[mCurrentBox] - y * mWidth;

		delete mTiles[mBoxLocations[mCurrentBox]];
		mTiles[mBoxLocations[mCurrentBox]] = new Tile(new Texture("Map/BoxSprite.png"), TileType::Box, x, y);
	}
}

void Map::EmptyPerk(int x, int y)
{
	if (IsPerk(x, y) != -1)
	{
		delete mTiles[x + y * mWidth];
		mTiles[x + y * mWidth] = new Tile(new Texture("Map/EmptyPerkSprite.png"), TileType::Solid, x, y);
	}
}

void Map::SetFocus(GameEntity * focus)
{
	mFocus = focus;
}

bool Map::InFOV(GameEntity * entity)
{
	int x = entity->Pos().x;
	int y = entity->Pos().y;

	if (mFOV->InFOV((int)x / mTileSize, (int)y / mTileSize))
		return true;
	else
	{
		if (!IsSolid((int)(x + mTileSize) / mTileSize, (int)(y + mTileSize) / mTileSize)
			&& mFOV->InFOV((int)(x + mTileSize) / mTileSize, (int)(y + mTileSize) / mTileSize))
			return true;
		if (!IsSolid((int)(x + mTileSize) / mTileSize, (int)(y - mTileSize) / mTileSize)
			&& mFOV->InFOV((int)(x + mTileSize) / mTileSize, (int)(y - mTileSize) / mTileSize))
			return true;
		if (!IsSolid((int)(x - mTileSize) / mTileSize, (int)(y + mTileSize) / mTileSize)
			&& mFOV->InFOV((int)(x - mTileSize) / mTileSize, (int)(y + mTileSize) / mTileSize))
			return true;
		if (!IsSolid((int)(x - mTileSize) / mTileSize, (int)(y - mTileSize) / mTileSize)
			&& mFOV->InFOV((int)(x - mTileSize) / mTileSize, (int)(y - mTileSize) / mTileSize))
			return true;
	}
	
	return false;
}

void Map::Update()
{
	if (mFocus != nullptr)
	{
		if (mVisionTimer <= 0.0f)
		{
			mFOV->calculateFOV(mFocus->Pos().x / mTileSize, mFocus->Pos().y / mTileSize, 21);
			mVisionTimer = 1.0f;
		}
		else
			mVisionTimer -= mTimer->DeltaTime();
	}

	for (auto& tile : mTiles)
	{
		if (tile.second != nullptr)
		{
			tile.second->Update();
		}
	}
}

void Map::Render()
{
	for (const auto& tile : mTiles)
	{
		if (tile.second != nullptr)
		{
			if (mFocus != nullptr)
			{
				if (!mFOV->InFOV(tile.second->Pos().x / mTileSize, tile.second->Pos().y / mTileSize))
					tile.second->Dim();
			}	
				
			tile.second->Render();
		}
	}
}
