#include "PowerUp.h"

#include "PhysicsManager.h"

PowerUp::PowerUp(Map* map, Vector2 pos)
	: Event(map, pos, Rotation())
{
	mType = rand() % 3;

	switch (mType)
	{
	case 0: //Heals player
		mSprite = new AnimatedTexture("HeartSprite.png", 0, 0, 16, 16, 2, 1.0f, AnimatedTexture::ANIM_DIR::horizontal);
		break;
	case 1: //Gives double points
		mSprite = new AnimatedTexture("CoinSprite.png", 0, 0, 16, 16, 2, 1.0f, AnimatedTexture::ANIM_DIR::horizontal);
		break;
	case 2: //Gives ammo
		mSprite = new AnimatedTexture("BoltSprite.png", 0, 0, 16, 16, 2, 1.0f, AnimatedTexture::ANIM_DIR::horizontal);
		break;
	}
	mSprite->Parent(this);

	mLifeTimer = 25.0f;

	Pos(pos);

	if (map->IsWindow(pos.x / map->mTileSize, pos.y / map->mTileSize))
		AddCollider(new BoxCollider(mSprite->ScaledDimensions() * 2.0f));
	else
		AddCollider(new BoxCollider(mSprite->ScaledDimensions()));

	mCollisionLayer = CollisionLayers::HostileProjectiles;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}

PowerUp::~PowerUp()
{
}

void PowerUp::Hit(PhysEntity * other)
{
	mLifeTimer = 0.0f;
}
