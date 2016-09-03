#include "stdafx.h"
#include "RenjuSolver.h"
//
#include "Renju.h"
#include<vector>
#include<iostream>
#include<map>
#include<algorithm>
#include<cstring>
#include <string> 
#include<hash_map>
#include<xutility>
#include<time.h>
#include<fstream>
using namespace std;
#define Trace(m) cout<<#m"="<<(m);//<<endl;
#ifndef INFINITE
#define INFINITE 10000000
#endif
#define _SHOW_SEARCH_INFO
#define _TEST
//
//

class IntervalPrint{
public:
	void print(){
		cout<<"Interval("<<stop()<<" milliseconds)"<<endl;
	}
	void start(){
		startTime=clock();
	}
	clock_t get(){
		return clock()-startTime;
	}
	clock_t stop(){
		clock_t now=clock();
		clock_t e=now-startTime;
		startTime=now;
		return e;
	}
private:
	clock_t startTime;
};

CRenjuSolver::RenjuPatternDetector::RenjuPatternDetector(){
	if(default_patternMap[0].size()==0){
		MapType tempm;
		tempm["111110"]=INFINITE;//win 5
		tempm["111112"]=INFINITE;//win 5

		tempm["011110"]=10000;//living 4
		tempm["211110"]=140;//to 4
		tempm["211101"]=135;//to 4
		tempm["211011"]=135;//to 4
		tempm["210111"]=135;//to 4

		tempm["011100"]=120;//living 3
		tempm["011010"]=100;//living jump 3
		tempm["010101"]=5;//
		tempm["210101"]=5;//
		tempm["211100"]=5;//sleep 3
		tempm["211010"]=5;//sleep jump 3
		tempm["211001"]=5;//sleep jump 3
		tempm["210110"]=5;//sleep jump 3
		tempm["210011"]=5;//sleep jump 3

		tempm["001100"]=3;//living 2
		tempm["011000"]=3;//living 2
		tempm["010100"]=2;//living jump 2
		tempm["010010"]=1;//living big jump 2
		tempm["211000"]=0.5;//sleep 2
		tempm["210100"]=0.3;//sleep jump 2
		tempm["210010"]=0.1;//sleep big jump 2
		//num:19
		copyWithReverse(this->default_patternMap,&tempm);
		copyWithReverse(this->default_patternMap+1,&tempm);
	}
}
void CRenjuSolver::RenjuPatternDetector::setPatternMap(MapType *turnToMeMap,MapType *turnToOtherMap){
	bufferPatternMap->clear();
	(bufferPatternMap+1)->clear();
	patternMap->clear();
	(patternMap+1)->clear();
	//
	copyWithReverse(patternMap,turnToMeMap);
	copyWithReverse(patternMap+1,turnToOtherMap);
	//
#ifdef TEST
	cout<<"toMe Map:"<<endl;

	for(MapType::iterator i=patternMap[0].begin();i!=patternMap[0].end();i++){
		cout<<i->first<<"="<<i->second<<endl;
	}
#endif
#ifdef TEST
	cout<<"toOther Map:"<<endl;
	for(MapType::iterator i=patternMap[1].begin();i!=patternMap[1].end();i++){
		cout<<i->first<<"="<<i->second<<endl;
	}/**/
#endif
}
double CRenjuSolver::RenjuPatternDetector::evalStr(const char *pattern,bool turnToMe){
	if(turnToMe){
#ifdef TEST
		cout<<"use to me"<<endl;
#endif
		pPatternMap=patternMap;
		pBufferPatternMap=bufferPatternMap;
	}else{
#ifdef TEST
		cout<<"use to other"<<endl;
#endif
		pPatternMap=patternMap+1;
		pBufferPatternMap=bufferPatternMap+1;
	}
	double v=0;
	char buffer[20];
	strcpy(buffer,pattern);
	//
	int len=strlen(buffer);
	char *lastp=buffer;
	for(int i=0;i<len;i++){
		if(buffer[i]=='2'){
			buffer[i]=0;
			v+=getStrWeight(lastp);
			lastp=buffer+i+1;
		}
	}
	v+=getStrWeight(lastp);
	return v;
}

void CRenjuSolver::RenjuPatternDetector::copyWithReverse(MapType *dest,MapType *src){
	for(MapType::iterator i=src->begin();i!=src->end();i++){
		string key=(string)i->first;
		string rkey=key;
		reverse(rkey.begin(),rkey.end());
		(*dest)[key]=(*src)[key];
		(*dest)[rkey]=(*src)[key];
	}
}
double CRenjuSolver::RenjuPatternDetector::getStrWeight(char *bits){
	if(!cut_off_zero(bits))return 0;
	string s=string(bits);
	if(pBufferPatternMap->find(s)==pBufferPatternMap->end()){(*pBufferPatternMap)[s]=getUnKnowStrWegiht(bits);}
	return (*pBufferPatternMap)[s];
}
bool CRenjuSolver::RenjuPatternDetector::cut_off_zero(char *&str){
	int len=strlen(str);
	if(len<=4)return false;
	char *p=str;
	while(*p=='0')p++;
	if(p>str+3){str=p-3;}
	//
	len=strlen(str);
	if(len<=4)return false;
	//
	p=str+len-1;
	while(*p=='0' && p>str)p--;
	if((str+len-1)>(p)+3){*(p+4)=0;}
	return true;
}
double CRenjuSolver::RenjuPatternDetector::getUnKnowStrWegiht(const char *bits){
	char buffer[20];
	int len=strlen(bits);
	strcpy(buffer+1,bits);
	buffer[0]='2';
	buffer[len+1]='2';
	buffer[len+2]=0;
	//
	double v=0;
	char temp[7];
	temp[6]=0;
	for(int i=0;i<len+2-5;i++){
		strncpy(temp,buffer+i,6);
		string s=string(temp);
		if(pPatternMap->find(s)!=pPatternMap->end()){
			v+=(*pPatternMap)[s];
			//	cout<<"find pattern:"<<s<<endl;
			i+=2;
		}
	}
	return v;
}



CRenjuSolver::RenjuPatternDetector::MapType CRenjuSolver::RenjuPatternDetector::default_patternMap[2];

inline Value_Type CRenjuSolver::Node::evalStr(char *pattern,bool turnToMe){return pargs->detector.evalStr(pattern,turnToMe);}


Value_Type CRenjuSolver::Node::eval(){//from the black's view.
	Value_Type  w=(pargs->DefenceToAttack+0.001);//the w bigger,the more defence.
	value=eval(1)*(Value_Type)(pstate->trunToBlack()?w:1)-eval(2)*(Value_Type)(pstate->trunToWhite()?w:1);
	return value;
}
Value_Type CRenjuSolver::Node::eval2(int who){//1:Black 2:White
	//return 0;
	Value_Type v=0;
	char temp[20];
	int lentemp=0;
	//	cout<<"find |"<<endl;
	for(int col=0;col<pstate->getNumCols();col++){
		lentemp=0;
		for(int row=0;row<pstate->getNumRows();row++){
			temp[lentemp++]=changeTo(row,col,who);
		}
		temp[lentemp++]=0;
		v+=evalStr(temp,who==pstate->turnToWho());
	}
	//	cout<<"find -"<<endl;
	for(int row=0;row<pstate->getNumRows();row++){
		lentemp=0;
		for(int col=0;col<pstate->getNumCols();col++){
			temp[lentemp++]=changeTo(row,col,who);

		}
		temp[lentemp++]=0;
		v+=evalStr(temp,who==pstate->turnToWho());
	}
	//	cout<<"find \\"<<endl;
	for(int col=-10;col<=10;col++){
		lentemp=0;
		for(int k=0;k<(15-abs(col));k++){
			if(col>=0){
				temp[lentemp++]=changeTo(k,col+k,who);
			}else{
				temp[lentemp++]=changeTo(k+abs(col),k,who);
			}
		}
		temp[lentemp++]=0;
		v+=evalStr(temp,who==pstate->turnToWho());
	}
	//	cout<<"find /"<<endl;
	for(int col=-10;col<=10;col++){
		lentemp=0;
		for(int k=0;k<(15-abs(col));k++){
			if(col>=0){
				temp[lentemp++]=changeTo(k,(14-col)-k,who);
			}else{
				temp[lentemp++]=changeTo(k+abs(col),14-k,who);
			}
		}
		temp[lentemp++]=0;
		v+=evalStr(temp,who==pstate->turnToWho());
	}
	return v;
}
void CRenjuSolver::Node::createSuccessor(){
	if(successors.size()==0){
		makeSuccessorNode(7,7);
		int maxA=min(7,pstate->getMaxOutLine()+pargs->MaxAddedSearchOutLine);
		for(int a=1;a<=maxA;a++){
			for(int i=7-a;i<=7+a;i++){
				makeSuccessorNode(7-a,i);
				makeSuccessorNode(7+a,i);
			}
			for(int i=7-a+1;i<=7+a-1;i++){
				makeSuccessorNode(i,7-a);
				makeSuccessorNode(i,7+a);
			}
		}
		if(pstate->trunToBlack()){
			sort(successors.begin(),successors.end(),biggercomp);
		}else{
			sort(successors.begin(),successors.end(),lesscomp);
		}
		if(!successors.empty()){
			if(abs(successors.front()->value)>INFINITE){
				while(abs(successors.back()->value)<INFINITE){
					delete successors.back();
					successors.pop_back();
				}
			}
		}
	}
}
char CRenjuSolver::Node::changeTo(int row,int col,int who){
	if(!pstate->isInRange(row,col)){
		cout<<"error"<<endl;
		return '0';
	}
	if(pstate->get(row,col)==0){
		return '0';
	}else if(pstate->get(row,col)==who){
		return '1';
	}else{
		return '2';
	}
}
void CRenjuSolver::Node::delSuccessors(){
	for(vector<Node *>::iterator i=successors.begin();i!=successors.end();i++){
		delete (*i);
	}
	successors.clear();
}

void CRenjuSolver::Node::makeSuccessorNode(int i,int j){
	if(pstate->isBlank(i,j)){
		Node *node=new Node();
		node->action.row=i;
		node->action.col=j;
		node->setState(this->getState());
		node->pargs=this->pargs;
		node->in();
		node->eval();
		node->out();
		successors.push_back(node);
	}
}
void CRenjuSolver::Node::print(){cout<<"Action:";action.print();cout<<"\t";cout<<"value:"<<value<<endl;}



inline void CRenjuSolver::thinking(CRenjuSolver::Node *node){if(thinkFunc!=NULL)thinkFunc(node);}
bool CRenjuSolver::cutoff_test(Node *node,int depth){
	if(node->value>=INFINITE || node->value<-INFINITE){
#ifdef SHOW_SEARCH_INFO
		cout<<"win or lose in cutoff_test()"<<endl;
		node->print();//TestRenjuViwer(node->getState()).print();
#endif
		return true;
	}
	if(depth>=args.MaxDepth){
		return true;
	}
	return false;
}
CRenjuSolver::Node CRenjuSolver::alpha_beta_search(Renju r){
	//cout<<"in alpha_beat_search()"<<endl;
	if(r.isFull())return glvars.LastNode;
	glvars.depth=0;
	Node *node=new Node();
	node->setPArgs(&this->args);
	node->setState(&r);node->value=0;
	glvars.LastNode.action.row=-1;
	Value_Type v;
	if(r.trunToBlack())v=max_value(node,-INFINITE,INFINITE);
	else v=min_value(node,-INFINITE,INFINITE);
	//
	if(glvars.LastNode.action.row==-1){//mean the node is cutoff.
		//measn someone wins,so create an arbitary action.
		node->createSuccessor();
		if(!node->successors.empty())glvars.LastNode=(*node->successors[0]);
		node->delSuccessors();
	}
	glvars.LastNode.value=v;
	delete node;
	return glvars.LastNode;
}
void CRenjuSolver::printTab(int t){
	for(int i=0;i<t;i++){
		cout<<"      ";
	}
}
Value_Type CRenjuSolver::max_value(Node* node,Value_Type a,Value_Type b){
	thinking(node);
	//cout<<"max_value:: in depth("<<depth<<")"<<endl;
	if(cutoff_test(node,glvars.depth)){
#ifdef SHOW_SEARCH_INFO
		printTab(glvars.depth+1);
#endif
		return node->value;
	}
	glvars.depth++; 
	Value_Type v=-INFINITE;
#ifdef SHOW_SEARCH_INFO
	printTab(glvars.depth);cout<<"{"<<endl;
#endif
	node->createSuccessor();
	Node *s;
	for(int i=0;i<min(args.MaxNumBrunch,(int)node->successors.size());i++){
		s=node->successors[i];
		s->in();
		double mv=min_value(s,a,b);
		if(mv>v){
			v=mv;
			if(glvars.depth<=1)glvars.LastNode=*s;
		}
#ifdef SHOW_SEARCH_INFO
		cout<<"Success["<<i<<"]:";node->successors[i]->print(); //RenjuViwer(node->getState()).print();
#endif
		s->out();
		if(v>=b){break;}
		a=max(a,v);
	}
	node->delSuccessors();
#ifdef SHOW_SEARCH_INFO
	printTab(glvars.depth);cout<<"}:";Trace(v);
#endif
	glvars.depth--;
	return v;
}
Value_Type CRenjuSolver::min_value(Node* node,Value_Type a,Value_Type b){
	thinking(node);
	//cout<<"min_value:: in depth("<<depth<<")"<<endl;
	if(cutoff_test(node,glvars.depth)){
#ifdef SHOW_SEARCH_INFO
		printTab(glvars.depth+1);
#endif
		return node->value;
	}
	glvars.depth++;
	Value_Type v=INFINITE;
#ifdef SHOW_SEARCH_INFO
	printTab(glvars.depth);cout<<"{"<<endl;
#endif
	node->createSuccessor();
	Node *s;
	for(int i=0;i<min(args.MaxNumBrunch,(int)node->successors.size());i++){
		s=node->successors[i];
		s->in();
		double mv=max_value(s,a,b);
		if(mv<v){
			v=mv;
			if(glvars.depth<=1)glvars.LastNode=*s;
		}
#ifdef SHOW_SEARCH_INFO
		cout<<"Success["<<i<<"]:";node->successors[i]->print();
#endif
		s->out();
		if(v<=a)break;
		b=min(b,v);
	}
	node->delSuccessors();
#ifdef SHOW_SEARCH_INFO
	printTab(glvars.depth);cout<<"}:";Trace(v);
#endif
	glvars.depth--;
	return v;
}
//


inline double Gene::myrand(double r){
	double t=rand()%32768;
	t/=(double)32768;
	t*=r;
	return t;
}


void Gene::init(double tome[],double toother[]){
	for(int i=0;i<23;i++){
		toMeWeight[i]=tome[i];
	}
	for(int i=0;i<23;i++){
		toOtherWeight[i]=toother[i];
	}
}
bool Gene::save(const char *fileName){
	ofstream outfile;
	outfile.open(fileName);
	if(outfile.fail()){
		cout<<"Can't open file: "<<fileName<<endl;
		return false;
	}
	outfile.setf(std::ios_base::fixed);
	for(int i=0;i<23;i++){
		outfile<<toMeWeight[i]<<endl;
	}
	outfile<<endl;
	outfile<<endl;
	for(int i=0;i<23;i++){
		outfile<<toOtherWeight[i]<<endl;
	}
	outfile.close();
	return true;
}
bool Gene::read(const char *fileName){
	ifstream infile;
	infile.open(fileName);
	if(infile.fail()){
		cout<<"Can't open file: "<<fileName<<endl;
		return false;
	}
	for(int i=0;i<23;i++){
		infile>>toMeWeight[i];
	}
	for(int i=0;i<23;i++){
		infile>>toOtherWeight[i];
	}
	infile.close();
	return true;
}
void Gene::print(){
	cout<<this->getName()<<":"<<endl;
	cout<<"To me:"<<endl;
	for(int i=0;i<23;i++){
		cout<<toMeWeight[i]<<',';
	}
	cout<<endl;
	cout<<"To other:"<<endl;
	for(int i=0;i<23;i++){
		cout<<toOtherWeight[i]<<',';
	}
	cout<<endl;
}
void Gene::printWithFeature(){
	cout<<this->getName()<<":"<<endl;
	cout<<"To me:"<<endl;
	print(toMeWeight);
	cout<<"To other:"<<endl;
	print(toOtherWeight);
}

void Gene::createFromParent(Gene *ga,Gene*gb){
	for(int i=0;i<23;i++){
		toMeWeight[i]=createFromParent(ga->toMeWeight[i],gb->toMeWeight[i]);
		toOtherWeight[i]=createFromParent(ga->toOtherWeight[i],gb->toOtherWeight[i]);
		if(rand()%10==1){
			toMeWeight[i]*=myrand(2);
			cout<<"toMeWeight big change here i="<<i<<" value:"<<toMeWeight[i]<<endl;
		}
		if(rand()%10==2){
			toOtherWeight[i]*=myrand(2);
			cout<<"toOtherWeight big change here i="<<i<<" value:"<<toOtherWeight[i]<<endl;
		}
	}
}

void Gene::print(double t[]){
	const char *features[]={
		"111110",//win 5
		"111112",//win 5

		"011110",//living 4
		"211110",//to 4
		"211101",//to 4
		"211011",//to 4
		"210111",//to 4

		"011100",//living 3
		"011010",//living jump 3

		"010101",//
		"210101",//
		"211100",//sleep 3
		"211010",//sleep jump 3
		"211001",//sleep jump 3
		"210110",//sleep jump 3
		"210011",//sleep jump 3

		"001100",//living 2
		"011000",//living 2
		"010100",//living jump 2
		"010010",//living big jump 2
		"211000",//sleep 2
		"210100",//sleep jump 2
		"210010"//sleep big jump 2
	};
	for(int i=0;i<23;i++){
		cout<<features[i]<<"="<<t[i]<<endl;
	}
}

/*//sleep big jump 2*/

void RenjuSolverCreator::createMap(CRenjuSolver::RenjuPatternDetector::MapType *m,double weights[23]){
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

CRenjuSolver* RenjuSolverCreator::createSolver(double DefenceToAttack,int MaxAddedSearchOutLine,int MaxDepth,int MaxNumBrunch,double toMeWegihts[23],double toOtherWeight[23]){
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
	solvers.push_back(psolver);
	return psolver;
}
CRenjuSolver*RenjuSolverCreator:: createSolverFromGene(Gene *pg){
	CRenjuSolver *psolver=new CRenjuSolver();
	string solverName="";
	solverName+="RenjuSolver(use "+pg->getName()+")";
	psolver->setName(solverName);

	CRenjuSolver::RenjuPatternDetector::MapType turnToMeMap,turnToOtherMap;
	createMap(&turnToMeMap,pg->getToMeWeight());
	createMap(&turnToOtherMap,pg->getToOtherWeight());
	arg.detector.setPatternMap(&turnToMeMap,&turnToOtherMap);

	psolver->setArgs(arg);
	solvers.push_back(psolver);
	return psolver;
}
void RenjuSolverCreator::setGlobalArgs(double DefenceToAttack,int MaxAddedSearchOutLine,int MaxDepth,int MaxNumBrunch){
	arg.DefenceToAttack=DefenceToAttack;
	arg.MaxAddedSearchOutLine=MaxAddedSearchOutLine;
	arg.MaxDepth=MaxDepth;
	arg.MaxNumBrunch=MaxNumBrunch;
}
//1.5,2,4,4

void RenjuSolverCreator::delSolver(CRenjuSolver *s){
	vector<CRenjuSolver *>::iterator i=find(solvers.begin(),solvers.end(),s);
	if(i!=solvers.end()){
		delete s;
		solvers.erase(i);
	}
}
void RenjuSolverCreator::delSolvers(){
	vector<CRenjuSolver *>::iterator i;
	for(i=solvers.begin();i!=solvers.end();i++){
		delete (*i);
	}
	solvers.clear();
}
