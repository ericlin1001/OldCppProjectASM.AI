// GA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
using namespace std;

#include "GA.h"

struct Individual{
	char value[8];
	void print(){
		for(int i=0;i<8;i++)cout<<(char)(value[i]+'0');
		cout<<endl;
		char maps[8][8];
		int row,col;
		//
		for( col=0;col<8;col++){
			for( row=0;row<8;row++){
				if((7-value[col])==row) maps[row][col]=1;
				else  maps[row][col]=0;
			}
		}
		//
		for( row=-1;row<=8;row++){
			
			
			for( col=0;col<8;col++){
				if(row==-1)cout<<"--";
				if(row==8)cout<<"--";
				if(0<=row && row<8)cout<<(maps[row][col]==0?"¡«":"££");
			}
			cout<<endl;
		}
		cout<<endl;
	}
};

Individual reproduce(const Individual& x,const Individual& y){
	int t=rand()%9;//0-8
	int i;
	Individual newOne;
	for(i=0;i<t;i++){
		newOne.value[i]=x.value[i];
	}
	for(i=t;i<8;i++){
		newOne.value[i]=y.value[i];
	}
	return newOne;
}

double fitnessFunc(const Individual& individual){
	//best is 28;
	vector<char>values;
	for(int i=0;i<8;i++)values.push_back(individual.value[i]);
	int notAttackPairs=0;
	while(values.size()>=2){
		char x=values.size();
		char y=values.back();
		values.pop_back();
		for(int i=0;i<values.size();i++){
			char tx=i+1;
			char ty=values[i];
			if(y==ty || (ty-y)==(tx-x)||(ty-y)==(x-tx)){
			}else{
				notAttackPairs++;
			}
		}
	}
	return notAttackPairs;
}
void mutate(Individual& individual){
	individual.value[rand()%8]=rand()%8;
}
int times;
int numPopulation=100;
double mutateP=0.10;
inline Individual Genetic_Algorithm()
{
	vector<Individual> population;
	vector<double> fitness;
	//int numPopulation=100;
	for(int i=0;i<numPopulation;i++){
		Individual individual;
		for(int j=0;j<8;j++){
			individual.value[j]=rand()%8;
		}
		population.push_back(individual);
	}
	//create the initial population

	//
	fitness.resize(population.size());
	for(int i=0;i<population.size();i++){
		fitness[i]=fitnessFunc(population[i]);
	}
	//int times=0;
	int maxTimes=2000;
	times=0;
	while(1){
		CreateNextGeneration(population,fitness,mutateP);
		for(int i=0;i<population.size();i++){
			fitness[i]=fitnessFunc(population[i]);
		}
		times++;
		if(times>=maxTimes||fitness[findMaxIndex(fitness)]>=28){
			break;
		}
		
	}
	//cout<<"times:"<<times<<endl;
	return population[findMaxIndex(fitness)];
}
struct Record{
	int times;
	double fitness;
	Individual individual;
	Record(int t,double f,Individual i){
		times=t;
		fitness=f;
		individual=i;
	}
	void print(){
		cout<<(fitness>=28?"Sucess":"Fail")<<"\tTimes:"<<times<<"\tFitness:"<<fitness<<endl;
	}
	bool isSuccess(){return fitness>=28;}
};
#define Trace(m) cout<<#m"="<<(m)<<endl;
int _tmain(int argc, _TCHAR* argv[])
{
	/*int times;
int numPopulation=100;
int mutateP=0.01;*/
	srand(time(NULL));
	//
	numPopulation=100;
	mutateP=0.10;
	vector<Record>records;
	//
	double min=0.109475;//0.063-0.0955
	//
	double max=0.109475;//0.109475-0.113375
	for(mutateP=min;mutateP<=max;mutateP+=(max-min)/10){
		Trace(mutateP);
		//
	records.clear();
	int i;int numRecords=30;
	for( i=0;i<numRecords;i++){
		Individual a=Genetic_Algorithm();
		double fitness=fitnessFunc(a);
		records.push_back(Record(times,fitness,a));
		records.back().print();
	}
	int avg_times=0;int numSuccess=0;
	Trace(numRecords);
	for( i=0;i<numRecords;i++){
		if(records[i].isSuccess()){
			numSuccess++;
			avg_times+=records[i].times;
		}
	}
	avg_times/=numSuccess;
	Trace(numSuccess);
	Trace(avg_times);
	Trace((double)numSuccess/numRecords);
	cout<<endl;
	}
	//
	system("pause");
	return 0;
}

