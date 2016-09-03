#include "stdafx.h"
#ifndef RENJU_H
#define RENJU_H
#include<vector>
#include<iostream>
using namespace std;
class Renju{
public:
	enum STONE_TYPE{BLANK,BLACK,WHITE};
	struct Action{
		Action(){row=0;col=0;}
		int row, col;
		void print(){cout<<"("<<row<<","<<col<<")";}
	};
	Renju(int row=15,int col=15){init(row,col);}
	inline int getNumRows()const {return numRows;}
	inline int getNumCols()const {return numCols;}
	inline bool trunToBlack(){return nextNum%2==1;}
	inline bool trunToWhite(){return nextNum%2==0;}
	inline int turnToWho(){return ((nextNum%2==1)?1:2);};
	inline bool isBlank(int row,int col){return maps[row][col]==0;}
	inline int getNum(int row,int col)const{return maps[row][col];}
	inline bool reset(int row,int col){if(maps[row][col]==nextNum-1){maps[row][col]=0;nextNum--;return true;}return false;}
	inline bool execute(Action *action){return set(action->row,action->col);}
	inline bool unexecute(Action *action){ return reset(action->row,action->col);}
	inline int getMaxOutLine()const {return maxOutLine;}
	inline bool isInRange(int row,int col){return 0<=row && row<this->getNumRows() && 0<=col && col<this->getNumCols();}
	//
	void init(int row,int col);
	int get(int row,int col);
	bool set(int row,int col);
	
	vector<Action> getSeq();
	//
	void run(const char *seqs);

private:
	int numRows;
	int numCols;
	int nextNum;
	vector<vector<int>> maps;
	int maxOutLine;
};

//just for test:
/*
class RenjuViwer{
public:
	RenjuViwer(Renju *r){this->renju=r;}
	void print(){
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
};*/
#endif