#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdlib>
using namespace std;

class token {
public:
	char clas;     //0-k 1-p 2-i 3-c
	int  id;
};

class identifier {
public:
	int id;
	string name;
};

class constnumber {
public:
	int id;
	float num;
};

extern vector<token> tokenarr;
extern vector<identifier> idarr;
extern vector<constnumber> ctarr;


void shengcheng(int clas, int id);
void showtkarr(ofstream& outfile);
void showidarr(ofstream& outfile);
void showctarr(ofstream& outfile);
int next_state(int pri_state, char ch);
int is_keyword(string str);
int is_pword(string str);
void show();
int Scanner(string w);