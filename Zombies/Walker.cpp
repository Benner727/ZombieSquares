#include "Walker.h"

#include "PhysicsManager.h"
#include "EventManager.h"
#include "Acid.h"

Walker::Walker(Map* map, GameEntity* target)
	: Enemy(map, target)
{
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mSprite = new Texture("WalkerSprite.png");
	mSprite->Parent(this);

	mDirection = -1;
	mRepathTimer = 0.0f;

	mSpeed = 40.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mHealth = 200;
	mDamage = 20;
	mDead = false;

	mAttackTimer = 6.0f;

	AddCollider(new BoxCollider(Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Hostile;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}


Walker::~Walker()
{
}

void Walker::HandleMovement()
{
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	if (mTarget == nullptr)
		return;

	if ((Pos() - mTarget->Pos()).Magnitude() <= mSprite->ScaledDimensions().x)
		return; //Attack

	if (mAttackTimer >= 5.0f)
		return;

	HandlePathing();
}

void Walker::HandleAttack()
{
	if (mTarget == nullptr)
		return;

	if (mAttackTimer > 0.0f)
	{
		mAttackTimer -= mTimer->DeltaTime();
		return;
	}

	if (mHasLOS && (Pos() - mTarget->Pos()).Magnitude() <= mSprite->ScaledDimensions().x * 5.0f)
	{
		Vector2 direction = (mTarget->Pos() - Pos(GameEntity::world)).Normalize();
		Vector2 newPos = Pos() + (direction * mMap->mTileSize);

		if (!mMap->IsEmpty(newPos.x / mMap->mTileSize, newPos.y / mMap->mTileSize))
			return;

		EventManager::Instance()->AddEvent(new Acid(mMap, newPos, Rotation()));
		mAttackTimer = 6.0f;

		mAudio->PlaySFX("zombieDeath.wav", 0, 4);
	}
}