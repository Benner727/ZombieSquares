#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "Collider.h"
#include <vector>

class BoxCollider : public Collider
{
private:
	static const int MAX_VERTS = 4;
	GameEntity* mVerts[MAX_VERTS];

	void AddVert(int index, Vector2 pos);

public:
	BoxCollider(Vector2 size, float rotation = 0.0f);
	~BoxCollider();

	Vector2 GetFurthestPoint() override;

	Vector2 GetVertexPos(int index) const;
};

#endif