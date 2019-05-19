#pragma once

#include "Map.h"

class FOV
{
public:
	FOV(Map* map);
	~FOV();

private:
	Map* mMap;

	std::vector<bool> mPoints;

	bool isValid(int x, int y) const;

	void resetFOV();
	void drawLine(int x0, int y0, int x1, int y1);

public:
	void calculateFOV(int x, int y, int dist);
	bool InFOV(int x, int y);

	std::vector<bool> points() const { return mPoints; }
};