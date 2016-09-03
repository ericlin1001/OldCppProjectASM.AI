#include "stdafx.h"
#include "Renju.h"

void Renju::init(int row,int col){
	this->numRows=row;
	this->numCols=col;
	maps.resize(row);
	for(int i=0;i<numRows;i++){
		maps[i].resize(col);
		for(int j=0;j<numCols;j++){
			maps[i][j]=STONE_TYPE::BLANK;
		}
	}
	nextNum=1;
	maxOutLine=0;
}
int Renju::get(int row,int col){if(maps[row][col]==0)return 0;return (getNum(row,col)+1)%2+1;}
bool Renju::set(int row,int col){
	if(isBlank(row,col)){
		maps[row][col]=nextNum++;
		maxOutLine=max(maxOutLine,abs(row-7));
		maxOutLine=max(maxOutLine,abs(col-7));
		return true;
	}return false;
}
void Renju::run(const char *seqs){//"(1,1);(2,2)
		Renju::Action action;
		while(*seqs){
			if(sscanf(seqs,"(%d,%d)",&action.row,&action.col)){
				execute(&action);
			}
			seqs++;
		}
}
vector<Renju::Action> Renju::getSeq(){
	vector<Action> seqs;
	seqs.resize((nextNum-1));
	for(int i=0;i<numRows;i++){
		for(int j=0;j<numCols;j++){
			if(!isBlank(i,j)){
				int num=maps[i][j];
				seqs[num-1].row=i;
				seqs[num-1].col=j;
			}
		}
	}
	return seqs;
}