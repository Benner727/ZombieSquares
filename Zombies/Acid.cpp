#include "Acid.h"

#include "PhysicsManager.h"

Acid::Acid(Map* map, Vector2 pos, float rotation)
	: Event(map, pos, rotation)
{
	mSprite = new AnimatedTexture("AcidSprite.png", 0, 0, 24, 24, 2, 0.5f, AnimatedTexture::ANIM_DIR::horizontal);
	mSprite->Parent(this);

	mDamage = 25;

	mLifeTimer = 10.0f;

	AddCollider(new BoxCollider(mSprite->ScaledDimensions()));

	Pos(pos);
	Rotation(rotation);

	mCollisionLayer = CollisionLayers::HostileProjectiles;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}


Acid::~Acid()
{
}

void Acid::Update()
{
	if (Active())
		mSprite->Update();

	int prevLife = (int)mLifeTimer;

	mLifeTimer -= mTimer->DeltaTime();

	if (prevLife > (int)mLifeTimer)
	{
		mSprite->Scale(mSprite->Scale() + Vector2(0.1f, 0.1f));
		RemoveColliders();
		AddCollider(new BoxCollider(mSprite->ScaledDimensions()));
	}

	if (mLifeTimer <= 0.0f)
		Active(false);
}