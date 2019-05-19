#include "LOS.h"

LOS::LOS(Map* map)
	: mMap(map)
{
}


LOS::~LOS()
{
}

bool LOS::isTraversable(Vector2 point)
{
	int x = point.x / mMap->mTileSize;
	int y = point.y / mMap->mTileSize;

	return !mMap->IsSolid(x, y);
}

std::vector<Vector2> LOS::drawLine(Vector2 src, Vector2 dest)
{
	int x0 = src.x;
	int y0 = src.y;
	int x1 = dest.x;
	int y1 = dest.y;

	bool steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = 0;
	int ystep;
	int y = y0;
	y0 < y1 ? ystep = 1 : ystep = -1;

	std::vector<Vector2> result;
	for (int x = x0; x <= x1; ++x)
	{
		if (steep)
			result.push_back(Vector2(y, x));
		else
			result.push_back(Vector2(x, y));
		error += deltay;
		if (error * 2 >= deltax)
		{
			y += ystep;
			error -= deltax;
		}
	}

	return result;
}

bool LOS::inLOS(Vector2 src, Vector2 dest)
{
	std::vector<Vector2> line = drawLine(src, dest);

	for (const auto& point : line)
	{
		if (!isTraversable(point))
			return false;
	}

	return true;
}
