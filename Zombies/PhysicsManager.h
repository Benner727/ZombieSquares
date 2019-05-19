#ifndef PHYSICSMANAGER_H
#define PHSYCICSMANAGER_H

#include "PhysEntity.h"
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <algorithm>

class PhysicsManager
{
private:
	static PhysicsManager* sInstance;

	static const int CELL_SIZE = 32;
	static const int WIDTH = 256;

	std::vector<PhysEntity*> mEntity;
	std::vector<PhysEntity*> mRegistered;

	std::bitset<static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers)> mLayerMasks[static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers)];
	
	std::unordered_map<int, std::vector<PhysEntity*>> mCells;

	unsigned long mLastId;

	PhysicsManager();
	~PhysicsManager();

	std::unordered_set<int> Index(PhysEntity* entity);
	void AddIndex(PhysEntity* entity, Vector2 pos, std::unordered_set<int>& set);
	void AddEntity(PhysEntity* entity);

	std::vector<PhysEntity*> CheckNearby(PhysEntity* entity);

public:
	static PhysicsManager* Instance();
	static void Release();

	void SetLayerCollisionMask(PhysEntity::CollisionLayers layer, PhysEntity::CollisionFlags flags);

	unsigned long RegisterEntity(PhysEntity* entity, PhysEntity::CollisionLayers layer);
	void UnregisterEntity(unsigned long id);

	bool SharingCell(PhysEntity* entity);
	bool CheckCollision(PhysEntity* entity);
		
	void Update();
};

#endif