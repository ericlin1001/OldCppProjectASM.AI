// RenjuTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Renju.h"
#include "RenjuSolver.h"
#define Trace(m) cout<<#m"="<<(m)<<endl;
#include<cstdlib>
#include<fstream>
#include<vector>
using namespace std;
using namespace std;
#define CASE5
void think(CRenjuSolver::Node *node){
	cout<<'.';
}
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
class FightArea{
	Renju r;
	CRenjuSolver *black,*white;
	int w;
	bool isPrintProcess;
	Renju::Action action;
public:
	FightArea(){isPrintProcess=false;}
	void setIsPrintProcess(bool b){isPrintProcess=b;}
	void setFighters(CRenjuSolver *black,CRenjuSolver *white){
		this->black=black;
		this->white=white;
	}
	void startFight(){
		r.init(15,15);
		cout<<"\""<<black->getName()<<"\" is playing the Black"<<endl;
		cout<<"\""<<white->getName()<<"\" is playing the White"<<endl;
		while(!r.isFull()){
			if((w=CRenjuSolver::getWinState(r))>0)break;
			action=black->getAction(r);
			r.execute(&action);
			if(isPrintProcess){action.print();cout<<endl;RenjuViwer(&r).print();}

			if((w=CRenjuSolver::getWinState(r))>0)break;
			action=white->getAction(r);
			r.execute(&action);
			if(isPrintProcess){action.print();cout<<endl;RenjuViwer(&r).print();}
		}
	}
	inline int getWinState(){return w;}
	void swapFighter(){
		CRenjuSolver *t =black;
		black=white;
		white=black;
	}
	void print(){
		if(w==1){
			cout<<"********************Black Win!!!********************"<<endl;
		}else if(w==2){
			cout<<"********************White Win!!!********************"<<endl;
		}else if(w==3){
			cout<<"*****************Black and White Win!!!******************"<<endl;
		}else if(w==0){
			cout<<"***********Nobody win!!!**************"<<endl;
		}
		RenjuViwer(&r).print();
		cout<<"seqs:";
		vector<Renju::Action> seq=r.getSeq();
		for(int i=0;i<(int)seq.size();i++){
			cout<<"("<<seq[i].row<<","<<seq[i].col<<");";
		}
		cout<<endl;
	}
	bool isBetter(CRenjuSolver *A,CRenjuSolver *B){
		bool b;
		if(rand()%2==0){
			setFighters(A,B);
			b=true;
		}else{
			setFighters(B,A);
			b=false;
		}
		startFight();
		if(getWinState()==1){cout<<"Black win"<<endl;return b?true:false;}
		else if(getWinState()==2){cout<<"White win"<<endl;return b?false:true;}
		return rand()%2==0;
	}
};
class Environment{
private:
	RenjuSolverCreator creator;
	string name;
	Gene parentGenePool[10];
	Gene childGenePool[10];
	//
	bool isEvolved;
private:
	void createChildGenePool(){//变异,
		for(int i=0;i<10;i++){
			int a,b;
			a=b=rand()%10;
			while(a==b){
				b=rand()%10;
			}
			cout<<"create child["<<i<<"] from parent["<<a<<","<<b<<"]"<<endl;
			childGenePool[i].createFromParent(parentGenePool+a,parentGenePool+b);
			cout<<endl;
		}
	}
	void setParentGenesName(){
		for(int i=0;i<10;i++){
			string n="";
			n="parentGene";
			char stri[10];
			_itoa_s(i,stri,10);
			n+=stri;
			parentGenePool[i].setName(n.c_str());
		}
	}
	void setChildGenesName(){
		for(int i=0;i<10;i++){
			string n="";
			n="childGene";
			char stri[10];
			_itoa_s(i,stri,10);
			n+=stri;
			childGenePool[i].setName(n.c_str());
		}
	}
	void selectGoodGeneAsParent(){//自然选择
		setParentGenesName();
		setChildGenesName();
		//
		vector<int>fightOrder=createRandSeqs(20);
		Gene betterGenes[10];
		for(int i=0;i<10;i++){
			int a=fightOrder[i*2];
			int b=fightOrder[i*2+1];
			Gene *ga,*gb;
			ga=getGene(a);
			gb=getGene(b);
			//
			cout<<"round("<<i<<"):"<<ga->getName()<<" VS "<<gb->getName()<<endl;
			if(isBetter(ga,gb)){
				cout<<ga->getName()<<" defeat "<<gb->getName()<<endl;
				betterGenes[i]=*ga;
			}else{
				cout<<gb->getName()<<" defeat "<<ga->getName()<<endl;
				betterGenes[i]=*gb;
			}
			cout<<endl;
		}
		cout<<"Better gene:";
		int numBetterParent=0;
		for(int i=0;i<10;i++){
			parentGenePool[i]=betterGenes[i];
			if(parentGenePool[i].getName().find("parent")!=-1){
				numBetterParent++;
			}
			cout<<parentGenePool[i].getName()<<",";
		}
		cout<<"numBetterParent="<<numBetterParent<<endl;
		if(numBetterParent>=8){
			cout<<"numBetterParent>=8,so isEvolved=true"<<endl;
			isEvolved=true;
		}
		cout<<endl;
	}
	void selectBestGeneFromParent(){
		setParentGenesName();
		//
		vector<int>fightOrder=createRandSeqs(10);
		Gene goodGenes[5];Gene badGenes[5];
		selectHalf(10,parentGenePool,goodGenes,badGenes);
		selectHalf(6,parentGenePool,goodGenes,badGenes);
		selectHalf(4,parentGenePool,goodGenes,badGenes);
		selectHalf(2,parentGenePool,goodGenes,badGenes);
		//
		cout<<endl;
	}
	void selectHalf(int num,Gene *genepool,Gene *goodgenes,Gene *badgenes){
		vector<int>fightOrder=createRandSeqs(num);
		int half_num=num/2;
		for(int i=0;i<half_num;i++){
			int a=fightOrder[i*2];
			int b=fightOrder[i*2+1];
			Gene *ga,*gb;
			ga=genepool+a;
			gb=genepool+b;
			//
			cout<<ga->getName()<<" VS "<<gb->getName()<<endl;
			if(isBetter(ga,gb)){
				cout<<ga->getName()<<" defeat "<<gb->getName()<<endl;
				goodgenes[i]=*ga;badgenes[i]=*gb;
			}else{
				cout<<gb->getName()<<" defeat "<<ga->getName()<<endl;
				goodgenes[i]=*gb;badgenes[i]=*ga;
			}
			cout<<endl;
		}
		for(int i=0;i<num;i++){
			genepool[i]=(i< half_num?goodgenes[i]:badgenes[i-half_num]);
			cout<<genepool[i].getName()<<",";
		}
	}
	bool isBetter(Gene *ga,Gene *gb){
			bool b=true;
			creator.setGlobalArgs(1,2,4,4);
			CRenjuSolver *sa=creator.createSolverFromGene(ga);
			CRenjuSolver *sb=creator.createSolverFromGene(gb);

			FightArea area;
			//area.setIsPrintProcess(true);
			if(area.isBetter(sa,sb)){
				b=true;
			}else{
				b=false;
			}
			creator.delSolver(sa);
			creator.delSolver(sb);
			return b;
	}
	vector<int> createRandSeqs(int n){//0 to n-1
		vector<int>num;
		for(int i=0;i<n;i++){
			num.push_back(i);
		}
		vector<int>seqs;
		for(int i=0;i<n;i++){
			int index=rand()%num.size();
			seqs.push_back(num[index]);
			num.erase(num.begin()+index);
		}
		return seqs;
	}
public:
	Gene *getGene(int index){//0-19
		if(index>=10){
			return childGenePool+(index-10);
		}else{
			return parentGenePool+index;
		}
	}
	bool startEvolve(int times){
		isEvolved=false;
		if(!readGenes()){
			cout<<"Error:can't readGenes()"<<endl;
			return false;
		}
		for(int i=0;i<times;i++){
			cout<<endl;
			cout<<"*********************Evolve"<<i<<" start *********************"<<endl;
			cout<<"createChildGenePool:"<<endl;
			createChildGenePool();
			cout<<endl;
			cout<<"selectGoodGenes:"<<endl;
			selectGoodGeneAsParent();
			if(this->saveGenes())cout<<"****************Evolve"<<i<<" Have saved!*****************"<<endl;
			cout<<endl;
			if(isEvolved){
				cout<<"Have been evolved to a some perfect state!"<<endl;
				break;
			}
		}
		selectBestGeneFromParent();
		return true;
	}
	inline Gene *getGene(){return parentGenePool;}
	Environment(){
		creator.setGlobalArgs(1,2,4,4);
		name="Environment";
		isEvolved=false;
	}
	void setName(string s){name=s;}
	const string getName(){return name;}
	void init(Gene *pg){
		for(int i=0;i<10;i++){
			parentGenePool[i]=*pg;
		}
		setParentGenesName();
	}
	
	bool saveGenes(){
		setParentGenesName();
		for(int i=0;i<10;i++){
			string filename="";
			filename+=name;
			filename+="-";
			filename+=parentGenePool[i].getName();
			filename+=".txt";
			parentGenePool[i].save(filename.c_str());
		}
		return true;
	}
	bool readGenes(){
		setParentGenesName();
		bool b=true;
		for(int i=0;i<10;i++){
			string filename="";
			filename+=name;
			filename+="-";
			filename+=parentGenePool[i].getName();
			filename+=".txt";
			if(!parentGenePool[i].read(filename.c_str()))b=false;
		}
		return b;
	}
	void print(){
		for(int i=0;i<10;i++){
			parentGenePool[i].print();
		}
	}

};
bool testIsWin(Renju *r){
	int w=CRenjuSolver::getWinState(*r);
	if(w==1){
		cout<<"********************Black Win!!!********************"<<endl;
	}else if(w==2){
		cout<<"********************White Win!!!********************"<<endl;
	}else if(w==3){
		cout<<"*****************Black and White Win!!!******************"<<endl;
	}else{
		return false;
	}
	return true;

}

bool playChess(CRenjuSolver *s){//true mean the computer win.
	bool isComputerWin=true;
	Renju r;
	RenjuViwer rv(&r);
	//
	char first[10];
	cout<<"You first?(Y/N):";cin>>first;
	if(first[0]=='Y' || first[0]=='y'){
		cout<<"You play Black."<<endl;
	}else{
		cout<<"You play White."<<endl;
		r.set(7,7);
	}
	//
	while(1){
		if((r.getNumStone()%12)==8||(r.getNumStone()%12)==9){
			s->getArgs()->MaxDepth+=1;
			s->getArgs()->MaxNumBrunch+=1;
		}
		if(r.isFull())break;
		rv.print();
		int row,col;
		cout<<"row:";cin>>row;
		if(row==-1){
			vector<Renju::Action> seq=r.getSeq();
			for(int i=0;i<(int)seq.size();i++){
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
		}else if(row==-4){
			cout<<"thinking,please wait"<<endl;
			CRenjuSolver::Node *node=s->getPNode(r);
			cout<<endl;
			Renju::Action action=node->action;
			double value=node->value;
			if(r.isInRange(action.row,action.col)){
				cout<<"The computer put the stone in ";action.print();cout<<endl;
				r.execute(&action);
			}else{cout<<"Error: the computer can't calculate a correct action!"<<endl;}
			if(testIsWin(&r))break;
			continue;
		}else if(row==-3){
			cout<<"example:(7,7);(7,6);"<<endl;
			r.init(15,15);
		}else{//the normal mode.
			cout<<"col:";cin>>col;
			system("cls");
			if(!r.set(row,col)){cout<<"can't put stone in here"<<endl;continue;}
			rv.print();
			int w;
			w=CRenjuSolver::getWinState(r);
			if(w==1){
				isComputerWin=false;
				cout<<"********************You(Black) Win!!!********************"<<endl;break;
			}else if(w==2){
				isComputerWin=false;
				cout<<"********************You(White) Win!!!********************"<<endl;break;
			}
			{
				cout<<"thinking,please wait"<<endl;
				CRenjuSolver::Node *node=s->getPNode(r);
				cout<<endl;
				Renju::Action action=node->action;
				double value=node->value;
				if(r.isInRange(action.row,action.col)){
					cout<<"The computer put the stone in ";action.print();cout<<endl;
					r.execute(&action);
				}else{cout<<"Error: the computer can't calculate a correct action!"<<endl;}
				//if(testIsWin(&r))break;
				w=CRenjuSolver::getWinState(r);
				if(w==1){
					rv.print();
					cout<<"********************Computer(Black) Win!!!********************"<<endl;break;
				}else if(w==2){
					rv.print();
					cout<<"********************Computer(White) Win!!!********************"<<endl;break;
				}
				//
			}
		}
	}
	return isComputerWin;
}

#ifdef CASE5
int main(int argc,char *argv[])
{
	srand((unsigned)time(NULL));
	Environment en;en.setName("TestEnvironment");
	en.readGenes();
	//
	RenjuSolverCreator c;
	c.setGlobalArgs(1.5,2,4,4);
	int geneIndex=0;
	CRenjuSolver *s;
	Gene *g;
	do{
		if(geneIndex>=5){//
			geneIndex=0;
			//
			int times;
			cout<<"Actually it can evolved by itself,do you want it to evolve(This evolution may cost time)?"<<endl;
			cout<<"Evolve times:";cin>>times;
			if(en.startEvolve(times)){
				cout<<"Evolve() success"<<endl;
			}else{
				cout<<"Evolve() fail!"<<endl;
			}
			//
		}
		g=en.getGene(geneIndex++);
		s=c.createSolverFromGene(g);
		cout<<"use this:";g->print();cout<<endl;
	}while(!playChess(s));
	//
	

	system("pause");
	return 0;
}
#endif
#ifdef CASE4
int main(int argc,char *argv[])
{
	srand((unsigned)time(NULL));
	Gene g;
	if(argc==2){
		if(g.read(argv[1])){
		RenjuSolverCreator c;
		c.setGlobalArgs(1,2,7,5);
		CRenjuSolver *s=c.createSolverFromGene(&g);//
		playChess(s);
		}
	}else{
		cout<<"usage:XXX.exe fileName"<<endl;
	}
	system("pause");
	return 0;
}
#endif
#ifdef CASE3
int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));
	Environment en;en.setName("TestEnvironment");
	int times;
	cout<<"Evolve times:";cin>>times;
	if(en.startEvolve(times)){
		cout<<"Evolve() success"<<endl;
	}else{
		cout<<"Evolve() fail!"<<endl;
	}
	//
	RenjuSolverCreator c;
	c.setGlobalArgs(1.5,2,4,4);
	CRenjuSolver *s=c.createSolverFromGene(en.getGene());
	cout<<"use this:";en.getGene()->print();cout<<endl;
	//
	playChess(s);

	system("pause");
	return 0;
}
#endif

//can play
#ifdef CASE2

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
