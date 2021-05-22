#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include "cifafenxiqi.h"
using namespace std;

class VN {
public:
	char vn;
	vector<char> first;
	vector<char> follow;
	int empty=-1;
	bool setable = true;
};
class VT {
public:
	char vt;
	vector<char> first;
	vector<char> follow;
};
class grammer_ {
public:
	string grammer;
	bool empty;
	vector<char> select;
};

//LL（1）分析表
class analyse_table {
public:
	int vt_num;
	int vn_num;
	int** table;
	void analyse_table_init(int vtnum, int vnnum);
	void show_table()const {
		for (int i = 0; i < vn_num; i++) {
			for (int j = 0; j < vt_num+1; j++) {
				cout << table[i][j]<<" ";
			}
			cout << endl;
		}
	};
};		

//语法分析
class parser {
public:
	//读入的原始信息 
	char empty;                     //表示空的字符串的字符
	vector<char> Vt;				
	vector<char> Vn;				
	int vt_num;
	int vn_num;
	vector<string> gramer_vec;

	//初始化语法分析器  读入文法 字符集合
	void init(string gramfilename,string Vfilename);
	void get_gramer(string filename);
	void getV(string filename);
	
	void show_gramer()const {
		int i = 0;
		for (auto iter = gramer_vec.begin(); iter != gramer_vec.end(); iter++) {
			cout << *iter << endl;
		}
		for (auto iter = Vn.begin(); iter != Vn.end(); iter++) {
			cout << *iter << endl;
			i++;
		}
		cout << empty;
	};
	void show_first()const {
		for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
			cout << iter->vn << ":{";
			for (auto i = iter->first.begin(); i != iter->first.end(); i++) {
				cout << *i;
			}
			cout << "}" << endl;
		}
	};
	void show_follow()const {
		for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
			cout << iter->vn << ":{";
			for (auto i = iter->follow.begin(); i != iter->follow.end(); i++) {
				cout << *i;
			}
			cout << "}" << endl;
		}
	};
	void show_empty()const {
		for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
			cout << iter->vn << ":{"<< iter->empty<<"}" << endl;
		}
	}
	void show_sen_empty()const {
		for (auto iter =grammerinfo.begin(); iter != grammerinfo.end(); iter++) {
			cout << iter->grammer << ":{" << iter->empty << "}" << endl;
		}
	}
	void show_select()const {
		for (auto iter = grammerinfo.begin(); iter != grammerinfo.end(); iter++) {
			cout << iter->grammer<<":{";
			for (auto s = iter->select.begin(); s !=iter->select.end(); s++) {
				cout << *s;
			}
			cout << "}" << endl;
		}
	};

	bool is_vn(char t);
	bool is_vt(char t);

	int get_vn_pos(char vn);
	int get_vt_pos(char vt);
	int get_gramer_pos(string gramerstr);

	VN& get_vn(char vn);
	VT& get_vt(char vt);
	grammer_& get_grammer(string gramerstr);

	//求first follow select 集合 
	void canempty();
	void is_empty(VN& det);
	void set_first();
	void set_follow();
	char get_first(string w, VN& det);
	char get_first(string w, grammer_& det);
	void get_follow(VN& det);
	void get_select();
	bool right_empty(string w);
	void qiufirst(VN& wait);
	void cleanfirst() {
		for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
			auto first = iter->first;
			for (auto i = first.begin(); i != first.end(); i++) {
				for (auto j = i + 1; j != first.end(); j++) {
					if (*i == *j) {
						j = first.erase(j);
						j--;
					}
				}
			}
			iter->first = first;
		}
		for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
			for (auto i = iter->first.begin(); i != iter->first.end(); i++) {
				if (*i == empty) {
					i = iter->first.erase(i);
					i--;
				}
			}
		}
	};
	void cleanfollow() {
		for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
			auto follow = iter->follow;
			for (auto i = follow.begin(); i != follow.end(); i++) {
				for (auto j = i + 1; j != follow.end(); j++) {
					if (*i == *j) {
						j = follow.erase(j);
						j--;
					}
				}
			}
			iter->follow = follow;
		}
		for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
			for (auto i = iter->follow.begin(); i != iter->follow.end(); i++) {
				if (*i == empty) {
					i = iter->follow.erase(i);
					i--;
				}
			}
		}
	}
	//填分析表
	void fillin_table();
	//分析器
	bool analyser(string w);
	int serch_gram_pos(char vn, char vt) {
		int vn_pos = 0;
		int vt_pos = 0;
		for (auto p = vninfo.begin(); p != vninfo.end() && p->vn != vn; p++) {
			vn_pos++;
		}
		for (auto p = vtinfo.begin(); p != vtinfo.end() && p->vt != vt; p++) {
			vt_pos++;
		}
		return table.table[vn_pos][vt_pos];
	}

	//保存经过分析过后的各产生式和符号的信息
	vector<VN> vninfo;
	vector<VT> vtinfo;
	vector<grammer_> grammerinfo;
	analyse_table table;
};


void analyse_table::analyse_table_init(int vtnum, int vnnum) {
	vt_num = vtnum;
	vn_num = vnnum;
	table = new int* [vnnum];
	for (int i = 0; i < vnnum; i++) {
		table[i] = new int[vtnum + 1];
	}
	for (int i = 0; i < vn_num; i++) {
		for (int j = 0; j < vt_num+1; j++) {
			table[i][j] = -1;
		}
	}
};

int parser::get_vn_pos(char vn) {
	int i = 0;
	for (auto iter = Vn.begin(); iter != Vn.end(); iter++) {
		if (*iter == vn)
			return i;
		i++;
	}
}

bool parser::is_vn(char t) {
	for (auto iter = Vn.begin(); iter != Vn.end(); iter++) {
		if (*iter == t)
			return true;
	}
	return false;
}
bool parser::is_vt(char t) {
	for (auto iter = Vt.begin(); iter != Vt.end(); iter++) {
		if (*iter == t)
			return true;
	}
	return false;
}

int parser::get_vt_pos(char vt) {
	int i = 0;
	for (auto iter = Vt.begin(); iter != Vt.end(); iter++) {
		if (*iter == vt)
			return i;
		i++;
	}
	return i;
}
int parser::get_gramer_pos(string gramerstr) {
	int i = 0;
	for (auto iter = gramer_vec.begin(); iter != gramer_vec.end(); iter++) {
		if (*iter == gramerstr)
			return i;
		i++;
	}
}

VN& parser::get_vn(char vn) {
	for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
		if (iter->vn == vn)
			return *iter;
	}
}
VT& parser::get_vt(char vt) {
	for (auto iter = vtinfo.begin(); iter != vtinfo.end(); iter++) {
		if (iter->vt == vt)
			return *iter;
	}
}
grammer_& parser::get_grammer(string gramerstr) {
	for (auto iter = grammerinfo.begin(); iter != grammerinfo.end(); iter++) {
		if (iter->grammer == gramerstr)
			return *iter;
	}
}


void parser::init(string gramfilename,string Vfilename) {
	get_gramer(gramfilename);
	getV(Vfilename);
	canempty();
	set_first();
	cleanfirst();
	set_follow();
	cleanfollow();
	get_select();
	table.analyse_table_init(vt_num, vn_num);
	fillin_table();
}
void parser::get_gramer(string filename) {
	ifstream gramer;
	string now;
	gramer.open(filename);
	while (getline(gramer,now)) {
		char left = now[0];
		string right = now.substr(3, now.length() - 3);
		string youxiao = "";
		for (int i = 0; i < right.length(); i++) {
			char nowch = right[i];
			if (nowch != '|') {
				youxiao.append(1,nowch);
			}
			else {
				string gram="";
				gram.append(1,left);
				gram = gram + youxiao;
				youxiao = "";
				gramer_vec.push_back(gram);
			}
		}
		string gram = "";
		gram.append(1, left);
		gram = gram + youxiao;
		youxiao = "";
		gramer_vec.push_back(gram);
	}
	return;
}
void parser::getV(string filename) {
	ifstream Vfile;
	Vfile.open(filename);
	string now;
	getline(Vfile, now);
	vt_num = 0;
	vn_num = 0;
	for (int i = 3; i < now.length(); i++) {
		Vn.push_back(now[i]);
		VN p;
		p.vn = now[i];
		vninfo.push_back(p);
		vn_num++;
	}
	getline(Vfile, now);
	for (int i = 3; i < now.length(); i++) {
		Vt.push_back(now[i]);
		VT p;
		p.vt = now[i];
		vtinfo.push_back(p);
		vt_num++;
	}
	getline(Vfile, now);
	empty=now[now.length()-1];
}


void parser::set_first() {
	for (auto i = vtinfo.begin(); i != vtinfo.end(); i++) {
		(i->first).push_back(i->vt);
	}

	for (auto p = vninfo.begin(); p != vninfo.end(); p++) {
		qiufirst(*p);
	}
}
void parser::qiufirst(VN& wait) {
	for (auto iter_gr = gramer_vec.begin(); iter_gr != gramer_vec.end(); iter_gr++) {
		if ((*iter_gr)[0] == wait.vn) {
			string right = "";
			right = (*iter_gr).substr(1, (*iter_gr).length() - 1);
			wait.first.push_back(get_first(right, wait));
		}
	}
}
char parser::get_first(string w, VN& det) {
	for (int i = 0; i < w.length(); i++) {
		if (is_vt(w[i])) {
			cout << w[i];
			return w[i];
		}
		else if (is_vn(w[i])) {
			VN& left_vn = get_vn(w[i]);
			auto firstjihe_iter = left_vn.first;
			if (firstjihe_iter.empty()) {
				qiufirst(left_vn);
				firstjihe_iter = left_vn.first;
			}
			if (left_vn.empty) {
				det.first.insert(det.first.end(), firstjihe_iter.begin(),firstjihe_iter.end());
				break;
			}
			else {
				det.first.insert(det.first.end(), firstjihe_iter.begin(), firstjihe_iter.end());
				det.first.pop_back();
				return *(firstjihe_iter.end() - 1);
			}
		}
		else {
			return empty;
		}
	}
}
char parser::get_first(string w, grammer_& det) {
	for (int i = 0; i < w.length(); i++) {
		if (is_vt(w[i])) {
			return w[i];
		}
		else if (is_vn(w[i])) {
			VN& left_vn = get_vn(w[i]);
			auto firstjihe_iter = left_vn.first;
			if (left_vn.empty) {
				det.select.insert(det.select.end(), firstjihe_iter.begin(), firstjihe_iter.end());
				break;
			}
			else {
				det.select.insert(det.select.end(), firstjihe_iter.begin(), firstjihe_iter.end());
				det.select.pop_back();
				return *(firstjihe_iter.end() - 1);
			}
		}
		else {
			return empty;
		}
	}
}

void parser::set_follow() {
	for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
		if (iter->vn == (*gramer_vec.begin())[0])
			iter->follow.push_back('#');
		get_follow(*iter);
	}
}
void parser::get_follow(VN& det) {
	for (auto sen = gramer_vec.begin(); sen != gramer_vec.end(); sen++) {
		string now = sen->substr(1, sen->length() - 1);
		for (int i = 0; i < now.length(); i++) {
			if (now[i] == det.vn) {
				if (i + 1 != now.length()) {
					char next = now[i + 1];
					if (is_vt(next)) {
						det.follow.push_back(next);
					}
					else if (is_vn(next)) {
						string right = now.substr(i + 1, now.length() - i - 1);
						if (right_empty(right)) {
							VN& left_vn=get_vn((*sen)[0]);
							det.follow.insert(det.follow.end(), left_vn.follow.begin(), left_vn.follow.end());
						}
						for (int j = i + 1; j < now.length(); j++) {
							VN& now_vn = get_vn(now[j]);
							det.follow.insert(det.follow.end(), now_vn.first.begin(), now_vn.first.end());
							if (!now_vn.empty)	break;
						}
					}
				}
				if (i + 1 == now.length()) {
					VN& left = get_vn((*sen)[0]);
					det.follow.insert(det.follow.end(), left.follow.begin(), left.follow.end());
				}
			}
		}
	}
}

void parser::get_select() {
	for (auto iter = gramer_vec.begin(); iter != gramer_vec.end(); iter++) {						//第一遍扫描确定表达式右部是否能推空
		grammer_ now;  
		now.grammer = *iter;
		string right=iter->substr(1,iter->length()-1);
		now.empty = right_empty(right);
		grammerinfo.push_back(now);											
	}
	for (auto iter = grammerinfo.begin(); iter != grammerinfo.end(); iter++) {						//第二遍扫描求select集合
		string now = iter->grammer;
		string right = now.substr(1, now.length() - 1);
		if (iter->empty) {
			iter->select.push_back(get_first(right, *iter));
			VN& left = get_vn(now[0]);
			iter->select.insert(iter->select.end(), left.follow.begin(), left.follow.end());
		}
		else {
			iter->select.push_back(get_first(right, *iter));
		}
	}
	for (auto iter = grammerinfo.begin(); iter != grammerinfo.end(); iter++) {						//清洗空符号
		for (auto i = iter->select.begin(); i != iter->select.end(); i++) {
			if (*i == empty) {
				i = iter->select.erase(i);
			}
		}
	}
}

void parser::canempty() {
	for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
		for (auto sen = gramer_vec.begin(); sen != gramer_vec.end() ; sen++) {
			if ((*sen)[0] == iter->vn) {
				string right = sen->substr(1, sen->length() - 1);
				for (int i = 0; i < right.length(); i++) {
					if (right.length() == 1 && right[i] == empty &&iter->setable) {
						iter->empty = 1;
						break;
					}
					else if (is_vt(right[i]) && iter->setable) {
						iter->empty = 0;
						break;
					}
					else if (is_vn(right[i]) && iter->setable) {
						VN& ch = get_vn(right[i]);
						if (ch.empty == -1) {
							is_empty(ch);
						}
						else if (ch.empty == 0) {
							iter->empty = 0;
							break;
						}
						else if (ch.empty == 1 && i == right.length() - 1) {
							iter->empty = 1;
							iter->setable = false;
							break;
						}
					}
				}
			}
		}
	}
}
void parser::is_empty(VN& det) {
	for (auto sen = gramer_vec.begin(); sen != gramer_vec.end(); sen++) {
		if ((*sen)[0] == det.vn) {
			string right = sen->substr(1, sen->length() - 1);
			for (int i = 0; i < right.length(); i++) {
				if (right.length() == 1 && right[i] == empty && det.setable) {
					det.empty = 1;
					break;
				}
				else if (is_vt(right[i]) && det.setable) {
					det.empty = 0;
					break;
				}
				else if (is_vn(right[i]) && det.setable) {
					VN& ch = get_vn(right[i]);
					if (ch.empty == -1) {
						is_empty(ch);
					}
					if (ch.empty == 0) {
						det.empty = 0;
						break;
					}
					else if (ch.empty == 1 && i == right.length() - 1) {
						det.empty = 1;
						det.setable = false;
						break;
					}
				}
			}
		}
	}
}
bool parser::right_empty(string w) {
	for (int i = 0; i < w.length(); i++) {
		if (is_vt(w[i])) {
			return false;
		}
		else if (is_vn(w[i])) {
			VN& iter = get_vn(w[i]);
			if (iter.empty == 0) {
				return false;
			}
		}
		return true;
	}
}

void parser::fillin_table() {
	int gram_pos = 0;
	for (auto iter = grammerinfo.begin(); iter != grammerinfo.end(); iter++) {
		gram_pos++;
		vector<char> select = iter->select;
		int vn_pos = 0;
		int vt_pos = 0;
		for (auto p = select.begin(); p != select.end(); p++) {
			char left = (iter->grammer)[0];
			char t = *p;
			vn_pos = get_vn_pos(left);
			vt_pos = get_vt_pos(t);
			table.table[vn_pos][vt_pos] = gram_pos;
		}
	}
}

string zhuanhuan(vector<token> tok) {
	string a="";
	for (auto iter = tok.begin(); iter != tok.end(); iter++) {
		if (iter->clas == 'I' || iter->clas == 'C') {
			a.push_back('I');
		}
		else if (iter->clas == 'P')
		{
			if (iter->id == 1 || iter->id == 8){
				a.push_back('W');
			}
			if (iter->id == 2 || iter->id == 9) {
				a.push_back('Q');
			}
			if (iter->id == 3) {
				a.push_back('(');
			}
			if (iter->id == 4) {
				a.push_back(')');
			}
		}
	}
	return a;
}
bool parser::analyser(string w) {
	stack<char>  fenxizhan;
	Scanner(w);
	string A = zhuanhuan(tokenarr);
	A.push_back('#');
	fenxizhan.push('#');
	fenxizhan.push('E');
	for (int i = 0; i < A.length(); i++)
	{
		bool flag = false;
		while (!flag) {
			char stk_top = fenxizhan.top();
			fenxizhan.pop();
			cout << i << stk_top << A[i]<<endl;
			if (is_vt(stk_top)) {
				if (stk_top == A[i])
					flag = true;
				else
					return false;
			}
			else if (is_vn(stk_top)) {
				int gramer_pos = serch_gram_pos(stk_top, A[i]);
				if (gramer_pos == -1) {
					return false;
				}
				else {
					auto iter = grammerinfo.begin();
					for (int j = 0; j != gramer_pos - 1; j++) {
						iter++;
					}
					string back = iter->grammer;
					back=back.substr(1, back.length() - 1);
					reverse(back.begin(), back.end());
					cout << back<<endl;
					for (int j = 0; j < back.length(); j++) {
						if(back[j]!=empty)
						fenxizhan.push(back[j]);
					}
				}
			}
			else {
				break;
			}
		}
	}
	return true;
};

int main()
{
	parser P;
	P.init("grammer.txt","fuhao.txt");
	string w = "x+y*(3*a+7-b/5)";
	cout<<P.analyser(w);
	return 0;
}