#include <iostream>
#include "Population.h"

int main()
{
    Population population(10, 10, 0.1, 0.1);
    population.SetIntervalX({ -5, 3 });
    population.SetIntervalY({ 2, 10 });

    population.Fit(1150);

	return 0;
}