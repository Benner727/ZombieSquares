#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::sInstance = nullptr;

PhysicsManager* PhysicsManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new PhysicsManager();

	return sInstance;
}

void PhysicsManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

void PhysicsManager::SetLayerCollisionMask(PhysEntity::CollisionLayers layer, PhysEntity::CollisionFlags flags)
{
	mLayerMasks[static_cast<unsigned int>(layer)] = std::bitset <static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers)>(static_cast<unsigned int>(flags));
}

std::unordered_set<int> PhysicsManager::Index(PhysEntity* entity)
{
	std::unordered_set<int> index;

	float radius = entity->BroadPhaseCollider()->GetFurthestPoint().Magnitude();
	Vector2 min = Vector2(floor(entity->Pos().x - radius), floor(entity->Pos().y - radius));
	Vector2 max = Vector2(ceil(entity->Pos().x + radius), ceil(entity->Pos().y + radius));

	for (int x = min.x; ; x += CELL_SIZE)
	{
		x = std::min(x, (int)max.x);
		for (int y = min.y; ; y += CELL_SIZE)
		{
			y = std::min(y, (int)max.y);

			AddIndex(entity, Vector2(x, y), index);

			if (y >= (int)max.y)
				break;
		}

		if (x >= (int)max.x)
			break;
	}
	return index;
}

void PhysicsManager::AddIndex(PhysEntity* entity, Vector2 pos, std::unordered_set<int>& set)
{
	int cellPos = std::max(0, (int)floor(pos.x / CELL_SIZE) + (int)floor(pos.y / CELL_SIZE) * WIDTH);

	set.insert(cellPos);
}

void PhysicsManager::AddEntity(PhysEntity* entity)
{
	std::unordered_set<int> index = Index(entity);

	for (const auto& i : index)
		mCells[i].push_back(entity);
}

unsigned long PhysicsManager::RegisterEntity(PhysEntity* entity, PhysEntity::CollisionLayers layer)
{
	mRegistered.push_back(entity);

	mLastId++;

	return mLastId;
}

void PhysicsManager::UnregisterEntity(unsigned long id)
{
	bool found = false;
	for (int i = 0; i < mEntity.size() && !found; i++)
	{
		if (mEntity[i]->Id() == id)
		{
			std::unordered_set<int> index = Index(mEntity[i]);
			for (const auto& i : index)
			{
				for (int j = 0; j < mCells[i].size(); j++)
				{
					if (mCells[i][j]->Id() == id)
					{
						mCells[i].erase(mCells[i].begin() + j);
						continue;
					}
				}
			}

			mEntity.erase(mEntity.begin() + i);
			found = true;
		}
	}
}

PhysicsManager::PhysicsManager()
{
	mLastId = 0;

	for (unsigned int i = 0; i < static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers); i++)
	{
		mLayerMasks[i] = std::bitset <static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers)>(static_cast<unsigned int>(PhysEntity::CollisionFlags::None));
	}
}

PhysicsManager::~PhysicsManager()
{
}

std::vector<PhysEntity*> PhysicsManager::CheckNearby(PhysEntity* entity)
{
	std::vector<PhysEntity*> other;
	std::unordered_set<int> index = Index(entity);

	for (const auto& i : index)
	{
		other.insert(other.begin(), mCells[i].begin(), mCells[i].end());
	}

	std::sort(other.begin(), other.end());
	other.erase(std::unique(other.begin(), other.end()), other.end());
	other.erase(std::remove(other.begin(), other.end(), entity), other.end());

	return other;
}

bool PhysicsManager::SharingCell(PhysEntity * entity)
{
	for (auto& other : CheckNearby(entity))
	{
		if (mLayerMasks[static_cast<unsigned int>(entity->CollisionLayer())].test(static_cast<unsigned int>(other->CollisionLayer())))
		{
			if (!other->HitBy(entity->Id()))
				return true;
		}
	}
	return false;
}

bool PhysicsManager::CheckCollision(PhysEntity* entity)
{
	for (auto& other : CheckNearby(entity))
	{
		if (mLayerMasks[static_cast<unsigned int>(entity->CollisionLayer())].test(static_cast<unsigned int>(other->CollisionLayer())))
		{
			if (entity->CheckCollision(other))
				return true;
		}
	}
	return false;
}

void PhysicsManager::Update()
{
	mCells.clear();

	if (mRegistered.size())
	{
		mEntity.insert(mEntity.begin(), mRegistered.begin(), mRegistered.end());
		mRegistered.clear();
	}

	for (auto& entity : mEntity)
		AddEntity(entity);

	for (auto& entity : mEntity)
	{
		for (auto& other : CheckNearby(entity))
		{
			if (mLayerMasks[static_cast<unsigned int>(entity->CollisionLayer())].test(static_cast<unsigned int>(other->CollisionLayer())))
			{
				if (entity->CheckCollision(other))
				{
					entity->Hit(other);
					other->Hit(entity);
				}
			}
		}
	}
}