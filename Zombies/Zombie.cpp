#include "Zombie.h"

#include "PhysicsManager.h"

Zombie::Zombie(Map* map, GameEntity* target)
	: Enemy(map, target)
{
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mSprite = new Texture("ZombieSprite.png");
	mSprite->Parent(this);

	mDirection = -1;
	mRepathTimer = 0.0f;

	mSpeed = 50.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mHealth = 100;
	mDamage = 12;
	mAngry = false;
	mDead = false;

	AddCollider(new BoxCollider(Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Hostile;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}

Zombie::~Zombie()
{
}

void Zombie::Hit(PhysEntity * other)
{
	if (other == nullptr)
		return;

	if (other->CollisionLayer() == PhysEntity::CollisionLayers::FriendlyProjectiles)
	{
		if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
			return;

		mHitBy.push_back(other->Id());

		mSprite->FlashRed();

		mHealth -= other->Damage();

		if (!mAngry)
		{
			if (rand() % 2)
			{
				mAngry = true;
				mSpeed *= 1.5f;
			}
		}

		mDead = (mHealth <= 0);

		if (mDead)
			mAudio->PlaySFX("fall.wav", 0, 5);
	}
}