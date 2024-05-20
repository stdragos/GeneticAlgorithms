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

double Chromosome::GetFitnessValue(std::function<double(const std::vector<bool>&)> fitnessFunction) const
{
	return fitnessFunction(m_chromosome);
}

std::pair<double, double> Chromosome::GetDecimal() const
{
	int power = 1;
	int x = 0;
	int y = 0;

	for (int i = m_chromosome.size() / 2 - 1; i >= 0; --i) {
		x += power * m_chromosome[i];
		y += power * m_chromosome[i + m_chromosome.size() / 2];
		power *= 2;
	}

	return { x, y };
}

size_t Chromosome::GetSize() const
{
	return m_chromosome.size();
}

void Chromosome::Mutate(const size_t& pos)
{
	m_chromosome[pos] = !m_chromosome[pos];
}

void Chromosome::Crossover(const size_t& pos, Chromosome& other)
{
	for (size_t index = pos; index < other.m_chromosome.size(); ++index)
	{
		swap(m_chromosome[index], other.m_chromosome[index]);
	}
}

