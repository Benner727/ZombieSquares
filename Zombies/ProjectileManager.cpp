#include "ProjectileManager.h"

ProjectileManager* ProjectileManager::sInstance = nullptr;

ProjectileManager* ProjectileManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new ProjectileManager();

	return sInstance;
}

void ProjectileManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
	for (std::vector<Projectile*>::iterator it = mProjectiles.begin(); it != mProjectiles.end(); ++it)
		delete * it;
}

void ProjectileManager::AddProjectile(Projectile * projectile)
{
	mProjectiles.push_back(projectile);
}

void ProjectileManager::Update()
{
	for (std::vector<Projectile*>::iterator it = mProjectiles.begin(); it != mProjectiles.end();)
	{
		if ((*it)->CleanUp())
		{
			delete * it;
			it = mProjectiles.erase(it);
		}
		else
		{
			(*it)->Update();
			++it;
		}
	}
}

void ProjectileManager::Render()
{
	for (const auto& projectile : mProjectiles)
		projectile->Render();
}