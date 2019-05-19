#include "Collider.h"

Collider::Collider(ColliderType type)
	: mType(type)
{
	mDebugTexture = nullptr;
}

Collider::~Collider()
{
	delete mDebugTexture;
}

void Collider::SetDebugTexture(Texture * texture)
{
	delete mDebugTexture;
	mDebugTexture = texture;
	mDebugTexture->Parent(this);
}

Collider::ColliderType Collider::GetType() const
{
	return mType;
}

void Collider::Render()
{
	if (DEBUG_COLLIDERS)
		mDebugTexture->Render();
}
