#pragma once

#include "AnimatedTexture.h"
#include "PhysEntity.h"

enum class TileType { Empty = 0, Solid, Door, Window, WallGun, Box, Machine, Perk, Boundary };

class Tile : public PhysEntity
{
public:
	Tile();
	Tile(Texture* tex, TileType type, int x, int y, int gunIndex = -1);
	~Tile();

	static const int mTileSize = 32;

private:
	Texture* mSprite;

	TileType mType;

	int mGunIndex;

public:
	TileType GetType() const;

	void Dim() { mSprite->Dim(); }

	int GunIndex() const { return mGunIndex; }

	void Update();
	void Render();
};

