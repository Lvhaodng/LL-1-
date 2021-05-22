#include"cifafenxiqi.h"

vector<token> tokenarr;
vector<identifier> idarr;
vector<constnumber> ctarr;

string keyword[14] = { "int","void","break","float","while","do","struct","const","case","for","return","if","default","else" };
string pword[16] = { "-","/","(",")","==","<=","<","+","*",">","=",",",";","++","{","}" };

void shengcheng(int clas, int id) {
	token* tk = new token;
	switch (clas)
	{
	case 0:
		tk->clas = 'K';
		break;
	case 1:
		tk->clas = 'P';
		break;
	case 2:
		tk->clas = 'I';
		break;
	case 3:
		tk->clas = 'C';
		break;
	}
	tk->id = id;
	tokenarr.push_back(*tk);
}


void showtkarr(ofstream& outfile) {
	for (auto iter = tokenarr.cbegin(); iter != tokenarr.cend(); iter++) {
		outfile << "(" << "" << iter->clas << " " << iter->id << ")";
	}
	outfile << endl;
}

void showidarr(ofstream& outfile) {
	outfile << "标识符表:";
	for (auto iter = idarr.cbegin(); iter != idarr.cend(); iter++) {
		outfile << iter->name << ",";
	}
	outfile << endl;
}

void showctarr(ofstream& outfile) {
	outfile << "常数表:";
	for (auto iter = ctarr.cbegin(); iter != ctarr.cend(); iter++) {
		outfile << iter->num << " ";
	}
	outfile << endl;
}

int next_state(int pri_state, char ch) {
	switch (pri_state)
	{
	case 0: {
		if (ch == ' ') {
			return 0;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			return 1;
		}
		else if (ch >= 'a' && ch <= 'z') {
			return 1;
		}
		else if (ch >= '0' && ch <= '9') {
			return 2;					//shuzi zhuangtai
		}
		else {
			switch (ch)
			{
			case '-':
			case '+':
			case '>':
			case '<':
			case '=':
			case '*':
			case '/':
			case '(':
			case ')':
			case ';':
			case '{':
			case '}':
			case ',':
				return 4;

			default:
				break;
			}
		}
		break;
	}
	case 1:
		if (ch == ' ') {
			return 0;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			return 1;
		}
		else if (ch >= 'a' && ch <= 'z') {
			return 1;
		}
		else if (ch >= '0' && ch <= '9') {
			return 1;
		}
		else {
			switch (ch)
			{
			case '-':
			case '+':
			case '>':
			case '<':
			case '=':
			case '*':
			case '/':
			case '(':
			case ')':
			case ';':
			case '{':
			case '}':
			case ',':
				return 4;

			default:
				break;
			}
		}

	case 2:
		if (ch == ' ') {
			return 0;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			return 1;
		}
		else if (ch >= 'a' && ch <= 'z') {
			return 1;
		}
		else if (ch >= '0' && ch <= '9') {
			return 2;
		}
		else {
			switch (ch)
			{
			case '@':
				//cout << "???";
				return -1;
				break;
			case '-':
			case '+':
			case '>':
			case '<':
			case '=':
			case '*':
			case '/':
			case '(':
			case ')':
			case ';':
			case '{':
			case '}':
			case ',':
				return 4;
			default:
				cout << "有非法符号" << endl;
				break;
			}
		}
	case 4:
		if (ch == ' ') {
			return 0;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			return 1;
		}
		else if (ch >= 'a' && ch <= 'z') {
			return 1;
		}
		else if (ch >= '0' && ch <= '9') {
			return 2;
		}
		else if (ch == '@') {
			return -1;
		}
		else {
			return 4;
		}
		break;
	}
}

int is_keyword(string str) {
	for (int i = 0; i < 14; i++) {
		if (str == keyword[i]) {
			shengcheng(0, i + 1);
			return i + 1;
		}
	}
	return -1;
}

int is_pword(string str) {
	for (int i = 0; i < 16; i++) {
		if (str == pword[i]) {
			shengcheng(1, i + 1);
			return i + 1;
		}
	}if (str.size() == 2) {
		for (int j = 0; j < str.size(); j++) {
			string str1 = "";
			str1.append(1, str[j]);
			//cout << str1;
			for (int i = 0; i < 16; i++) {
				if (str1 == pword[i]) {
					shengcheng(1, i + 1);
				}
			}
		}
		return 1;
	}
	else {
		cout << "有非法界符" << str << endl;
	}
	return -1;
}

void show() {
	cout << "Token序列：";
	for (auto iter = tokenarr.cbegin(); iter != tokenarr.cend(); iter++) {
		cout << "(" << "" << iter->clas << " " << iter->id << ")";
	}
	cout << endl;
	cout << "标识符表：";
	for (auto iter = idarr.begin(); iter != idarr.end(); iter++) {
		if ((iter + 1) != idarr.end())
			cout << iter->name << " ";
		else
			cout << iter->name;
	}
	cout << endl;
	cout << "常数表：";
	for (auto iter = ctarr.cbegin(); iter != ctarr.cend(); iter++) {
		if ((iter + 1) != ctarr.end())
			cout << iter->num << " ";
		else
			cout << iter->num;
	}
}

int Scanner(string w) {
	int state = 0;
	int nextstate = 0;
	char ch = ' ';
	char nextch = ' ';
	string buffer;
	string idstr = "";

	//	ifstream infile("inputfile.txt");
	//	ofstream outfile("outputfile.txt");
	//	if (!infile.is_open()) {
	//		cout << "未能成功打开文件";
	//		return -1;
	//	}
	//	else {



	//getline(cin, buffer);
	buffer = w;
	
	
	
	
	//int state = 0;
	//int nextstate = 0;
	//string idstr = "";
	buffer.append(1, '@');
	for (int i = 0; i < buffer.size(); i++) {

		ch = buffer[i];
		nextch = buffer[i + 1];
		nextstate = next_state(state, ch);
		//			cout << ch << state << "|" << nextstate << "|" << endl;
		switch (state)
		{
		case 0:
			if (nextstate != 0) {
				idstr.append(1, ch);
			}
			break;
		case 1:
			if (nextstate == 4 || nextstate == -1 || nextstate == 0) {
				int same = 0;
				if (is_keyword(idstr) == -1) {
					for (auto iter = idarr.begin(); iter != idarr.end(); iter++) {
						if (idstr == (*iter).name) {
							same = 1;
							shengcheng(2, iter->id);
							break;
						}
					}
					if (same == 0) {
						if (!idarr.empty()) {
							identifier* idtok = new identifier;
							auto iter = idarr.rbegin();
							idtok->id = ((iter)->id) + 1;
							idtok->name = idstr;
							shengcheng(2, idtok->id);
							idarr.push_back(*idtok);
						}
						else {
							identifier* idtok = new identifier;
							idtok->id = 1;
							idtok->name = idstr;
							shengcheng(2, idtok->id);
							idarr.push_back(*idtok);
						}
					}
				}
				idstr = "";
			}
			if (ch != ' ')
				idstr.append(1, ch);
			break;
		case 2:
			if (nextstate == 2) {
				idstr.append(1, ch);
			}
			else {
				int same = 0;
				float num = stoi(idstr);
				for (auto iter = ctarr.begin(); iter != ctarr.end(); iter++) {
					if (num == (*iter).num) {
						same = 1;
						shengcheng(3, iter->id);
						break;
					}
				}
				if (same == 0) {
					if (!ctarr.empty()) {
						constnumber* cttok = new constnumber;
						auto iter = ctarr.rbegin();
						cttok->id = ((iter)->id) + 1;
						cttok->num = num;
						shengcheng(3, cttok->id);
						ctarr.push_back(*cttok);
					}
					else {
						constnumber* cttok = new constnumber;
						cttok->id = 1;
						cttok->num = num;
						shengcheng(3, cttok->id);
						ctarr.push_back(*cttok);
					}
					idstr = "";
				}
				if (ch != ' ')
					idstr.append(1, ch);
			}
			break;
		case 4:
			if (nextstate == 4) {
				idstr.append(1, ch);
			}
			else if (nextstate == -1) {
				is_pword(idstr);
				idstr = "";
			}
			else {
				is_pword(idstr);
				idstr = "";
				if (ch != ' ')
					idstr.append(1, ch);
			}
			break;
		default:
			break;
		}
		state = nextstate;
	}
	//show();
	//		showtkarr(outfile);
	//		showidarr(outfile);
	//		showctarr(outfile);
	//	}
	return 0;
}