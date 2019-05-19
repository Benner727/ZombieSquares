#ifndef ACTOR_H
#define ACTOR_H

#include "PhysEntity.h"
#include "Map.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

class Actor : public PhysEntity
{
public:
	Actor(Map* map = nullptr);
	~Actor();

protected:
	Map* mMap;

	bool mNonPlayer;

	bool HasGround();
	bool HasCeiling();
	bool HasLeftWall();
	bool HasRightWall();

	bool InsideTile();

	bool OutsideMap();

	bool IgnoreCollisions() override;

public:
	void Update();
	void Render();

};

#endif