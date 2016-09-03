#include "stdafx.h"
#ifndef RENJUSOLVER_H 
#define RENJUSOLVER_H
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
using namespace std;
#define INFINITE 1000000

typedef double Value_Type ;



class CRenjuSolver{
public:
	
	class RenjuPatternDetector{
	private:
		struct MyStringComp
		{
			enum{	// parameters for hash table
				bucket_size = 1		// 0 < bucket_size
			};
			size_t operator()(const string &s) const
			{
				const char *p=(const char*)s.c_str();
				p+=s.size()-1;
				size_t hashval=0;
				for(size_t n=s.size();n>0;n--){
					hashval*=3;
					hashval+=(*p-'0');
					p--;
				}
				return hashval;
			}
			bool operator()(const string& str1, const string&str2) const
			{	// test if _Keyval1 ordered before _Keyval2
				return strcmp(str1.c_str(),str2.c_str())<0;
			}
		};
	public:
		typedef hash_map<string,double,MyStringComp> MapType;
		static MapType default_patternMap[2];
	private:
		MapType patternMap[2];//0 mean the turn to me,1 mean turnTo other.
		MapType bufferPatternMap[2];
		MapType *pPatternMap;//point to which to use.
		MapType *pBufferPatternMap;//the same as upper one.
	public:
		RenjuPatternDetector();
		void setPatternMap(MapType *turnToMeMap,MapType *turnToOtherMap);
		double evalStr(const char *pattern,bool turnToMe);
	private:
		void copyWithReverse(MapType *dest,MapType *src);
		double getStrWeight(char *bits);
		bool cut_off_zero(char *&str);
		double getUnKnowStrWegiht(const char *bits);
	};
	struct Arg{
		int MaxAddedSearchOutLine;//=2;
		double DefenceToAttack;//=1.1;
		int MaxDepth;//=1;
		int MaxNumBrunch;//=20;
		RenjuPatternDetector detector;
		Arg(){
			MaxAddedSearchOutLine=2;
			DefenceToAttack=1.5;
			MaxDepth=4;
			MaxNumBrunch=5;
		}
	};
	struct Node{
	private:
		Arg *pargs;
	public:
		Renju::Action action;
		Renju *pstate;
		Value_Type value;
		vector<Node *>successors;
		//method:
	private:
		Value_Type eval();
	public:
		static void init();
		static inline bool biggercomp(Node *a,Node *b){return a->value > b->value;}
		static inline bool lesscomp(Node *a,Node *b){return a->value < b->value;	}
		//
		inline void setPArgs(Arg *pargs){this->pargs=pargs;}
		inline Value_Type evalStr(char *pattern,bool turnToMe=true);
		inline bool isBlackWin(){return eval(1)>INFINITE;}
		inline bool isWhiteWin(){return eval(2)>INFINITE;}
		inline Value_Type eval(int who){return eval2(who);}
		inline void in(){pstate->execute(&action);}
		inline void out(){pstate->unexecute(&action);}
		inline void setState(Renju *r){pstate=r;}
		inline Renju* getState(){return pstate;}
		//
		Node(){pstate=NULL;}
		~Node(){delSuccessors();}
		char changeTo(int row,int col,int who);
		Value_Type eval2(int who);
		void createSuccessor();
		void makeSuccessorNode(int i,int j);
		void delSuccessors();
		void print();
	};
	typedef void (*ThinkingFunc)(Node *node);
private:
	struct GLVar{
		int depth;
		Node LastNode;
	};	
	string name;
	ThinkingFunc thinkFunc;
	Arg args;
	GLVar glvars;
	//method:
public:
	inline void setThinkFunc(ThinkingFunc t){
		thinkFunc=t;
	}
	CRenjuSolver(){thinkFunc=NULL;name="RenjuSolver";}
	inline void setName(string s){name=s;}
	inline const string getName(){return name;}
	inline void setArgs(Arg args){this->args=args;}
	inline  Arg *getArgs(){return &args;}
	Renju::Action getAction(Renju r){
		alpha_beta_search(r);
		return glvars.LastNode.action;
	}
	Node *getPNode(Renju r){
		alpha_beta_search(r);
		return &glvars.LastNode;
	}
	static int getWinState(Renju r){
		//1:balck 2:white
		int w=0;
		Node node;
		Arg arg;
		RenjuPatternDetector::MapType map;
		map["111110"]=INFINITE*2;
		map["111112"]=INFINITE*2;
		arg.detector.setPatternMap(&map,&map);
		node.setPArgs(&arg);
		node.setState(&r);
		if(node.isBlackWin())w+=1;
		if(node.isWhiteWin())w+=2;
		return w;
	}
private:
	bool cutoff_test(Node *node,int depth);
	Node alpha_beta_search(Renju r);
	Value_Type max_value(Node* node,Value_Type a,Value_Type b);
	Value_Type min_value(Node* node,Value_Type a,Value_Type b);
	inline void thinking(Node *node);
	static void printTab(int t);
};

//

class Gene{
private:
	double toMeWeight[23],toOtherWeight[23];
	string name;
	//method:
private:
	void print(double t[]);
	inline double createFromParent(double a,double b){return (a*(0.8+myrand(0.4))+b*(0.8+myrand(0.4)))/2;}
public:
	static inline double myrand(double r);
	Gene(){name="Gene";}
	inline void setName(const char *s){name.assign(s);}
	inline const string getName(){return name;}
	inline double*getToMeWeight(){return toMeWeight;}
	inline double *getToOtherWeight(){return toOtherWeight;}
	void init(double tome[],double toother[]);
	bool save(const char *fileName);
	bool read(const char *fileName);
	void print();
	void printWithFeature();
	void createFromParent(Gene *ga,Gene*gb);	
};

class RenjuSolverCreator{
private:
	vector<CRenjuSolver *>solvers;
	CRenjuSolver::Arg arg;
	void createMap(CRenjuSolver::RenjuPatternDetector::MapType *m,double weights[23]);
public:
	CRenjuSolver* createSolver(double DefenceToAttack,int MaxAddedSearchOutLine,int MaxDepth,int MaxNumBrunch,double toMeWegihts[23],double toOtherWeight[23]);
	CRenjuSolver* createSolverFromGene(Gene *pg);
	void setGlobalArgs(double DefenceToAttack,int MaxAddedSearchOutLine,int MaxDepth,int MaxNumBrunch);
	~RenjuSolverCreator(){delSolvers();}
	void delSolver(CRenjuSolver *s);
	void delSolvers();
};
#endif