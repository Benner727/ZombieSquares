#pragma once

#include "Event.h"

class Acid : public Event
{
public:
	Acid(Map* map, Vector2 pos, float rotation);
	~Acid();

	void Update() override;
};