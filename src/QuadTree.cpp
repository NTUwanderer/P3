/*
 * QuadTree.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: goldman
 */

#include "QuadTree.h"

QuadTree::QuadTree(Random& random, int _max_level, int _level):
	rand(random), level(_level), max_level(_max_level)
{
	order = {{0, 1, 2, 3}};
	children = {{NULL, NULL, NULL, NULL}};
	std::shuffle(order.begin(), order.end(), rand);
	minx = 0;
	miny = 0;
	min_value = std::numeric_limits<float>::max();
	dirty = true;
}

QuadTree::~QuadTree()
{
	for(auto& child: children)
	{
		delete child;
		child=NULL;
	}
}

/*
void QuadTree::toplevel(Random& rand, int x, int y, int level, float value)
{
	if(x > y)
	{
		std::swap(x, y);
	}
	update(rand, x, y, level, value);
	//std::cout << "-----------------END---------"<<std::endl;
}

void QuadTree::update(Random& rand, int x, int y, int level, float value)
{
	// not at the bottom
	if(level >= 0)
	{
		// get the bit for this level
		int key = (x >> level) & 1;
		key = (key << 1) | ((y >> level) & 1);
		//cout << "level " << level << " update key " << key <<" x " << x << " y " << y << endl;
		if(not children[key])
		{
			children[key] = new QuadTree(rand, x, y, value);
		}
		children[key]->update(rand, x, y, level-1, value);
		min_value = std::numeric_limits<float>::max();
		//minimums.clear();
		//std::shuffle(order.begin(), order.end(), rand);
		for(const auto& index: order)
		{
			const auto& child = children[index];
			if(child)
			{
				if(child->min_value <= min_value)
				{
					if(child->min_value < min_value)
					{
						min_value = child->min_value;
						minx = child->minx;
						miny = child->miny;
						//minimums.clear();
					}
					//minimums.insert(minimums.end(),
					//		child->minimums.begin(), child->minimums.end());
				}
			}
		}
	}
	// the bottom
	else
	{
		min_value = value;
		minx = x;
		miny = y;
	}
}
*/
void QuadTree::insert(const list<data>& updates)
{
	if(level >= max_level)
	{
		const auto& update = updates.front();
		minx = update.x;
		miny = update.y;
		min_value = update.value;
		return;
	}

	array<list<data>, 4> sublists;
	int key;
	for(const auto& it: updates)
	{
		// determine which sublist this is
		key = (it.x >> level) & 1;
		key = (key << 1) | ((it.y >> level) & 1);
		sublists[key].push_back(it);
	}
	min_value = std::numeric_limits<float>::max();
	for(const auto& index: order)
	{
		// if there are updates for this sublist
		const auto& sublist = sublists[index];
		auto & child = children[index];
		if(sublist.size())
		{
			if(not child)
			{
				child = new QuadTree(rand, max_level, level+1);
			}
			// Insert the new information into the lower level
			child->insert(sublist);
		}
		if(child and child->min_value < min_value)
		{
			min_value = child->min_value;
			minx = child->minx;
			miny = child->miny;
		}
	}
}

// only call when you know the top is invalid
void QuadTree::remove_invalid(const vector<bool>& invalid)
{
	min_value = std::numeric_limits<float>::max();
	for(const auto& index: order)
	{
		auto & child = children[index];
		if(child)
		{
			if(invalid[child->minx] or invalid[child->miny])
			{
				child->remove_invalid(invalid);
			}
			if(child->min_value == std::numeric_limits<float>::max())
			{
				delete child;
				child = NULL;
			}
			else if(child->min_value < min_value)
			{
				min_value = child->min_value;
				minx = child->minx;
				miny = child->miny;
			}
		}
	}
}



void QuadTree::dirty_insert(int x, int y, float value)
{
	if(level >= max_level)
	{
		minx = x;
		miny = y;
		min_value = value;
		dirty = false;
		return;
	}
	dirty = true;
	int key;
	key = (x >> level) & 1;
	key = (key << 1) | ((y >> level) & 1);
	auto& child = children[key];
	if(not child)
	{
		child = new QuadTree(rand, max_level, level+1);
	}
	// Insert the new information into the lower level
	child->dirty_insert(x, y, value);
}

// only call when you know the top is invalid
void QuadTree::clean(const vector<bool>& invalid)
{
	min_value = std::numeric_limits<float>::max();
	for(const auto& index: order)
	{
		auto & child = children[index];
		if(child)
		{
			if(child->dirty or invalid[child->minx] or invalid[child->miny])
			{
				child->clean(invalid);
			}
			if(child->min_value == std::numeric_limits<float>::max())
			{
				delete child;
				child = NULL;
			}
			else if(child->min_value < min_value)
			{
				min_value = child->min_value;
				minx = child->minx;
				miny = child->miny;
			}
		}
	}
	dirty = false;
}
