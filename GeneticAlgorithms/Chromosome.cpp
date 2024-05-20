#include "Chromosome.h"

Chromosome::Chromosome(const std::vector<bool>& chromosome)
{
	m_chromosome = chromosome;
}

Chromosome::Chromosome(const Chromosome& other)
{
	*this = other;
}

Chromosome::Chromosome(Chromosome&& other) noexcept
{
	*this = std::move(other);
}

Chromosome& Chromosome::operator=(const Chromosome& other)
{
	if (this != &other)
		m_chromosome = other.m_chromosome;
	return *this;
}

Chromosome& Chromosome::operator=(Chromosome&& other) noexcept
{
	if (this != &other)
		m_chromosome = std::exchange(other.m_chromosome, std::vector<bool>());

	return *this;
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

