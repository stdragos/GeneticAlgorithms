#include <fstream>
#include <iostream>
#include "Population.h"

int main()
{
    Population population(10, 10, 0.1, 0.1);
    population.SetIntervalX({ -5, 3 });
    population.SetIntervalY({ 2, 10 });

    std::ofstream fout("output.txt");

    fout << "#start\n\n";

    auto best = population.Fit(100, fout);

    

    fout << "Best chromosome: \n";
    fout << "Epoch: " << best.second << '\n';
    fout << "Chromosome: " << best.first << '\n';
    fout << "Value: " << best.first.GetFitnessValue(population.GetFitnessFunction());

    fout << "\n\n#end\n\n";


	return 0;
}