#pragma once
#include <vector>
#include <string>
#include <functional>
#include <random>

class Chromosome
{
public:
	Chromosome() = default;
	Chromosome(const std::vector<bool>& chromosome);

	Chromosome(const Chromosome& other);
	Chromosome(Chromosome&& other) noexcept;

	Chromosome& operator=(const Chromosome& other);
	Chromosome& operator=(Chromosome&& other) noexcept;

	~Chromosome() = default;


	std::string ToString() const;
	double GetFitnessValue(std::function<double(const std::vector<bool>&)> fitnessFunction) const;
	void Mutate(const size_t& pos);
	void Crossover(Chromosome& other);
	std::pair<double, double> GetDecimal() const;
	size_t GetSize() const;

private:
	std::vector<bool> m_chromosome;
};

