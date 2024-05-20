#pragma once
#include "Chromosome.h"

class Population
{
public:
	Population(const size_t& populationSize, const size_t& chromosomeSize);

private:
	std::vector<Chromosome> m_population;
};

