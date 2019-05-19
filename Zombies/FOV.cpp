#include "FOV.h"


FOV::FOV(Map* map)
	: mMap(map)
{
	resetFOV();
}

FOV::~FOV()
{
}

bool FOV::isValid(int x, int y) const
{
	return (x > -1 && y > -1 && x < mMap->Width() && y < mMap->Height());
}

void FOV::resetFOV()
{
	mPoints.clear();
	for (int i = 0; i < mMap->Width() * mMap->Height(); i++)
		mPoints.push_back(false);
}

void FOV::drawLine(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	// if x1 == x2, then it does not matter what we set here
	int ix((dx > 0) - (dx < 0));

	dx = abs(dx) << 1;

	int dy = y2 - y1;
	// if y1 == y2, then it does not matter what we set here
	int iy((dy > 0) - (dy < 0));
	dy = abs(dy) << 1;

	if (!isValid(x1, y1))
		return;
	
	mPoints[x1 + y1 * mMap->Width()] = true;
	if (mMap->IsBoundary(x1, y1) || mMap->IsSolid(x1, y1))
		return;

	if (dx >= dy)
	{
		// error may go below zero
		int error(dy - (dx >> 1));

		while (x1 != x2)
		{
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= dx;
				y1 += iy;
			}
			// else do nothing

			error += dy;
			x1 += ix;

			if (!isValid(x1, y1))
				return;
			
			mPoints[x1 + y1 * mMap->Width()] = true;
			if (mMap->IsBoundary(x1, y1) || mMap->IsSolid(x1, y1))
				return;
		}
	}
	else
	{
		// error may go below zero
		int error(dx - (dy >> 1));

		while (y1 != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= dy;
				x1 += ix;
			}
			// else do nothing

			error += dx;
			y1 += iy;

			if (!isValid(x1, y1))
				return;
			
			mPoints[x1 + y1 * mMap->Width()] = true;
			if (mMap->IsBoundary(x1, y1) || mMap->IsSolid(x1, y1))
				return;
		}
	}
}

void FOV::calculateFOV(int x, int y, int dist)
{
	resetFOV();

	if (x < 0)
		x = 0;
	else if (x >= mMap->Width())
		x = mMap->Width() - 1;

	if (y < 0)
		y = 0;
	else if (y >= mMap->Height())
		y = mMap->Height() - 1;

	for (float angle = 0.0f; angle <= 360; angle += 1.0f)
	{
		int x1 = dist * cos(angle * 3.14f / 180.0f);
		int y1 = dist * sin(angle * 3.14f / 180.0f);

		/*
		if (x + x1 < 0)
			x1 = 0 - x;
		else if (x + x1 >= mMap->Width())
			x1 = mMap->Width() - x - 1;

		if ( y + y1 < 0)
			y1 = 0 - y;
		else if (y + y1 >= mMap->Height())
			y1 = mMap->Height() - y - 1;
		*/

		drawLine(x, y, x + x1, y + y1);
	}
}

bool FOV::InFOV(int x, int y)
{
	return mPoints[x + y * mMap->Width()];
}