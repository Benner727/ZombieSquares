#pragma once

#include "AudioManager.h"
#include "AnimatedTexture.h"
#include "Actor.h"

class Event : public Actor
{
public:
	Event(Map* map, Vector2 pos, float rotation);
	~Event();

protected:
	Timer* mTimer;
	AudioManager* mAudio;

	AnimatedTexture* mSprite;

	float mLifeTimer;

	void Hit(PhysEntity* other) override;

public:
	bool CleanUp();

	void Update();
	void Render();
};