#include "Stalker.h"

#include "PhysicsManager.h"

Stalker::Stalker(Map* map, GameEntity* target)
	: Enemy(map, target)
{
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mSprite = new Texture("StalkerSprite.png");
	mSprite->Parent(this);

	mDirection = -1;
	mRepathTimer = 0.0f;

	mSpeed = 80.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mHealth = 100;
	mDamage = 15;
	mDead = false;

	mStealth = true;
	mStealthTimer = 10.0f;

	AddCollider(new BoxCollider(Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Hostile;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}


Stalker::~Stalker()
{
}

void Stalker::Hit(PhysEntity * other)
{
	if (other == nullptr)
		return;

	if (other->CollisionLayer() == PhysEntity::CollisionLayers::FriendlyProjectiles)
	{
		if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
			return;

		mHitBy.push_back(other->Id());

		mStealth = false;
		mStealthTimer = 0.5f;
		mSprite->FlashRed();

		mHealth -= other->Damage();

		mDead = (mHealth <= 0);

		if (mDead)
			mAudio->PlaySFX("fall.wav", 0, 5);
	}
}

void Stalker::HandleAttack()
{
	if ((Pos() - mTarget->Pos()).Magnitude() <= mSprite->ScaledDimensions().x * 3.0f)
	{
		mStealth = false;
		mStealthTimer = 1.0f;
	}

	mStealthTimer -= mTimer->DeltaTime();

	if (mStealthTimer <= 0.0f)
	{
		mStealth ? mStealthTimer = 1.0f : mStealthTimer = 4.0f;
		mStealth = !mStealth;
	}
}

void Stalker::Render()
{
	if (!mStealth)
		mSprite->Render();

	PhysEntity::Render();
}
