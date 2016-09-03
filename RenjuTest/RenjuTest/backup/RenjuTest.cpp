// RenjuTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Renju.h"
#include "RenjuSolver.h"
#define Trace(m) cout<<#m"="<<(m)<<endl;
//
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


#define CASE2

#ifdef CASE2
//

//
void think(CRenjuSolver::Node *node){
	cout<<'.';
}
class RenjuSolverCreator{
private:
	vector<CRenjuSolver *>solvers;
	void createMap(CRenjuSolver::RenjuPatternDetector::MapType *m,double weights[23]){
		CRenjuSolver::RenjuPatternDetector::MapType turnToMeMap,turnToOtherMap;
		(*m)["111110"]=weights[0];//win 5
		(*m)["111112"]=weights[1];//win 5

		(*m)["011110"]=weights[2];//living 4
		(*m)["211110"]=weights[3];//to 4
		(*m)["211101"]=weights[4];//to 4
		(*m)["211011"]=weights[5];//to 4
		(*m)["210111"]=weights[6];//to 4

		(*m)["011100"]=weights[7];//living 3
		(*m)["011010"]=weights[8];//living jump 3

		(*m)["010101"]=weights[9];//
		(*m)["210101"]=weights[10];//
		(*m)["211100"]=weights[11];//sleep 3
		(*m)["211010"]=weights[12];//sleep jump 3
		(*m)["211001"]=weights[13];//sleep jump 3
		(*m)["210110"]=weights[14];//sleep jump 3
		(*m)["210011"]=weights[15];//sleep jump 3

		(*m)["001100"]=weights[16];//living 2
		(*m)["011000"]=weights[17];//living 2
		(*m)["010100"]=weights[18];//living jump 2
		(*m)["010010"]=weights[19];//living big jump 2
		(*m)["211000"]=weights[20];//sleep 2
		(*m)["210100"]=weights[21];//sleep jump 2
		(*m)["210010"]=weights[22];//sleep big jump 2
	}
public:
	CRenjuSolver* createSolver(double DefenceToAttack,int MaxAddedSearchOutLine,int MaxDepth,int MaxNumBrunch,double toMeWegihts[23],double toOtherWeight[23]){
		CRenjuSolver *psolver=new CRenjuSolver();
		CRenjuSolver::Arg arg;
		arg.DefenceToAttack=DefenceToAttack;
		arg.MaxAddedSearchOutLine=MaxAddedSearchOutLine;
		arg.MaxDepth=MaxDepth;
		arg.MaxNumBrunch=MaxNumBrunch;

		CRenjuSolver::RenjuPatternDetector::MapType turnToMeMap,turnToOtherMap;
		createMap(&turnToMeMap,toMeWegihts);
		createMap(&turnToOtherMap,toOtherWeight);
		arg.detector.setPatternMap(&turnToMeMap,&turnToOtherMap);

		psolver->setArgs(arg);
		psolver->setThinkFunc(think);
		solvers.push_back(psolver);
		return psolver;
	}
	~RenjuSolverCreator(){delSolvers();
	}
	void delSolver(CRenjuSolver *s){
		vector<CRenjuSolver *>::iterator i=find(solvers.begin(),solvers.end(),s);
		if(i!=solvers.end()){
			delete s;
			solvers.erase(i);
		}
	}
	void delSolvers(){
		vector<CRenjuSolver *>::iterator i;
		for(i=solvers.begin();i!=solvers.end();i++){
			delete (*i);
		}
		solvers.clear();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Renju r;
	RenjuViwer rv(&r);
	RenjuSolverCreator c;

	double toMeWeight[23]={
		INFINITE*3,	//0
		INFINITE*3,	//1
		INFINITE/10,		//2
		INFINITE/100,		//3
		INFINITE/100,		//4

		INFINITE/100,		//5
		INFINITE/100,		//6

		INFINITE/1000,		//7
		INFINITE/1000,		//8
		5,		//9

		5,		//10
		5,		//11
		5,		//12
		5,		//13
		5,		//14

		5,		//15
		3,		//16
		3,		//17
		2,		//18
		1,		//19

		0.5,		//
		0.3,		//
		0.1		//
	};
	//							0		
	double toOtherWegiht[23]={
		INFINITE*3,	//0
		INFINITE*3,	//1
		INFINITE/10,	//2

		135,		//3
		135,		//4

		130,		//5
		130,		//6

		133,		//7
		128,		//8

		5,		//9

		5,		//10
		5,		//11
		5,		//12
		5,		//13
		5,		//14

		5,		//15
		3,		//16
		3,		//17
		2,		//18
		1,		//19

		0.5,		//
		0.3,		//
		0.1		//
	};		
	CRenjuSolver *s=c.createSolver(1.5,2,5,5,toMeWeight,toOtherWegiht);
	//
	char first[10];
	cout<<"You first?(Y/N):";cin>>first;
	if(first[0]=='Y' || first[0]=='y'){
	}else{
		r.set(7,7);
	}
	//
	while(1){
		rv.print();
		int row,col;
		cout<<"row:";cin>>row;
		if(row==-1){
			vector<Renju::Action> seq=r.getSeq();
			for(int i=0;i<seq.size();i++){
				cout<<"("<<seq[i].row<<","<<seq[i].col<<");";
			}
			cout<<endl;
			continue;
		}else if(row==-2){
			char buffer[1000];
			cin>>buffer;
			r.run(buffer);
			rv.print();
			continue;
		}else if(row==-3){
			cout<<"example:(7,7);(7,6);"<<endl;
			r.init(15,15);
		}else{
			cout<<"col:";cin>>col;
			system("cls");
			if(!r.set(row,col)){cout<<"can't put stone in here"<<endl;continue;}
			rv.print();
			{
				cout<<"thinking,please wait"<<endl;
				CRenjuSolver::Node *node=s->getPNode(r);
				cout<<endl;

				Renju::Action action=node->action;
				double value=node->value;
				//action.print();cout<<"value:"<<value<<endl;
				if(r.isInRange(action.row,action.col)){
					cout<<"The computer put the stone in ";action.print();cout<<endl;
					r.execute(&action);
				}else{
					action.print();
					cout<<"Error: the computer can't calculate a correct action!"<<endl;
				}
				int w=s->CRenjuSolver::getWinState(r);
				if(w==1){
					cout<<"********************Black Win!!!********************"<<endl;
				}else if(w==2){
					cout<<"********************White Win!!!********************"<<endl;
				}else if(w==3){
					cout<<"*****************Black and White Win!!!******************"<<endl;
				}
				//Trace(w);
			}
		}
	}
	system("pause");
	return 0;
}
#endif
#ifdef CASE1
double testSolver(Renju &r,bool run=false){
	IntervalPrint t;
	t.start();
	//	cout<<"thinking....(about :"<<(r.trunToBlack()?"Black":"White")<<")"<<endl;
	Renju::Action action=RenjuSolver::getAction(r);
	cout<<endl<<"action:";action.print();cout<<endl;
	if(run)r.execute(&action);
	return (double)t.stop()/1000;
	//cout<<"used:"<<(double)t.stop()/1000<<" seconds"<<endl;
}

int _tmain(int argc, _TCHAR* argv[])
{

	char buffer[100]="0020000100110022";
	/*while(1){
	cin>>buffer;
	cout<<"value:"<<RenjuPatternDector::evalStr(buffer)<<endl;
	}*/
	//
	IntervalPrint t[100];
	//
	string ss="0123456789";
	cout<<ss.substr(2,3)<<endl;
	Node::init();
	//
	Renju r;

	RenjuViwer rv(&r);

	/*r.set(7,7);
	r.set(7,8);
	r.set(6,7);
	r.set(8,7);
	r.set(6,6);
	r.set(9,6);*/
	run(r,"(7,7);(7,6);(6,7);(5,6);(6,6);(8,8);");
	rv.print();

	//
	int row,col;
	RenjuSolver::setMaxDepth(4);//4
	RenjuSolver::setDefenceToAttack(1.5);
	RenjuSolver::setMaxNumBrunch(7);//7
	RenjuSolver::setMaxAddedSearchOutLine(2);
	//
	//r.set(0,0);
	int numTests=1000;
	double avg_cost=0;
	//
	Node node;
	node.setState(&r);
	double avg_createS_cost=0;
	//
	for(int i=0;i<numTests;i++){
		/*double cost=testSolver(r);
		avg_cost+=cost;
		cout<<"*****************cost:"<<cost<<"**********************"<<endl;*/
		//
		/*double ccost=0;
		t[0].start();
		node.createSuccessor();
		node.delSuccessors();
		ccost=t[0].stop();
		Trace(ccost);
		avg_createS_cost+=ccost;*/
		/*	t[5].start();
		cout<<"value:"<<RenjuPatternDector::evalStr(buffer)<<endl;
		double cost=t[5].stop();
		cout<<"cost:"<<cost<<endl;
		avg_cost+=cost;*/
	}
	avg_cost/=numTests;
	avg_createS_cost/=numTests;
	cout<<"avg_cost:"<<avg_cost<<endl;
	cout<<"avg_createS_cost:"<<avg_createS_cost<<endl;
	//
	rv.print();


	//	cout<<"cost:"<<testSolver(r,true)<<endl;

	rv.print();


	while(1){
		cout<<"row:";cin>>row;
		if(row==-1){
			vector<Renju::Action> seq=r.getSeq();
			for(int i=0;i<seq.size();i++){
				cout<<"("<<seq[i].row<<","<<seq[i].col<<");";
			}
			cout<<endl;
			continue;
		}else if(row==-2){
			while(1){
				char buffer[100];
				cin>>buffer;
				if(strcmp(buffer,"exit")==0)break;
				run(r,buffer);
				rv.print();
			}
			continue;
		}
		cout<<"col:";cin>>col;
		system("cls");
		if(!r.set(row,col)){cout<<"can't put stone in here"<<endl;continue;}
		rv.print();

		cout<<"cost:"<<testSolver(r,true)<<endl;

		rv.print();
	}
	system("pause");
	return 0;
}
#endif
/*
W W W
W B B B
B B W
B

5 7
6 5
5 9
5 8
4 6
9 6
*/
/*
(7,7);(7,8);(6,7);(8,7);(6,9);(6,8);(5,8);
(7,7);(7,6);(6,7);(5,6);(6,6);(8,8);(6,8);
(7,7);(7,8);(6,7);(8,7);(6,6);(9,6);(6,9);(6,8);(5,8);(8,8);(7,6);(8,5);(8,6);(9,8);(10,8);(9,7);(9,5);(10,7);(11,8);(7,9);(6,10);(11,6);(12,5);(10,5);(11,4);(10,6);(11,5);(12,7);(9,4);(11,7);(13,7);(8,10);(5,7);(8,9);(8,11);(7,10);
*/