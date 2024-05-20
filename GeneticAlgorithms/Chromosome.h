#pragma once
#include <vector>
#include <string>
#include <functional>
#include <random>

class Chromosome
{
public:
	Chromosome(const std::vector<bool>& chromosome);


	std::string ToString() const;
	float GetFitnessValue(std::function<float(const std::vector<bool>&)> fitnessFunction) const;
	void Mutate(const size_t& pos);
	void Crossover(Chromosome& other);
private:
	std::vector<bool> m_chromosome;
};

