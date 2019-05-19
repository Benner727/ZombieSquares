#include "Explosive.h"

#include "PhysicsManager.h"
#include "Player.h"
#include "Explosion.h"
#include "EventManager.h"

Explosive::Explosive(Map* map, Vector2 pos, float rotation, bool upgraded, GameEntity* user)
	: Projectile(map, pos, rotation, user), mUpgraded(upgraded)
{
	mSprite = new AnimatedTexture("ExplosiveSprite.png", 0, 0, 4, 4, 2, (rand() % 2) ? 0.01 : 0.05f, AnimatedTexture::ANIM_DIR::horizontal);
	mSprite->WrapMode(AnimatedTexture::WRAP_MODE::once);
	mSprite->Parent(this);

	mSpeed = 750.0f;
	mDamage = 150;

	mSprite->Scale(Vector2(1.5f, 1.5f));
	AddCollider(new CircleCollider(mSprite->ScaledDimensions().x * 0.66f));

	Pos(pos);
	Rotation(rotation);

	mCollisionLayer = CollisionLayers::FriendlyProjectiles;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}

Explosive::~Explosive()
{
}

void Explosive::Hit(PhysEntity* other)
{
	Active(false);

	if (other != nullptr)
	{
		if (mUser != nullptr)
		{
			if (dynamic_cast<Player*>(mUser))
				dynamic_cast<Player*>(mUser)->AddPoints(5);
		}
	}

	EventManager::Instance()->AddEvent(new Explosion(mMap, Pos(), Rotation(), mUpgraded));
}