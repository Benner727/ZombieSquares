#include "PathFinder.h"

PathFinder::PathFinder(Map* map)
	: mMap(map)
{
	neighbours[0] = point(0, -1);  //Left
	neighbours[1] = point(-1, 0); //Up
	neighbours[2] = point(0, 1);  //Right
	neighbours[3] = point(1, 0); //Down

	//Diagnols
	neighbours[4] = point(-1, -1); //Up and Left
	neighbours[5] = point(-1, 1); //Down and Left
	neighbours[6] = point(1, -1); //Up and Right
	neighbours[7] = point(1, 1); //Down and Right
}

PathFinder::~PathFinder()
{
}

int PathFinder::calcDist(point& p) {
	int x = abs(end.x - p.x), y = abs(end.y - p.y);

	const int D = 10;
	const int D2 = 14;

	return D * (x + y) + (D2 - 2 * D) * std::min(x, y);
}

bool PathFinder::isValid(point& p) {
	return (p.x > -1 && p.y > -1 && p.x < mMap->Width() && p.y < mMap->Height());
}

bool PathFinder::canWalkTo(point & p, int dir)
{
	switch (dir)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		return true;
		break;
	case 4: //(-1, -1)
		if (!mMap->IsSolid(p.x + 1, p.y) && !mMap->IsSolid(p.x, p.y + 1))
			return true;
		break;
	case 5: //(-1, 1)
		if (!mMap->IsSolid(p.x + 1, p.y) && !mMap->IsSolid(p.x, p.y - 1))
			return true;
		break;
	case 6: //(1, -1)
		if (!mMap->IsSolid(p.x - 1, p.y) && !mMap->IsSolid(p.x, p.y + 1))
			return true;
		break;
	case 7: //(1, 1)
		if (!mMap->IsSolid(p.x - 1, p.y) && !mMap->IsSolid(p.x, p.y - 1))
			return true;
		break;
	}
	return false;
}

bool PathFinder::existPoint(point& p, int cost) {
	std::list<node>::iterator i;
	i = std::find(closed.begin(), closed.end(), p);
	if (i != closed.end()) {
		if ((*i).cost + (*i).dist < cost) return true;
		else { closed.erase(i); return false; }
	}
	i = std::find(open.begin(), open.end(), p);
	if (i != open.end()) {
		if ((*i).cost + (*i).dist < cost) return true;
		else { open.erase(i); return false; }
	}
	return false;
}

bool PathFinder::fillOpen(node& n) {
	int stepCost, nc, dist;
	point neighbour;

	for (int x = 0; x < 8; x++) {
		// make diagonals have different cost
		x < 4 ? stepCost = 10 : stepCost = 14;
		neighbour = n.pos + neighbours[x];
		if (neighbour == end) return true;

		if (isValid(neighbour) && !mMap->IsSolid(neighbour.x, neighbour.y) && canWalkTo(neighbour, x)) {
			nc = stepCost + n.cost;
			dist = calcDist(neighbour);
			if (!existPoint(neighbour, nc + dist)) {
				node m;
				m.cost = nc; m.dist = dist;
				m.pos = neighbour;
				m.parent = n.pos;
				open.push_back(m);
			}
		}
	}
	return false;
}

bool PathFinder::search(point& s, point& e) {
	node n; end = e; start = s;
	n.cost = 0; n.pos = s; n.parent = 0; n.dist = calcDist(s);
	open.push_back(n);
	while (!open.empty()) {
		//open.sort();
		node n = open.front();
		open.pop_front();
		closed.push_back(n);
		if (fillOpen(n)) return true;
	}
	return false;
}

int PathFinder::getPath(std::list<point>& path) {
	path.push_front(end);
	float cost = 1 + closed.back().cost;
	path.push_front(closed.back().pos);
	point parent = closed.back().parent;

	for (std::list<node>::reverse_iterator i = closed.rbegin(); i != closed.rend(); i++) {
		if ((*i).pos == parent && !((*i).pos == start)) {
			path.push_front((*i).pos);
			parent = (*i).parent;
		}
	}
	path.push_front(start);
	return cost;
}

int PathFinder::getCost(Vector2 src, Vector2 dest)
{
	point s(src.x / mMap->mTileSize, src.y / mMap->mTileSize);
	point e(dest.x / mMap->mTileSize, dest.y / mMap->mTileSize);

	std::list<point> path;
	if (search(s, e))
		return getPath(path);

	return INT_MAX;
}

std::list<point> PathFinder::generatePath(Vector2 src, Vector2 dest)
{
	point s(src.x / mMap->mTileSize, src.y / mMap->mTileSize);
	point e(dest.x / mMap->mTileSize, dest.y / mMap->mTileSize);

	std::list<point> path;
	if (search(s, e)) {
		int c = getPath(path);

		/*
		std::cout << "Start: " << s.x << ", " << s.y << std::endl;
		std::cout << "Goal: " << e.x << ", " << e.y << std::endl;

		std::cout << "\nPath cost " << c << ": ";
		for (std::list<point>::iterator i = path.begin(); i != path.end(); i++) {
			std::cout << "(" << (*i).x << ", " << (*i).y << ") ";
		}
		*/
	}
	else
		std::cout << "Path not found!" << std::endl;

	return path;
}
