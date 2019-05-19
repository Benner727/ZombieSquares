#include "Tile.h"

#include "BoxCollider.h"

Tile::Tile()
{
	mSprite = nullptr;
	mType = TileType::Empty;
	mGunIndex = -1;
	mHealth = 1;
}

Tile::Tile(Texture * tex, TileType type, int x, int y, int gunIndex)
{
	mSprite = tex;
	mSprite->Parent(this);

	mType = type;

	mGunIndex = gunIndex;

	mHealth = 1;

	Pos(Vector2(x * mTileSize + mTileSize * 0.5f, y * mTileSize + mTileSize * 0.5f));

	AddCollider(new BoxCollider(mSprite->ScaledDimensions()));
}

Tile::~Tile()
{
	delete mSprite;
}

TileType Tile::GetType() const
{
	return mType;
}

void Tile::Update()
{
}

void Tile::Render()
{
	if (mSprite != nullptr)
	{
		mSprite->Render();

		//PhysEntity::Render();
	}
}
