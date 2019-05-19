#pragma once

#include "Event.h"

class Explosion : public Event
{
public:
	Explosion(Map* map, Vector2 pos, float rotation, bool upgraded = false);
	~Explosion();

	bool IgnoreCollisions() override;
};

