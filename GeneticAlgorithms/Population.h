#pragma once
#include "Chromosome.h"

class Population
{
public:
	Population(const size_t& populationSize, const size_t& chromosomeSize, float mutationProbability);

	Population(const Population& other);
	Population(Population&& other) noexcept;

	Population& operator=(const Population& other);
	Population& operator=(Population&& other) noexcept;

	~Population() = default;
private:
	std::vector<Chromosome> m_population;
	float m_mutationProbability;
};

