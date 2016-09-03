// eightDigitalGamesTest1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<cstring>
#include<windows.h>
#include<cmath>
#include<vector>
#include<conio.h>
#include<algorithm>
using namespace std;
class CDigitalGame{

public://0 means the space.
	struct Point{
		int row,col;
	};
	CDigitalGame(){
	}
	void init(int rows,int cols){
		numRows=rows;
		numCols=cols;
		//maps.resize
		maps.resize(numRows);
		for(int i=0;i<this->numRows;i++){
			maps[i].resize(numCols);
		}
		for(int row=0;row<this->numRows;row++){
			for(int col=0;col<this->numCols;col++){
				maps[row][col]=row*numCols+col;
			}
		}
	}
	
	void init(int rows,int cols,int *arr){
		numRows=rows;
		numCols=cols;
		//maps.resize
		maps.resize(numRows);
		for(int i=0;i<this->numCols;i++){
			maps[i].resize(numCols);
		}
		assign(arr);
	}
	int &get(int row,int col){
		return maps[row][col];
	}
	int &get(Point p){
		return maps[p.row][p.col];
	}
	vector<vector<int>> getMaps(){
		return maps;
	}
	int getNumRows()const {
		return numRows;
	}
	int getNumCols()const {
		return numCols;
	}
	Point find(int f){
		Point p;p.row=p.col=-1;
		for(int row=0;row<this->numRows;row++){
			for(int col=0;col<this->numCols;col++){
				if(maps[row][col]==f){
					p.row=row;
					p.col=col;
				}
			}
		}
		return p;
	}
	//operator
	bool up(){
		Point spacePoint=this->find(0);
		Point changePoint=spacePoint;
		changePoint.row++;
		if(isInRange(changePoint)){
			swap(get(spacePoint),get(changePoint));
			return true;
		}
		return false;
	}
	bool down(){
		Point spacePoint=this->find(0);
		Point changePoint=spacePoint;
		changePoint.row--;
		if(isInRange(changePoint)){
			swap(get(spacePoint),get(changePoint));
			return true;
		}
		return false;
	}
	bool left(){
		Point spacePoint=this->find(0);
		Point changePoint=spacePoint;
		changePoint.col++;
		if(isInRange(changePoint)){
			swap(get(spacePoint),get(changePoint));
			return true;
		}
		return false;
	}
	bool right(){
		Point spacePoint=this->find(0);
		Point changePoint=spacePoint;
		changePoint.col--;
		if(isInRange(changePoint)){
			swap(get(spacePoint),get(changePoint));
			return true;
		}
		return false;
	}
private:
	inline bool isInRange(int row,int col){
		return 0<=row && row<numRows && 0<=col && col<numCols;
	}
	inline bool isInRange(Point p){
		return isInRange(p.row,p.col);
	}
	void assign(int *arr){
		for(int row=0;row<this->numRows;row++){
			for(int col=0;col<this->numCols;col++){
				maps[row][col]=arr[row*this->getNumCols()+col];
			}
		}
	}
	int numCols,numRows;
	vector<vector<int>> maps;

};
#define Trace(m) cout<<#m"="<<(m)<<endl;

#include<iostream>
using namespace std;
int getIntLen(int n){
	int len=0;
	while(n){len++;n/=10;}
	return len;
}
void printNSpace(int n){
	for(int i=0;i<n;i++)cout<<" ";
}
void print(CDigitalGame g){
	int max=g.getNumCols()*g.getNumRows()-1;
	int maxlen=getIntLen(max);
	for(int row=0;row<g.getNumRows();row++){
		for(int col=0;col<g.getNumCols();col++){
			int num=g.get(row,col);

			int preSpace;
			if(col==0)preSpace=0;
			else preSpace=1;

			if(num!=0){
				printNSpace(preSpace+maxlen-getIntLen(num));
				cout<<num;
			}
			else{
				printNSpace(preSpace+maxlen-getIntLen(num));
			}
		}
		cout<<endl;
	}
}
void printGame(CDigitalGame g){
	int max=g.getNumCols()*g.getNumRows()-1;
	int maxlen=getIntLen(max);
	for(int row=0;row<g.getNumRows();row++){
		for(int col=0;col<g.getNumCols();col++){
			int num=g.get(row,col);

			int preSpace;
			if(col==0)preSpace=0;
			else preSpace=1;

			if(num!=0){
				printNSpace(preSpace+maxlen-getIntLen(num));
				cout<<num;
			}
			else{
				printNSpace(preSpace+maxlen-getIntLen(num));
			}
		}
		cout<<endl;
	}
}
bool move(CDigitalGame &g,int dir){
	bool b=true;
	if(dir==0){
		b&=g.left();
	}else if(dir==1){
		b&=g.up();
	}else if(dir==2){
		b&=g.right();
	}else if(dir==3){
		b&=g.down();
	}else {
		return false;
	}
	return b;
}
bool move(CDigitalGame &g,vector<int> dirs){
	bool b=true;
	for(int i=0;i<dirs.size();i++){
		move(g,dirs[i]);
	}
	return b;
}
bool isWin(CDigitalGame g){
	for(int row=0;row<g.getNumRows();row++){
		for(int col=0;col<g.getNumCols();col++){
			if(g.get(row,col)!=row*g.getNumCols()+col)return false;
		}
	}
	return true;
}
int ManHanDonH(CDigitalGame g){
	int h=0;
	for(int row=0;row<g.getNumRows();row++){
		for(int col=0;col<g.getNumCols();col++){
			int num=g.get(row,col);
			int rightrow,rightcol;
			if(num!=0){
			rightrow=num/g.getNumCols();
			rightcol=num%g.getNumCols();
			h+=abs(row-rightrow)+abs(col-rightcol);
			}
		}
	}
	return h;
}
struct Node{
	Node(CDigitalGame g){
		state=g;
	}
	//
	CDigitalGame state;
	int action;
	//
	vector<Node *>childs;
	Node *parent;
	//
	int g,h,f;
	bool operator<(Node &n){
		return f<n.f;
	}
	bool operator>(Node &n){
		return f>n.f;
	}
	void addChild(Node *node){
		node->parent=this; 
		this->childs.push_back(node);
	} 
	void print(){
		cout<<"Node:"<<endl;
		Trace(g);
		Trace(h);
		Trace(f);
		printGame(state);
	}
};
Node *head=NULL;
//Node *tail=NULL;
Node *makeNode( CDigitalGame g){
	Node *newNode=new Node(g);
	if(head==NULL){
		newNode->g=0;
		newNode->h=ManHanDonH(newNode->state);
		newNode->parent=NULL;
		newNode->action=-1;
		newNode->f=newNode->g+newNode->h;
		head=newNode;
	}
	return newNode;
}
//#define INFINITE 10000
struct Recursive_Best_First_Search_Returns{
	vector<int> solution;
	bool bFailure;
};
struct RBFS_Returns{
	Node *node;
	bool bFailure;
	int f_limit;
};

vector<Node *> expand(Node *node){
	for(int i=0;i<4;i++){
		CDigitalGame g=node->state;
		if(move(g,i)){
			Node *newNode=new Node(g);
			node->addChild(newNode);
			newNode->action=i;
			newNode->g=newNode->parent->g+1;
			newNode->h=ManHanDonH(newNode->state);
		}
	}
	return node->childs;
}
bool NodeComp(Node*node1,Node *node2){
	return node1->f < node2->f;
}

int RBFS_Recurving_Count=0;
#define _SHOW_TEST_OUTPUT 
RBFS_Returns RBFS(CDigitalGame problem,Node *node,int f_limit){
	RBFS_Recurving_Count++;
#ifdef SHOW_TEST_OUTPUT
	cout<<"In RBFS:***"<<RBFS_Recurving_Count<<"****"<<endl;
	cout<<"RBFS(node) node:"<<endl;node->print();
#endif
	RBFS_Returns returns;
	if(isWin(node->state)){cout<<"find a win's state!"<<endl;returns.bFailure=false;returns.node=node;return returns;}
	vector<Node *> successors=expand(node);

	if(successors.empty()){cout<<"faild:No successors"<<endl;returns.bFailure=true;return returns;}

	for(int i=0;i<successors.size();i++){
		Node *s=successors[i];
		s->f=max(s->g+s->h,node->f);
	}sort(successors.begin(),successors.end(),NodeComp);
	#ifdef SHOW_TEST_OUTPUT
	for(int i=0;i<successors.size();i++){
		Node *s=successors[i];
		cout<<"successors["<<i<<"]:"<<endl;s->print();
	}
	#endif
	while(1){
		//
		Node *best,*second_lowestNode;
		best=second_lowestNode=successors[0];
		if(successors.size()>1)second_lowestNode=successors[1];
		
		if(best->f>f_limit){ returns.bFailure=true;returns.f_limit=best->f;return returns;}
		int alternative=second_lowestNode->f;
		RBFS_Returns recurReturns=RBFS(problem,best,min(f_limit,alternative));
		if(!recurReturns.bFailure)return recurReturns;
	}
}
void deleteTree(Node *&node){
	if(node==NULL)return;
	for(int i=0;i<node->childs.size();i++){
		deleteTree(node->childs[i]);
	}
	delete node;
	node=NULL;
}

#include<time.h>
class CTimer{
public:
	CTimer(){
		lasttime=0;
	}
	clock_t getTime(){
		clock_t delay=0;
		if(lasttime==0){
			lasttime=clock();
		}else{
			clock_t nowtime=clock();
			delay=nowtime-lasttime;
			lasttime=nowtime;
		}
		return delay;

	}
	clock_t lasttime;
};

int RBSdepth;
int RBStime;
int nodeCount;
Recursive_Best_First_Search_Returns Recursive_Best_First_Search(CDigitalGame problem){
	Recursive_Best_First_Search_Returns returns;
	RBFS_Recurving_Count=0;
	CTimer timer;
	timer.getTime();
	RBFS_Returns r=RBFS(problem,makeNode(problem),1000000);
	RBStime=timer.getTime();
	/*cout<<"Using time:"<<timer.getTime()<<" ms"<<endl;
	cout<<"RBFS_Recurving_Count:"<<RBFS_Recurving_Count<<endl;*/
	//
	if(r.bFailure){
		returns.bFailure=true;
		cout<<"Fail in Recursive_Best_First_Search()"<<endl;
	}else{
		Node *node=r.node;
		returns.solution.clear();
		int depth=0;
		while(node->parent!=NULL){
			depth++;
			returns.solution.push_back(node->action);
			node=node->parent; 
		}
		RBSdepth=depth;
		//cout<<"Depth:"<<RBSdepth<<endl;
		reverse(returns.solution.begin(),returns.solution.end());
		returns.bFailure=false;
	}
	deleteTree(head);
	return returns;
}
#define Find_Solution 
int _tmain(int argc, _TCHAR* argv[])
{
	const int numCases=10;
	int depth[100];
	int times[100];
	int RBSRevokeCounts[100];
	int pCase=0;
	while(pCase<numCases){
	CDigitalGame g;
	int myGird[3][3]={
		{1,0,2},
		{3,4,8},
		{6,5,7}
	};//solution={1,1,0,3,2,3,2}


	//g.init(3,3,&myGird[0][0]);
	g.init(3,3);
	//
	vector<int> randDirs;
	//
	int preDirs[100]={0,0,1,2};
#define _MANUL 
#ifdef MANUL
	cout<<"randDirs:";
	for(int i=0;i<1;i++){
		int randDir=preDirs[i]
		cout<<randDir<<",";
		randDirs.push_back(randDir);
	}
	cout<<endl;
#else
	cout<<"randDirs:";
	 srand ( time(NULL));
	for(int i=0;i<20;i++){
		int randDir=rand()%4;
		cout<<randDir<<",";
		randDirs.push_back(randDir);
	}cout<<endl;
#endif
	//
	move(g,randDirs);
	cout<<"Init:"<<endl;print(g);


#ifdef Find_Solution
	cout<<"Start solving..."<<endl;
	Recursive_Best_First_Search_Returns returns=Recursive_Best_First_Search(g);
	if(returns.bFailure){
		cout<<"Fail to find a solution..."<<endl;
	}else{
		cout<<"Find a solution:"<<endl;
		for(int i=0;i<returns.solution.size();i++){
			cout<<returns.solution[i]<<",";
		}
		cout<<endl;
		move(g,returns.solution);
		cout<<"After the solution:"<<endl;
		print(g);
		if(isWin(g)){cout<<"You win!!!"<<endl;}
	}

	 depth[pCase]=RBSdepth;
	 times[pCase]=RBStime;
	 RBSRevokeCounts[pCase]=RBFS_Recurving_Count;
#endif
	pCase++;
	
	#ifndef Find_Solution
	while(1){
		int dir;
		//cout<<"dir:";
		char ch;
		if(kbhit()){
			ch=getch();
			dir=ch-'0';
			
			move(g,dir);
		}
		system("cls");print(g);
		if(isWin(g)){cout<<"You win!!!"<<endl;break;}
		Sleep(50);
	}
#endif
	}

	//
	cout<<"Case\tDepth\tTime\tRevokeCount"<<endl;
	for(int i=0;i<numCases;i++){
		cout<<i<<"\t"<<depth[i]<<"\t"<<times[i]<<"\t"<<RBSRevokeCounts[i]<<endl;
	}
	double ave_depth=0,ave_times=0,ave_RBSRevokeCounts=0;
	for(int i=0;i<numCases;i++){
		ave_depth+=depth[i];
		ave_times+=times[i];
		ave_RBSRevokeCounts+=RBSRevokeCounts[i];
	}
	ave_depth/=numCases;
		ave_times/=numCases;
		ave_RBSRevokeCounts/=numCases;
	cout<<"average"<<"\t"<<ave_depth<<"\t"<<ave_times<<"\t"<<ave_RBSRevokeCounts<<endl;

	system("pause");
	return 0;
}
/*

if(GetAsyncKeyState('A')){
cout<<"you press A"<<endl;
}*/

/*if(dir==4){
				g.left();
			}else if(dir==8){
				g.up();
			}else if(dir==6){
				g.right();
			}else if(dir==5){
				g.down();
			}else {
			}*/