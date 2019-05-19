#pragma once

#include "Event.h"

class EventManager
{
private:
	static EventManager* sInstance;

	EventManager();
	~EventManager();

	std::vector<Event*> mEvents;

public:
	static EventManager* Instance();
	static void Release();

	void AddEvent(Event* event);

	void Update();
	void Render();
};
