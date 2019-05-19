#pragma once

#include "Event.h"

class PowerUp : public Event
{
public:
	PowerUp(Map* map, Vector2 pos);
	~PowerUp();

private:
	int mType;

	void Hit(PhysEntity* other) override;

public:
	int Type() const { return mType; }
};

