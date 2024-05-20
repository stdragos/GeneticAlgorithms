#include "Population.h"

Population::Population(const size_t& populationSize, const size_t& chromosomeSize, float mutationProbability) :
	m_mutationProbability{ mutationProbability }
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::bernoulli_distribution distribution(0.5);

	std::vector<bool> currentChromosome;

	for (size_t curr = 0; curr < populationSize; ++curr)
	{
		currentChromosome.resize(chromosomeSize);

		for (size_t gene = 0; gene < chromosomeSize; ++gene)
			currentChromosome[gene] = distribution(generator);

		m_population.emplace_back(Chromosome(currentChromosome));
	}
}

Population::Population(const Population& other)
{
	*this = other;
}

Population::Population(Population&& other) noexcept
{
	*this = std::move(other);
}

Population& Population::operator=(const Population& other)
{
	if (this != &other)
	{
		m_population = other.m_population;
		m_mutationProbability = other.m_mutationProbability;
	}
		
	return *this;
}

Population& Population::operator=(Population&& other) noexcept
{
	if (this != &other)
	{
		float resetValue = 0.f;
		m_population = std::exchange(other.m_population, std::vector<Chromosome>());
		m_mutationProbability = std::exchange(other.m_mutationProbability, resetValue);
	}

	return *this;
}
