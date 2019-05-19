#ifndef COLLIDER_H
#define COLLIDER_H

#include "Texture.h"

class Collider : public GameEntity
{
public:
	enum class ColliderType
	{
		Box,
		Circle
	};

protected:
	ColliderType mType;

	static const bool DEBUG_COLLIDERS = false;
	Texture* mDebugTexture;

	void SetDebugTexture(Texture* texture);

public:
	Collider(ColliderType type);
	virtual ~Collider();

	virtual Vector2 GetFurthestPoint() = 0;

	ColliderType GetType() const;

	virtual void Render();
};

#endif