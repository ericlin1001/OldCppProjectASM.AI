// LogicEntail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Phaser.h"
static bool isDigital(char ch){return '0'<=ch && ch<='9';}
static bool isUpperLetter(char ch){return 'A'<=ch && ch<='Z';}
static bool isLowerLetter(char ch){return 'a'<=ch && ch<='z';}
static bool isLetter(char ch){return isUpperLetter(ch)||isLowerLetter(ch);}
static bool isSymbol(char ch){return ch=='('||ch==')'||ch=='['||ch==']'||ch=='.'||ch==','||ch=='~'||ch=='|'||ch=='&'||ch=='>'||ch=='-'||ch=='?'||ch==';'||ch==':';}
static bool isSpace(char ch){return ch==' '|| ch=='\n' || ch=='\t';}
/*

~P(f(ad),ad(23))V(adf(ad),31)->q;
p(x)<-r(y);
p->q;
q->r;

*/
#include<vector>
using namespace std;
class Term{
	enum Type{FUNCTION,CONSTANT,VARIABLE};
	int type;
public:
	string name;
	string value;
	vector<Term>args;
	Term(){
	}
	bool read(char *&str){
		if(!isLetter(*str))return false;
		name.clear();
		args.clear();
		while(isLetter(*str)||isDigital(*str)&&*str!='('){
			name.push_back(*str++);
		}
		if(*str=='('){
			type=Type::FUNCTION;
			Term t;
			do{
				if(!t.read(str)){return false;break;}
				args.push_back(t);
				if(*str==',')str++;
			}while(*str!=')');
			if(*str==')')str++;
		}else{
			if(isUpperLetter(name.at(0))){
				type=Type::CONSTANT;
			}else{
				type=Type::VARIABLE;
			}
		}
		return true;
	}
	bool isConstant(){return type==Type::CONSTANT;}
	bool isFunction(){return type==Type::FUNCTION;}
	bool isVaribale(){return type==Type::VARIABLE;}
};
class AtomicSentence{
private:
	bool isEquation;
	string name;
	bool value;
	vector<Term>args;
public:
	AtomicSentence(){
	}
	bool read(char *&str){
		if(!isLetter(*str))return false;
		
		name.clear();
		args.clear();

		isEquation=false;
		char *p;
		p=str;
		while(*p && *p!=';'){if(*p=='=')isEquation=true;p++;}
		Term t;
		if(isEquation){
			name.assign("Equal");
			t.read(str);
			args.push_back(t);
			if(*str=='=')str++;
			t.read(str);
			args.push_back(t);
		}else{
			
			while(isLetter(*str)||isDigital(*str)&&*str!='('){
				name.push_back(*str++);
			}
			if(*str=='('){
				do{
					if(!t.read(str)){return false;break;}
					args.push_back(t);
					if(*str==',')str++;
				}while(*str!=')');
				if(*str==')')str++;
			}
		}

	}
};
class Sentence{
public:
	Sentence(){
	}
	bool read(char *&str){

	}
};
int _tmain(int argc, _TCHAR* argv[])
{char s[]="\xa1\xc1\xa1\xc4\xa1\xc5\xa5\xe8\xa1\xd4\xa1\xfa\xa1\xfb";
for(int i=0;i<strlen(s);i++)cout<<s[i];
Phaser c;
char str[100];
while(1){
	cin>>str;
	c.init(str);
	c.getToken();
	while(!c.isEnd()){
		c.getToken('~')
	}
	//¡Á  ¡Ä  ¡Å  ¥è  ¡Ô  ¡ú  ¡û
	while(!c.isEnd()){
		//	for(int i=0;i<strlen(str);i++)cout<<str[i]<<"="<<hex<<(int)str[i]<<endl;
		c.getToken();c.getTokenValue()->print();
	}
}
return 0;
}


/*
class Sentence{
static bool isDigital(char ch){return '0'<=ch && ch<='9';}
static bool isUpperLetter(char ch){return 'A'<=ch && ch<='Z';}
static bool isLowerLetter(char ch){return 'a'<=ch && ch<='z';}
static bool isLetter(char ch){return isUpperLetter(ch)||isLowerLetter(ch);}
static bool isSymbol(char ch){return ch=='('||ch==')'||ch=='['||ch==']'||ch=='.'||ch==','||ch=='/'||ch=='?'||ch==';';}
static bool isSpace(char ch){return ch==' '|| ch=='\n' || ch=='\t';}
private:
char *str;
char *senstr;
bool isSentenceEnd(){return *str==';';}
bool isSpace(){return *str==' '||*str=='\t';}//to obmit
bool isPrefixSentence(){return *str=='~';}
bool isLetter(){return isLetter(*str);}
bool readPrefix(){

}
bool readAtomicSentence(){

}
public:
Sentence(){senstr=NULL;}
Sentence(Sentence &p){
//
}
~Sentence(){if(senstr!=NULL)delete senstr;}
int init(char *p){
str=p;
while(isSpace()&&!isSentenceEnd())str++;
//
if(senstr!=NULL)delete senstr;
senstr=new char[strlen(str)+1];
strcpy(senstr,str);
//
str=senstr;
prefix.clear();
if(isPrefix()){
prefix.push_back(*str);
nextChar();
}

}
void nextChar(){str++;}
bool isAtomic(){
return prefix.size()==0;
}
//
string prefix;
//
string origin;
};*/