#include "Chromosome.h"

Chromosome::Chromosome(const std::vector<bool>& chromosome)
{
	//std::random_device rd;
	//std::mt19937 gen(rd());

	//std::bernoulli_distribution distribution(0.5);

	//m_chromosome = std::vector<bool>(size, false);

	//for (size_t pos = 0; pos < size; ++pos)
	//	m_chromosome[pos] = distribution(gen);

	m_chromosome = chromosome;
}

std::string Chromosome::ToString() const
{
	std::string aux = "";
	for (const auto& gene : m_chromosome)
		aux += gene ? '1' : '0';

	return aux;
}

float Chromosome::GetFitnessValue(std::function<float(const std::vector<bool>&)> fitnessFunction) const
{
	return fitnessFunction(m_chromosome);
}

