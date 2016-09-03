#ifndef GA_H
#define GA_H
#include<vector>
using namespace std;
#define ASSERT(m)
inline double myRand(){//return [0,1)
	return (double)(rand()%65536)/65536.0;
}
int randomSelect(const vector<double>& arr);
int findMaxIndex(const vector<double>& arr);





//need to carry out:
template<class Individual>
Individual reproduce(const Individual& x,const Individual& y);

template<class Individual>
void mutate(Individual& individual);

template<class Individual>
double fitnessFunc(const Individual& individual);

template<class Individual>
inline Individual Genetic_Algorithm();
/*
{
	vector<Individual> population;
	vector<double> fitness;
	int times=0;int maxTimes=10;
	
	//create the initial population
	
	fitness.resize(population.size());
	for(int i=0;i<population.size();i++){
		fitness[i]=fitnessFunc(population[i]);
	}
	while(times++<maxTimes){
		CreateNextGeneration(population,fitness);
		for(int i=0;i<population.size();i++){
			fitness[i]=fitnessFunc(population[i]);
		}
	}
	return population[findMaxIndex(fitness)];
}
*/
template<class Individual>
void CreateNextGeneration(vector<Individual>& population,const vector<double>& fitness,const double &mutateProbality=0.01){
	vector<Individual> newPopulation;
	unsigned int i;
	for(i=0;i<population.size();i++){
		Individual x=population[randomSelect(fitness)];
		Individual y=population[randomSelect(fitness)];
		Individual child=reproduce(x,y);
		if(myRand()<=mutateProbality)mutate(child);
		newPopulation.push_back(child);
	}
	population=newPopulation;
}
#endif