#pragma once

#include "Map.h"

class LOS
{
public:
	LOS(Map* map);
	~LOS();

private:
	Map* mMap;

	bool isTraversable(Vector2 point);
	
	template <class T>
	void swap(T &a, T &b);
	
	std::vector<Vector2> drawLine(Vector2 src, Vector2 dest);

public:
	bool inLOS(Vector2 src, Vector2 dest);
};

template <class T>
inline void LOS::swap(T &a, T &b)
{
	T c = a;
	a = b;
	b = c;
}