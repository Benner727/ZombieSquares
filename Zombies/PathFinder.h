#pragma once

#include "Map.h"

#include <list>
#include <algorithm>
#include <iostream>

struct point {
	point(int a = 0, int b = 0) { x = a; y = b; }
	bool operator ==(const point& o) { return o.x == x && o.y == y; }
	point operator +(const point& o) { return point(o.x + x, o.y + y); }
	int x, y;
};

struct node {
	bool operator == (const node& o) { return pos == o.pos; }
	bool operator == (const point& o) { return pos == o; }
	bool operator < (const node& o) { return dist + cost < o.dist + o.cost; }
	point pos, parent;
	int dist, cost;
};

class PathFinder
{
public:
	PathFinder(Map* map);
	~PathFinder();

private:
	Map* mMap;
	point end, start;
	point neighbours[8];
	std::list<node> open;
	std::list<node> closed;

	int calcDist(point& p);
	bool isValid(point& p);
	bool canWalkTo(point& p, int dir);
	bool existPoint(point& p, int cost);
	bool fillOpen(node& n);
	bool search(point& s, point& e);
	int getPath(std::list<point>& path);

public:
	int getCost(Vector2 src, Vector2 dest);
	std::list<point> generatePath(Vector2 src, Vector2 dest);
};