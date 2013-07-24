/*
 * Pyramid.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: goldman
 */

#include "Pyramid.h"

void Pyramid::climb(Random& rand, vector<bool> & solution, float & fitness, Evaluator& evaluator)
{
	add_unique(rand, solution, 0);
	for(size_t level=0; level < pops.size(); level++)
	{
		//use population clusters and population solutions to make new solution
		pops[level].improve(rand, solution, fitness, evaluator);
		add_unique(rand, solution, level+1);
	}
}

bool Pyramid::add_unique(Random& rand, const vector<bool> & solution, size_t level)
{
	if(seen.find(solution) == seen.end())
	{
		if(pops.size() == level)
		{
			pops.push_back(Population(length));
			pops[level].never_use_singletons();
			//std::cout << "Making Level " << level << " seen " << seen.size() << std::endl;
		}
		pops[level].add(solution);
		pops[level].rebuild_tree(rand);
		seen.insert(solution);
		return true;
	}
	return false;
}

void Pyramid::optimize(Random& rand, Evaluator& evaluator, Configuration& config)
{
	float fitness = 0;
	auto hill_climber = config.get<hill_climb::pointer>("hill_climber");
	while(fitness < 1.0)
	{
		vector<bool> solution = rand_vector(rand, length);
		fitness = evaluator.evaluate(solution);
		hill_climber(rand, solution, fitness, evaluator);
		climb(rand, solution, fitness, evaluator);
	}
	/*
	for(size_t level=0; level < pops.size(); level++)
	{
		std::cout << "Level " << level << " size " << pops[level].solutions.size();
		float total = 0;
		for(auto& solution: pops[level].solutions)
		{
			total += evaluator.evaluate(solution);
		}
		std::cout << " average " << total / pops[level].solutions.size() << std::endl;
	}
	*/
}
