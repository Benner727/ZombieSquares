#include "Explosion.h"

#include "PhysicsManager.h"

Explosion::Explosion(Map* map, Vector2 pos, float rotation, bool upgraded)
	: Event(map, pos, rotation)
{
	mDamage = 950;
	if (upgraded) mDamage *= 2.0f;

	mLifeTimer = 0.4f;

	mSprite = new AnimatedTexture("ExplosionSprite.png", 0, 0, 60, 56, 8, 0.4f, AnimatedTexture::ANIM_DIR::horizontal);
	mSprite->WrapMode(AnimatedTexture::WRAP_MODE::once);
	mSprite->Parent(this);


	mSprite->Scale(Vector2(2.0f, 2.0f));
	AddCollider(new CircleCollider(mSprite->ScaledDimensions().x * 0.5f));

	Pos(pos);
	Rotation(rotation);

	mCollisionLayer = CollisionLayers::FriendlyProjectiles;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);

	mAudio->PlaySFX("explode.wav", 0, 1);
}


Explosion::~Explosion()
{
}

bool Explosion::IgnoreCollisions()
{
	return (mLifeTimer < (0.4f - mTimer->DeltaTime()));
}
