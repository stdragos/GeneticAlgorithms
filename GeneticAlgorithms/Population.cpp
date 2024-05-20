#include "Population.h"

Population::Population(const size_t& populationSize, const size_t& chromosomeSize, double mutationProbability) :
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

		m_population.emplace_back(currentChromosome);
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
		double resetValue = 0.f;
		m_population = std::exchange(other.m_population, std::vector<Chromosome>());
		m_mutationProbability = std::exchange(other.m_mutationProbability, resetValue);
	}

	return *this;
}

std::string Population::ToString() const
{
	std::string toString = "";

	for (const auto& chromosome : m_population)
	{
		toString += "Chromosome "
			+ chromosome.ToString()
			+ '\n' + "Value "
			+ std::to_string(chromosome.GetFitnessValue(m_fitnessFunction))
			+ '\n';
	}

	return toString;
}

void Population::SetIntervalX(const Interval& interval)
{
	m_intervalX = interval;
}

void Population::SetIntervalY(const Interval& interval)
{
	m_intervalY = interval;
}

void Population::SetFitnessFunction(const std::function<double(const Chromosome&)>& fitnessFunction)
{
	m_fitnessFunction = fitnessFunction;
}

Chromosome Population::GetMin() const
{
	return *(std::min_element(m_population.cbegin(), m_population.cend(), [this](const Chromosome& a, const Chromosome& b) {
		return a.GetFitnessValue(m_fitnessFunction) < b.GetFitnessValue(m_fitnessFunction);
		}));
}

Chromosome Population::GetMax() const
{
	return *(std::max_element(m_population.cbegin(), m_population.cend(), [this](const Chromosome& a, const Chromosome& b) {
		return a.GetFitnessValue(m_fitnessFunction) < b.GetFitnessValue(m_fitnessFunction);
		}));
}

std::function<double(const Chromosome&)> Population::GetFitnessFunction() const
{
	return m_fitnessFunction;
}

void Population::Selection()
{
	double sum = 0.f;

	for (const auto& chromosome : m_population)
		sum += chromosome.GetFitnessValue(m_fitnessFunction);

	std::vector<double> probabilty_density;

	std::for_each(m_population.begin(),
		m_population.end(),
		[&probabilty_density, sum, this](const Chromosome& chromosome) -> void
		{
			probabilty_density.emplace_back(chromosome.GetFitnessValue(m_fitnessFunction) / sum);
		});

	std::vector<double> cumulative_probability(probabilty_density.size());

	cumulative_probability[0] = probabilty_density[0];

	for (size_t i = 1; i < probabilty_density.size(); ++i)
		cumulative_probability[i] = cumulative_probability[i - 1] + probabilty_density[i];

	std::vector<Chromosome> newPopulation(m_population.size());

	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_real_distribution<double>  uniform(0, 1);


	for (size_t i = 0; i < newPopulation.size(); ++i)
	{
		double selection = uniform(generator);

		const auto& argChromosome = std::upper_bound(cumulative_probability.begin(),
			cumulative_probability.end(),
			selection);

		newPopulation[i] = m_population[std::distance(cumulative_probability.begin(), argChromosome)];
	}

	m_population = newPopulation;
}
