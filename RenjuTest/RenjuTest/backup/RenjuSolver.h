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
#define INFINITE 10000000

typedef double Value_Type ;



class CRenjuSolver{
	//struct declare

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
	private:
		MapType patternMap[2];//0 mean the turn to me,1 mean turnTo other.
		MapType bufferPatternMap[2];
		//
		MapType *pPatternMap;
		MapType *pBufferPatternMap;
		
	public:
		static MapType default_patternMap[2];
		RenjuPatternDetector();
		void setPatternMap(MapType *turnToMeMap,MapType *turnToOtherMap);
		double evalStr(const char *pattern,bool turnToMe);
	private:
		void copyWithReverse(MapType *dest,MapType *src);
		double getStrWeight(char *bits);
		bool cut_off_zero(char *&str);
		double getUnKnowStrWegiht(const char *bits);
	};

public:
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
	public:
	struct Node{
	private:
		Arg *pargs;
	public:
		Renju::Action action;
		Renju *pstate;
		Value_Type value;
		vector<Node *>successors;
		static void init();
		inline void setPArgs(Arg *pargs){this->pargs=pargs;}
	private:
		Value_Type eval();
	public:
		
		static inline bool biggercomp(Node *a,Node *b){return a->value > b->value;}
		static inline bool lesscomp(Node *a,Node *b){return a->value < b->value;	}
		inline Value_Type evalStr(char *pattern,bool turnToMe=true);
		inline bool isBlackWin(){return eval(1)>INFINITE;}
		inline bool isWhiteWin(){return eval(2)>INFINITE;}
		inline Value_Type eval(int who){return eval2(who);}
		inline void in(){pstate->execute(&action);}
		inline void out(){pstate->unexecute(&action);}
		inline void setState(Renju *r){pstate=r;}
		inline Renju* getState(){return pstate;}

		char changeTo(int row,int col,int who);
		Value_Type eval2(int who);
		void createSuccessor();
		void makeSuccessorNode(int i,int j);
		Node(){pstate=NULL;}
		~Node(){delSuccessors();}
		void delSuccessors();
		void print();
	};
private:
	struct GLVar{
		int depth;
		Node LastNode;
	};
	//variables:
	public:
		typedef void (*ThinkingFunc)(Node *node);
private:
	ThinkingFunc thinkFunc;
	Arg args;
	GLVar glvars;
public:
	inline void setThinkFunc(ThinkingFunc t){
		thinkFunc=t;
	}
	CRenjuSolver(){thinkFunc=NULL;}
	inline void setArgs(Arg args){this->args=args;}
	inline const Arg *getArgs(){return &args;}
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
#endif