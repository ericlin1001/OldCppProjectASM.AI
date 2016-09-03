// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<cstring>
#include<string>
//
#include "Renju.h"
#include "RenjuSolver.h"
using namespace std;

class RenjuViwer{
public:
	RenjuViwer(Renju *r){this->renju=r;}
	void print(){
		cout<<"Renju's State(Turn To "<<(renju->trunToBlack()?"Black":"White")<<")"<<endl;
		cout<<' ';
		for(int k=0;k<renju->getNumCols();k++){
			if(k>=10)cout<<k;
			else cout<<' '<<k;
		}
		cout<<endl;
		for(int i=0;i<renju->getNumRows();i++){
			cout<<i;
			if(i<10)cout<<' ';
			for(int j=0;j<renju->getNumCols();j++){
				if(renju->isBlank(i,j)){
					cout<<' ';
				}else if(renju->get(i,j)==Renju::BLACK){
					cout<<'B';
				}else {
					cout<<'W';
				}
				cout<<' ';
			}
			cout<<endl;
		}
		cout<<endl;
	}
private:
	Renju *renju;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Renju r;
	RenjuViwer rv(&r);
	rv.print();
	CRenjuSolver s;
	CRenjuSolver::Arg args;
	s.setArgs(args);
	while(1){
		int row,col;
		cin>>row>>col;
		r.set(row,col);
		rv.print();
		int w;
		if((w=CRenjuSolver::getWinState(r))>0){
		}
		Renju::Action action =s.getAction(r);
		action.print();
		r.execute(&action);
		rv.print();
	}
	system("pause");
	return 0;
}

