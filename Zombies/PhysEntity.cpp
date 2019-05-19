#include "PhysEntity.h"
#include "PhysicsHelper.h"
#include "PhysicsManager.h"

PhysEntity::PhysEntity()
{
	mBroadPhaseCollider = nullptr;
	mId = 0;
	mCollisionLayer = CollisionLayers::MaxLayers;

	mHealth = 0;
	mDamage = 0;

	mHitBy.clear();
}

PhysEntity::~PhysEntity()
{
	if (mId != 0)
	{
		PhysicsManager::Instance()->UnregisterEntity(mId);
	}

	for (int i = 0; i < mColliders.size(); i++)
	{
		delete mColliders[i];
		mColliders[i] = nullptr;
	}
	mColliders.clear();

	delete mBroadPhaseCollider;
	mBroadPhaseCollider = nullptr;
}

unsigned long PhysEntity::Id()
{
	return mId;
}

PhysEntity::CollisionLayers PhysEntity::CollisionLayer()
{
	return mCollisionLayer;
}

int PhysEntity::Damage()
{
	return mDamage;
}

Collider* PhysEntity::BroadPhaseCollider()
{
	return (mBroadPhaseCollider != nullptr) ? mBroadPhaseCollider : mColliders[0];
}

bool PhysEntity::CheckCollision(PhysEntity* other)
{
	if (IgnoreCollisions() || other->IgnoreCollisions())
		return false;

	if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
		return false;

	bool narrowPhaseCheck = false;
	if (mBroadPhaseCollider && other->mBroadPhaseCollider)
	{
		narrowPhaseCheck = (ColliderColliderCheck(mBroadPhaseCollider, other->mBroadPhaseCollider));
	}
	else
		narrowPhaseCheck = true;

	if (narrowPhaseCheck)
	{
		for (int i = 0; i < mColliders.size(); i++)
		{
			for (int j = 0; j < other->mColliders.size(); j++)
			{
				if (ColliderColliderCheck(mColliders[i], other->mColliders[j]))
					return true;
			}
		}
	}

	return false;
}

bool PhysEntity::HitBy(unsigned long id) const
{
	if (std::find(mHitBy.begin(), mHitBy.end(), id) != mHitBy.end())
			return true;

	return false;
}

void PhysEntity::Hit(PhysEntity* other)
{

}

bool PhysEntity::IgnoreCollisions()
{
	return false;
}

void PhysEntity::AddCollider(Collider* collider, Vector2 localPos)
{
	collider->Parent(this);
	collider->Pos(localPos);
	mColliders.push_back(collider);

	if (mColliders.size() > 1 || mColliders[0]->GetType() != Collider::ColliderType::Circle)
	{
		float furthestDist = mColliders[0]->GetFurthestPoint().Magnitude();
		float dist = 0.0f;

		for (int i = 1; i < mColliders.size(); i++)
		{
			dist = mColliders[i]->GetFurthestPoint().Magnitude();

			if (dist > furthestDist)
				furthestDist = dist;
		}

		delete mBroadPhaseCollider;
		mBroadPhaseCollider = new CircleCollider(furthestDist, true);
		mBroadPhaseCollider->Parent(this);
		mBroadPhaseCollider->Pos(VEC2_ZERO);
	}
}

void PhysEntity::RemoveColliders()
{
	for (int i = 0; i < mColliders.size(); i++)
	{
		delete mColliders[i];
		mColliders[i] = nullptr;
	}
	mColliders.clear();

	delete mBroadPhaseCollider;
	mBroadPhaseCollider = nullptr;
}

void PhysEntity::Render()
{
	for (int i = 0; i < mColliders.size(); i++)
		mColliders[i]->Render();

	if (mBroadPhaseCollider)
		mBroadPhaseCollider->Render();
}