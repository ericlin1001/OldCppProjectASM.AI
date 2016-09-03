#include "stdafx.h"
#include "GA.h"
int randomSelect(const vector<double>& arr){
	double total=0;
	for(int i=0;i<arr.size();i++){
		total+=arr[i];
	}
	double r=myRand()*total;
	total=0;
	for(int i=0;i<arr.size();i++){
		total+=arr[i];
		if(total>r)return i;
	}
	ASSERT(false);
}
int findMaxIndex(const vector<double>& arr){
	double max=arr[0];
	int index=0;
	for(int i=1;i<arr.size();i++){
		if(arr[i]>max){
			max=arr[i];
			index=i;
		}
	}
	return index;
}

