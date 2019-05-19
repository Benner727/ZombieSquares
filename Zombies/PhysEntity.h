#ifndef PHYSENTITY_H
#define PHYSENTITY_H

#include "Collider.h"
#include <vector>

class PhysEntity : public GameEntity
{
public:
	enum class CollisionLayers
	{
		Friendly = 0,
		FriendlyProjectiles,
		Hostile,
		HostileProjectiles,
		PowerUp,

		MaxLayers
	};

	enum class CollisionFlags
	{
		None = 0x00,
		Friendly = 0x01,
		FriendlyProjectiles = 0x02,
		Hostile = 0x04,
		HostileProjectiles = 0x08,
		PowerUp = 0x10
	};

protected:
	unsigned long mId;
	CollisionLayers mCollisionLayer;

	std::vector<Collider*> mColliders;

	Collider* mBroadPhaseCollider;

	std::vector<unsigned long> mHitBy;

	int mHealth;
	int mDamage;

	void AddCollider(Collider* collider, Vector2 localPos = VEC2_ZERO);
	void RemoveColliders();

	virtual bool IgnoreCollisions();

public:
	PhysEntity();
	virtual ~PhysEntity();

	bool operator < (const PhysEntity& entity) const
	{
		return (this->mId < entity.mId);
	}

	unsigned long Id();
	CollisionLayers CollisionLayer();

	int Damage();

	Collider* BroadPhaseCollider();

	bool CheckCollision(PhysEntity* other);
	bool HitBy(unsigned long id) const;

	virtual void Hit(PhysEntity* other);

	virtual void Render();
};

inline PhysEntity::CollisionFlags operator|(PhysEntity::CollisionFlags a, PhysEntity::CollisionFlags b)
{
	return static_cast<PhysEntity::CollisionFlags>(static_cast<unsigned int> (a) | static_cast<unsigned int> (b));
}

inline PhysEntity::CollisionFlags operator&(PhysEntity::CollisionFlags a, PhysEntity::CollisionFlags b)
{
	return static_cast<PhysEntity::CollisionFlags>(static_cast<unsigned int> (a) & static_cast<unsigned int> (b));
}

#endif