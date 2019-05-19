#include "Bullet.h"

#include "PhysicsManager.h"

Bullet::Bullet(Map* map, Vector2 pos, float rotation, GameEntity* user, bool shell)
	: Projectile(map, pos, rotation, user, shell)
{
	mSprite = new AnimatedTexture("BulletSprite.png", 0, 0, 4, 4, 2, (rand() % 2) ? 0.01 : 0.05f, AnimatedTexture::ANIM_DIR::horizontal);
	mSprite->WrapMode(AnimatedTexture::WRAP_MODE::once);
	mSprite->Parent(this);

	mSpeed = 750.0f;
	mDamage = 25;

	AddCollider(new CircleCollider(mSprite->ScaledDimensions().x * 0.5f));

	Pos(pos);
	Rotation(rotation);

	mCollisionLayer = CollisionLayers::FriendlyProjectiles;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);

}

Bullet::~Bullet()
{
}

void Bullet::Damage(int damage)
{
	mDamage = damage;
}

void Bullet::Penetrates(bool penetrates)
{
	mPenetrates = penetrates;
}
