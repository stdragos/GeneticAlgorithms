#pragma once
#include "Chromosome.h"


class Population
{
public:
	using Interval = std::pair<double, double>;

	Population(const size_t& populationSize, const size_t& chromosomeSize, double mutationProbability, double crossoverProbability);

	Population(const Population& other);
	Population(Population&& other) noexcept;

	Population& operator=(const Population& other);
	Population& operator=(Population&& other) noexcept;

	void Crossover();
	void Mutation();
	void Selection();

	~Population() = default;

	friend std::ostream& operator<<(std::ostream&, const Population& population);

	void SetIntervalX(const Interval&);
	void SetIntervalY(const Interval&);
	void SetFitnessFunction(const std::function<double(const Chromosome&)>&);

	Chromosome GetMin() const;
	Chromosome GetMax() const;
	std::function<double(const Chromosome&)> GetFitnessFunction() const;

	

	std::pair<Chromosome, size_t> Fit(size_t epochs, std::ostream& out = std::cout);
private:

	std::function<double(const Chromosome&)>
		m_fitnessFunction = [this](const Chromosome& chromosome) -> double {
		auto values = chromosome.GetDecimal();

		double x = m_intervalX.first +
			values.first * (m_intervalX.second - m_intervalX.first) / ((1 << chromosome.GetSize() / 2) - 1);
		double y = m_intervalY.first +
			values.second * (m_intervalY.second - m_intervalY.first) / ((1 << chromosome.GetSize() / 2) - 1);

		return 3.0 + abs(log(std::max(1., 3.0 + sin(x) * 5.0)) + tan(y));
		};

	std::vector<Chromosome> m_population;
	Interval m_intervalX;
	Interval m_intervalY;

	size_t m_chromosomeSize;
	double m_mutationProbability;
	double m_crossoverProbability;
};

