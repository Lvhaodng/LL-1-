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


class parser {
public:
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

	bool is_right(vector<token>& shuruchuan);
	char empty;
	vector<char> Vt;
	vector<char> Vn;
	int vt_num;
	int vn_num;
	vector<string> gramer_vec;

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
	void fillin_table();
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

void parser::set_first() {

	for (auto i = vtinfo.begin(); i != vtinfo.end(); i++) {
			(i->first).push_back(i->vt);
	}

	for (auto p = vninfo.begin(); p != vninfo.end(); p++) {
		for (auto iter_gr = gramer_vec.begin(); iter_gr != gramer_vec.end(); iter_gr++) {
			if ((*iter_gr)[0] == p->vn) {
				string right = "";
				right = (*iter_gr).substr(1, (*iter_gr).length() - 1);
				p->first.push_back(get_first(right, *p));
			}
		}
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
			return w[i];
		}
		else if (is_vn(w[i])) {
			for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
				if (iter->vn == w[i]) {
					auto p = iter->first;
					if (p.empty()) {
						qiufirst(*iter);
						iter--;
					}
					else {
						if (iter->empty) {
							for (auto i = p.begin(); i != p.end(); i++) {
								det.first.push_back(*i);
							}
							break;
						}
						else {
							for (auto i = p.begin(); i != p.end(); i++) {
								det.first.push_back(*i);
							}
							det.first.pop_back();
							return *(p.end() - 1);
						}
					}
				}
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
			for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
				if (iter->vn == w[i]) {
					auto p = iter->first;
					if (p.empty()) {
						qiufirst(*iter);
						iter--;
					}
					else {
						if (iter->empty) {
							for (auto i = p.begin(); i != p.end(); i++) {
								det.select.push_back(*i);
							}
							break;
						}
						else {
							for (auto i = p.begin(); i != p.end(); i++) {
								det.select.push_back(*i);
							}
							det.select.pop_back();
							return *(p.end() - 1);
						}
					}
				}
			}
		}
		else {
			return empty;
		}
	}
}

void parser::set_follow() {
	for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
		for (auto sen = gramer_vec.begin(); sen != gramer_vec.end(); sen++) {
			if (iter->vn == (*gramer_vec.begin())[0]) {
				iter->follow.push_back('#');
			}
			string now = sen->substr(1, sen->length() - 1);
			for (int i = 0;i < now.length(); i++) {
				if (now[i] == iter->vn) {
					if (i + 1 != now.length()) {
						char next = now[i + 1];
						if (is_vt(next)) {
							iter->follow.push_back(next);
						}
						else if (is_vn(next)) {
							string right = now.substr(i+1,now.length()-i-1);
							if (right_empty(right)) {
								auto left = vninfo.begin();
								for (; left != vninfo.end() && left->vn != (*sen)[0]; left++) {}
								if (left->follow.empty()) {
									get_follow(*left);
								}
								iter->follow.insert(iter->follow.end(), left->follow.begin(), left->follow.end());
							}
								bool flag = true;
								for (int j = i + 1; j < now.length()&&flag==true;j++) {
									char ch = now[j];
									for (auto vnp = vninfo.begin(); vnp != vninfo.end(); vnp++) {
										if (vnp->vn == ch) {
											if (vnp->empty) {
												iter->follow.insert(iter->follow.end(), vnp->first.begin(), vnp->first.end());
											}
											else {
												iter->follow.insert(iter->follow.end(), vnp->first.begin(), vnp->first.end());
												flag = false;
											}
											break;
										}
									}
								}
						}
					}
					if (i +1== now.length()) {
						for (auto left = vninfo.begin(); left != vninfo.end(); left++) {
							if (left->vn == (*sen)[0]) {
								if(!left->follow.empty())
								iter->follow.insert(iter->follow.end(), left->follow.begin(), left->follow.end());
								else {
									get_follow(*left);
								}
							}
						}
					}
				}
			}
		}
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
							auto left = vninfo.begin();
							for (; left != vninfo.end() && left->vn != (*sen)[0]; left++) {}
							if (left->follow.empty()) {
								get_follow(*left);
							}
							det.follow.insert(det.follow.end(), left->follow.begin(), left->follow.end());
						}
							bool flag = true;
							for (int j = i + 1; j < now.length() && flag == true; j++) {
								char ch = now[j];
								for (auto vnp = vninfo.begin(); vnp != vninfo.end(); vnp++) {
									if (vnp->vn == ch) {
										if (vnp->empty) {
											det.follow.insert(det.follow.end(), vnp->first.begin(), vnp->first.end());
										}
										else {
											det.follow.insert(det.follow.end(), vnp->first.begin(), vnp->first.end());
											flag = false;
										}
										break;
									}
								}
							}
					}
				}
				if (i + 1 == now.length()) {
					for (auto left = vninfo.begin(); left != vninfo.end(); left++) {
						if (left->vn == (*sen)[0]) {
							if (!left->follow.empty())
								det.follow.insert(det.follow.end(), left->follow.begin(), left->follow.end());
							else {
								get_follow(*left);
								det.follow.insert(det.follow.end(), left->follow.begin(), left->follow.end());
							}
						}
					}
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
			for (auto left = vninfo.begin(); left != vninfo.end(); left++) {
				if (left->vn == now[0]) {
					iter->select.insert(iter->select.end(), left->follow.begin(), left->follow.end());
					break;
				}
			}
		}
		else {
			iter->select.push_back(get_first(right, *iter));
		}
	}
	for (auto iter = grammerinfo.begin(); iter != grammerinfo.end(); iter++) {						
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
			bool flag = true;
			if ((*sen)[0] == iter->vn) {
				string right = sen->substr(1, sen->length() - 1);
				for (int i = 0; i < right.length() && flag == true; i++) {
					if (right.length() == 1 && right[i] == empty &&iter->setable) {
						iter->empty = 1;
						flag = false;
					}
					else if (is_vt(right[i]) && iter->setable) {
						iter->empty = 0;
						flag = false;
					}
					else if (is_vn(right[i]) && iter->setable) {
						for (auto ch = vninfo.begin(); ch != vninfo.end(); ch++) {
							if (ch->vn == right[i]) {
								if (ch->empty == -1) {
									is_empty(*ch);
								}
								if (ch->empty == 0) {
									iter->empty = 0;
									flag = false;
								}
								else if (ch->empty == 1 && i == right.length() - 1) {
									iter->empty = 1;
									flag = false;
									iter->setable = false;
								}
							}
						}
					}
				}
			}
		}
	}
}

void parser::is_empty(VN& det) {
	for (auto sen = gramer_vec.begin(); sen != gramer_vec.end(); sen++) {
		bool flag = true;
		if ((*sen)[0] == det.vn) {
			string right = sen->substr(1, sen->length() - 1);
			for (int i = 0; i < right.length() && flag == true; i++) {
				if (right.length() == 1 && right[i] == empty && det.setable) {
					det.empty = 1;
					flag = false;
				}
				else if (is_vt(right[i]) && det.setable) {
					det.empty = 0;
					flag = false;
				}
				else if (is_vn(right[i]) && det.setable) {
					for (auto ch = vninfo.begin(); ch != vninfo.end(); ch++) {
						if (ch->vn == right[i]) {
							if (ch->empty == -1) {
								is_empty(*ch);
							}
							if (ch->empty == 0) {
								det.empty = 0;
								flag = false;
							}
							else if (ch->empty == 1 && i == right.length() - 1) {
								det.empty = 1;
								flag = false;
								det.setable = false;
							}
						}
					}
				}
			}
		}
		flag = true;
	}
}

bool parser::right_empty(string w) {
	for (int i = 0; i < w.length(); i++) {
		if (is_vt(w[i])) {
			return false;
		}
		else if (is_vn(w[i])) {
			for (auto iter = vninfo.begin(); iter != vninfo.end(); iter++) {
				if (iter->vn == w[i]) {
					if (iter->empty == 0) {
						return false;
					}
					break;
				}
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
			auto l = vninfo.begin();
			for (; l != vninfo.end() && l->vn != left; l++) {
				vn_pos++;
			}
			char t = *p;
			auto zhongjiefu = vtinfo.begin();
			for (; zhongjiefu != vtinfo.end() && zhongjiefu->vt != t; zhongjiefu++) {
				vt_pos++;
			}
			//todo  cout << vn_pos << endl<<vt_pos<<endl<<gram_pos<<endl<<endl;
			table.table[vn_pos][vt_pos] = gram_pos;
			vn_pos = 0;
			vt_pos = 0;
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
			
			//for(auto iter=fenxizhan.)




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
				//cout << gramer_pos;
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
					//cout << back;
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
	string w = "x+y*(3*a+7-b/5";
	cout<<P.analyser(w);
	return 0;
}