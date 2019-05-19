#include "EventManager.h"

EventManager* EventManager::sInstance = nullptr;

EventManager* EventManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new EventManager();

	return sInstance;
}

void EventManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
	for (std::vector<Event*>::iterator it = mEvents.begin(); it != mEvents.end(); ++it)
		delete * it;
}

void EventManager::AddEvent(Event * event)
{
	mEvents.push_back(event);
}

void EventManager::Update()
{
	for (std::vector<Event*>::iterator it = mEvents.begin(); it != mEvents.end();)
	{
		if ((*it)->CleanUp())
		{
			delete * it;
			it = mEvents.erase(it);
		}
		else
		{
			(*it)->Update();
			++it;
		}
	}
}

void EventManager::Render()
{
	for (const auto& event : mEvents)
		event->Render();
}