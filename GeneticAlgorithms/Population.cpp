#include "Population.h"


Population::Population(const size_t& populationSize, const size_t& chromosomeSize, double mutationProbability, double crossoverProbability) :
	m_mutationProbability{ mutationProbability }, m_crossoverProbability { crossoverProbability }, m_chromosomeSize { chromosomeSize }
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::bernoulli_distribution distribution(0.5);

	std::vector<bool> currentChromosome;

	for (size_t curr = 0; curr < populationSize; ++curr)
	{
		currentChromosome.resize(m_chromosomeSize);

		for (size_t gene = 0; gene < m_chromosomeSize; ++gene)
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

std::ostream& operator<<(std::ostream& out, const Population& population)
{
	for (const auto& chromosome : population.m_population)
		out << "Chromosome " <<
		chromosome <<
		"\nValue " <<
		chromosome.GetFitnessValue(population.GetFitnessFunction()) <<
		'\n';

	return out;
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

std::pair<Chromosome, size_t> Population::Fit(size_t epochs, std::ostream& out)
{
	Chromosome best;
	size_t bestEpoch = -1;

	for (size_t epoch = 1; epoch <= epochs; ++epoch)
	{
		Selection();
		Crossover();
		Mutation();

		auto maxChromosome = GetMax();

		out << "Epoch " << epoch << '\n';
		out << *this;
		out << "Best chromosome: \n";
		out << maxChromosome << '\n' << "Value: "
			<< maxChromosome.GetFitnessValue(m_fitnessFunction) << "\n\n";


		if(bestEpoch != -1)
		{
			if (maxChromosome.GetFitnessValue(m_fitnessFunction) > best.GetFitnessValue(m_fitnessFunction))
			{
				best = maxChromosome;
				bestEpoch = epoch;
			}
		}
		else
		{
			best = maxChromosome;
			bestEpoch = epoch;
		}
	}

	return { best, bestEpoch };
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

void Population::Crossover()
{
	const double epsilon = 1e-6;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distValue(epsilon, 1.0 + epsilon);

	std::vector<size_t> parentsIndices = std::vector<size_t>();

	for (size_t chromosomeIndex = 0; chromosomeIndex < m_population.size(); ++chromosomeIndex)
	{
		const double randomValue = distValue(gen);

		if (randomValue < m_crossoverProbability)
		{
			parentsIndices.push_back(chromosomeIndex);
		}
	}

	if (parentsIndices.size() % 2)
	{
		parentsIndices.erase(parentsIndices.end() - 1);
	}

	auto rng = std::default_random_engine{ rd() };
	std::shuffle(std::begin(parentsIndices), std::end(parentsIndices), rng);
	std::uniform_int_distribution<> distPos(0, m_chromosomeSize - 1);

	for (size_t parentIndex = 0; parentIndex < parentsIndices.size(); parentIndex += 2)
	{
		const int randomPos = distPos(gen);
		m_population[parentIndex].Crossover(randomPos, m_population[parentIndex + 1]);
	}
}

void Population::Mutation()
{
	const double epsilon = 1e-6;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distValue(epsilon, 1.0 + epsilon);

	for (auto& chromosome : m_population)
	{
		for (size_t indexChromosome = 0; indexChromosome < m_chromosomeSize; ++indexChromosome)
		{
			const double randomValue = distValue(gen);

			if (randomValue < m_mutationProbability)
			{
				chromosome.Mutate(indexChromosome);
			}
		}
	}
}
