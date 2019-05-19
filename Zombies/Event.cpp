#include "Event.h"

Event::Event(Map* map, Vector2 pos, float rotation)
	: Actor(map)
{
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mLifeTimer = 10.0f;

	mSprite = nullptr;
}


Event::~Event()
{
	delete mSprite;
}

void Event::Hit(PhysEntity* other)
{
}

bool Event::CleanUp()
{
	return !(Active());
}

void Event::Update()
{
	if (Active())
		mSprite->Update();

	mLifeTimer -= mTimer->DeltaTime();

	if (mLifeTimer <= 0.0f)
		Active(false);
}

void Event::Render()
{
	if (Active() && mMap->InFOV(this))
		mSprite->Render();

	PhysEntity::Render();
}