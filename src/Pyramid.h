/*
 * Pyramid.h
 *
 *  Created on: Jun 26, 2013
 *      Author: goldman
 */

#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "Population.h"
#include "Evaluation.h"
#include "Util.h"

class Pyramid
{
private:
	vector<Population> pops;

	bool add_unique(Random& rand, const vector<bool> & solution, size_t level);
	size_t length;
public:
	std::unordered_set<vector<bool>> seen;
	Pyramid(int l): length(l) {}
	virtual ~Pyramid() = default;
	void climb(Random& rand, vector<bool> & solution, float & fitness, Evaluator& evaluator);
};

#endif /* PYRAMID_H_ */