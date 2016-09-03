#include "stdafx.h"
#include "Phaser.h"
#include<cstring>
#include<iostream>
using namespace std;

#define _SHOW_PHASER_DEBUG_OUTPUT
Phaser::Phaser(){
}
void Phaser::init(char *str){
	this->str=str;
	p=-1;
	len=strlen(str);
	nextChar();
}
bool Phaser::getToken(char *str){
	//make the str lower...
	if(strcmp(token.str,str)==0){
		getToken();
		return true;
	}else return false;
}
bool Phaser::getToken(char ch){
	if(token.ch==ch){
		getToken();
		return true;
	}else return false;
}
bool  Phaser::getToken(int &k){
	if(token.type==TOKEN_TYPE::INTEGER){
		k=token.i;
		getToken();
		return true;
	}else return false;
}
#define Trace(m) cout<<#m"="<<(m)<<endl;
bool  Phaser::getToken(double &k){
	if(token.type==TOKEN_TYPE::FLOAT){
		k=token.d;
		getToken();
		return true;
	}else if(token.type==TOKEN_TYPE::INTEGER){
#ifdef SHOW_PHASER_DEBUG_OUTPUT
		cout<<"Waring:uppercae() integer->doulbe"<<endl;
#endif
		k=token.i;
		getToken();
		return true;
	}else return false;
}
bool  Phaser::getToken(float &k){
	if(token.type==TOKEN_TYPE::FLOAT){
		k=token.d;
		getToken();
		return true;
	}else if(token.type==TOKEN_TYPE::INTEGER){
#ifdef SHOW_PHASER_DEBUG_OUTPUT
		cout<<"Waring:uppercae() integer->float"<<endl;
#endif
		k=token.i;
		getToken();
		return true;
	}else return false;
}
Phaser::Token*  Phaser::getToken(){
	if(this->isEnd()){
		token.type==ENDOFFILE;
		return &token;
	}
	while(isSpace(ch) && !isEnd())nextChar();
	char temp[20];int templen=0;
	temp[templen++]=ch;
	if(isDigital(ch)||(ch=='-' && isDigital(str[p+1]))){
		while(isDigital(nextChar()) && !isEnd())temp[templen++]=ch;
		if(ch=='.'){
			temp[templen++]='.';
			while(isDigital(nextChar()) && !isEnd())temp[templen++]=ch;
			temp[templen++]=0;
			token.d=atof(temp);
			token.type=TOKEN_TYPE::FLOAT;
		}else{
			temp[templen++]=0;
			token.i=atoi(temp);
			token.type=TOKEN_TYPE::INTEGER;
		}
	}else if(isLetter(ch)){
		token.type=TOKEN_TYPE::IDENTITY;
		while(isLetter(nextChar()) && !isEnd())temp[templen++]=ch;
		temp[templen++]=0;
		strcpy(token.str,temp);
	}else if(isSymbol(ch)){
		token.type=TOKEN_TYPE::SYMBOL;
		token.ch=ch;
		nextChar();
	}else  if(ch=='-' && str[p+1]=='>'){
		token.type=TOKEN_TYPE::CONNECTIVE;
		strcpy(token.str,"->");
		nextChar();nextChar();
	}else  if(ch=='<' && str[p+1]=='-'){
		token.type=TOKEN_TYPE::CONNECTIVE;
		strcpy(token.str,"<-");
		nextChar();nextChar();
	}else if(ch==0){
#ifdef SHOW_PHASER_DEBUG_OUTPUT
		cout<<"End of file."<<endl;
#endif
	}else{
		token.type=TOKEN_TYPE::OTHER;
#ifdef SHOW_PHASER_DEBUG_OUTPUT
		cout<<"Error:Undefined char("<<ch<<":"<<(int)ch<<")"<<endl;
#endif
		nextChar();
	}
	return &this->token;
}

char  Phaser::nextChar(){
	if(isEnd())return ENDOFSTR;
	p++;
	ch=str[p];
	return ch;
}
