//============================================================================
// Name        : P3.cpp
// Author      : Brian W. Goldman
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "Evaluation.h"
#include "HillClimb.h"
#include "Util.h"
#include "MiddleLayer.h"
#include "Population.h"
#include "Configuration.h"
#include "Optimizer.h"
#include "OptimizationCollection.h"
#include <memory>
#include "Record.h"
#include "Experiments.h"
#include "QuadTree.h"

int main(int argc, char * argv[])
{
	/*
	QuadTree tree(0, 0, 0);

	tree.toplevel(4, 5, 2, 10);
	tree.toplevel(2, 5, 2, 9);
	cout << tree.min_value << endl;
	for(const auto& min: tree.minimums)
	{
		cout <<"("<< min.first << ", " << min.second <<") ";
	}
	cout << endl;
	tree.top_remove(4, 5, 2);
	cout << tree.min_value << endl;
	for(const auto& min: tree.minimums)
	{
		cout <<"("<< min.first << ", " << min.second <<") ";
	}
	cout << endl;
	return 0;
	*/
	Random rand;
	Configuration config;
	config.parse(argc, argv);
	int seed = config.get<int>("seed");
	if(seed == -1)
	{
		std::random_device rd;
		seed = rd();
		config.set("seed", seed);
	}
	rand.seed(seed);
	auto problem = config.get<evaluation::pointer>("problem");
	auto optimizer_method = config.get<optimize::pointer>("optimizer");
	if(config.get<string>("experiment") == "bisection")
	{
		int pop_size = bisection(rand, config, problem, optimizer_method);
		cout <<"POP SIZE " << pop_size << endl;
	}
	else
	{
		vector<Record> records = multirun(rand, config, problem, optimizer_method);
		auto summary = Record::summarize(records);
		cout << summary[MES] << " " << summary[MAD] << " " << summary[FAILURES] << endl;
	}
	return 0;
}
