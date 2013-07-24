/*
 * QuadTree.h
 *
 *  Created on: Jul 22, 2013
 *      Author: goldman
 */

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <vector>
#include <array>
#include <utility>
#include <list>

using std::vector;
using std::list;

//TODO REMOVE
#include <iostream>
using namespace std;
#include "Util.h"

#include <limits>

class QuadTree {
private:

	array<QuadTree*, 4> children;
	array<int, 4> order;
	Random& rand;
	int level;
	int max_level;
	bool dirty;

public:
	struct data
	{
		int x, y;
		float value;
		data(int _x, int _y, float _value): x(_x), y(_y), value(_value) {}
	};

	float min_value;

	int minx, miny;
	QuadTree(Random& rand, int max_level, int level=0);
	virtual ~QuadTree();
	//void toplevel(Random& rand, int x, int y, int level, float value);
	//void update(Random& rand, int x, int y, int level, float value);
	void insert(const list<data>& updates);
	void remove_invalid(const vector<bool>& invalid);

	void dirty_insert(int x, int y, float value);
	void clean(const vector<bool>& invalid);
};

#endif /* QUADTREE_H_ */
