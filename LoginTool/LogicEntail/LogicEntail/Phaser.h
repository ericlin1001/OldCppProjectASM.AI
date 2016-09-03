#pragma once
#include<cstring>
#include<iostream>
using namespace std;
#define ENDOFSTR -1
class Phaser{
	enum TOKEN_TYPE{CONNECTIVE,QUANTIFIER,INTEGER,FLOAT,IDENTITY,SYMBOL,OTHER,ENDOFFILE};
	struct Token{
		int type;
		union{
			struct {int i;};
			struct {double d;};
			struct {char str[20];};
			struct {char ch;};
		};
		void print(){
			switch(type){
			case INTEGER:
				cout<<"INTEGER("<<i<<")";
				break;
				case CONNECTIVE:
				cout<<"CONNECTIVE("<<i<<")";
				break;
				case QUANTIFIER:
				cout<<"QUANTIFIER("<<i<<")";
				break;
			case FLOAT:
				cout<<"FLOAT("<<d<<")";
				break;
			case IDENTITY:
				cout<<"IDENTITY("<<str<<")";
				break;
			case SYMBOL:
				cout<<"SYMBOL("<<ch<<")";
				break;
			case OTHER:
				cout<<"OTHER("<<"OTHER"<<")";
				break;
			}
			cout<<endl;
		}
	};

public:
	Phaser();
	void init(char *str);
	bool getToken(char *str);
	bool getToken(char ch);
	Token* getTokenValue(){return &token;}
	bool getToken(int &k);
	bool getToken(double &k);
	bool getToken(float &k);
	bool isEnd(){return p>=len;}
	Token* getToken();

private:
	static bool isDigital(char ch){return '0'<=ch && ch<='9';}
	static bool isUpperLetter(char ch){return 'A'<=ch && ch<='Z';}
	static bool isLowerLetter(char ch){return 'a'<=ch && ch<='z';}
	static bool isLetter(char ch){return isUpperLetter(ch)||isLowerLetter(ch);}
	static bool isSymbol(char ch){return ch=='('||ch==')'||ch=='['||ch==']'||ch=='.'||ch==','||ch=='~'||ch=='|'||ch=='&'||ch=='>'||ch=='-'||ch=='?'||ch==';'||ch==':';}
	static bool isSpace(char ch){return ch==' '|| ch=='\n' || ch=='\t';}
	//
	char nextChar();
	char ch;
	char *str;
	int p;
	int len;
	Token token;
};