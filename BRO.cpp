// RETEST.cpp : 定义控制台应用程序的入口点。
//


/* **********************************************
Auther: fangyh09
Created Time: 2015/11/6 13:24:11
File Name   : RE.cpp
*********************************************** */
#include <iostream>
#include <cstring>
#include <deque>
#include <cmath>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
using namespace std;



//equal
//less than more than 
enum TOKENNAME {ID,NUMBER,RELOP,BOLOP,LBRACKET,RBRACKET,SPE}; 
enum RELOPVALUE {LT,LE,EQ,NE,GT,GE,NONE};
enum BOLOPVALUE {OR,AND,NOT};
struct Token
{
   TOKENNAME tname;
   RELOPVALUE relopvalue;
   BOLOPVALUE bolopvalue;
   string id;
   int val;  //not contain double...     
};

struct Exp
{
	Token rop;
	Token left;
	Token right;
};
class LA
{
public:
	LA()
	{
		s = "";
		curpos = 0;
	}
	void read(string filename)
	{
		freopen(filename.c_str(),"r",stdin);
		getline(cin,s);
		fclose(stdin);
		freopen("CON","r",stdin);
	}
	bool getNextToken(Token& token)
	{
		int sz = s.size();
		while(curpos<sz && s[curpos] == ' ')
		{
			curpos ++;
		}

		if(curpos >= sz)
		{
			return false;
		}
		if(s[curpos] == '(')
		{
			token.tname = LBRACKET;
			curpos ++;
		}
		else if(s[curpos] == ')')
		{
			token.tname = RBRACKET;
			curpos ++;
		}
		else if(s[curpos] == '=')
		{
			token.tname = RELOP;
			token.relopvalue = EQ;
			curpos ++;
			if(curpos < sz && s[curpos] == '=')
			{
				curpos ++; 
			}
		}
		else if(s[curpos] == '<')
		{
			if(curpos + 1 < sz && s[curpos+1] == '=')
			{
				token.tname = RELOP;
				token.relopvalue = LE;
				curpos += 2;
			}
			else
			{
				token.tname = RELOP;
				token.relopvalue = LT;
				curpos ++;
			}
		}
		else if(s[curpos] == '>')
		{
			if(curpos +1 <sz && s[curpos+1] == '=')
			{
				token.tname = RELOP;
				token.relopvalue = GE;
				curpos += 2;
			}
			else
			{
				token.tname = RELOP;
				token.relopvalue = GT;
				curpos ++;
			}
		}
		else if(s[curpos] == '!')
		{
			if(curpos + 1 < sz && s[curpos+1]  == '=')
			{
				token.tname = RELOP;
				token.relopvalue = NE;
				curpos += 2;
			}
			else
			{
				token.tname = BOLOP;
				token.bolopvalue = NOT;
				curpos ++;
			}
		}
		else if(s[curpos] == '&')
		{
			token.tname = BOLOP;
			token.bolopvalue = AND;
			curpos += 2;
		}
		else if(s[curpos] == '|')
		{
			token.tname = BOLOP;
			token.bolopvalue = OR;
			curpos += 2;
		}
		else if(isalpha(s[curpos]) || s[curpos] == '_')
		{
			token.tname = ID;
	        string tmp = "";
			tmp = tmp + s[curpos];
			curpos ++;
			while(curpos < sz && isalpha(s[curpos]))
			{
			   tmp = tmp + s[curpos];	
			   curpos++;
			}
			token.id = tmp;
		}
		else if(isdigit(s[curpos]))
		{
			token.tname = NUMBER;
			int sum = 0;
			sum += s[curpos] - '0';
			curpos ++;
			while(curpos < sz && isdigit(s[curpos]))
			{
				sum = sum * 10 + (s[curpos] - '0');
				curpos ++;
			}
			token.val = sum;
		}
		return true;
	}
	
private:
	string s;
	int curpos;
};
#define prln(x) cout << #x << x << endl

const int maxn = 110;
const int maxm = 110;
struct BoolExp
{
  vector<Token> vec[maxn];
};
struct Tnode
{
	Token t;
	Tnode* left;
	Tnode* right;
	Token token;
	vector<vector<string> > True;
	vector<vector<string> > False;
	
	Tnode(Token t)
	{
		token = t;
		left = right = NULL;
	}
	Tnode()
	{
		left = right = NULL;
		
	}
};

class ExpTree
{
public:
	~ExpTree()
	{
		if(root != NULL)
	    {
			dfsdelete(root);
		}
	}

	void dfsdelete(Tnode* root)
	{
		if(root->left != NULL)
		{
			dfsdelete(root->left);
		}
		if(root->right != NULL)
		{
			dfsdelete(root->right);
		}
		delete root;
	}

	
	void build(Tnode* &t,vector<Token> str)
	{
		stack<Tnode*> nodestack;
		Token token;
		int sz = str.size();
		for(int i=0;i<sz;i++)
		{
			token = str[i];
			if(token.tname == SPE)
			{
				t = new Tnode(token);
				nodestack.push(t);
			}
			else 
			{
				t = new Tnode(token);
				if(token.bolopvalue == NOT)
				{
					t->left = nodestack.top();
					nodestack.pop();
					nodestack.push(t);
				}
				else if(token.bolopvalue == AND || token.bolopvalue == OR)
				{
					if(!nodestack.empty())
					{
						t->right = nodestack.top();
						nodestack.pop();
					}
					if(!nodestack.empty())
					{
						t->left = nodestack.top();
						nodestack.pop();
					}
					nodestack.push(t);
				}
				else
				{
					cout << "error" << endl;
				}
			}
		}
		root = t;
	}
	vector<vector<string> > ONTO(vector<vector<string> > x,vector<vector<string> > y) //on to xor
		{
			vector<vector<string> > ret;
			vector<string> single;
			ret.clear();
			int xnums = x.size();
			int ynums = y.size();
			int maxnums = max(xnums,ynums);
			for(int i=0;i<maxnums;i++)
			{
				single.clear();
				single.insert(single.end(),x[i % xnums].begin(),x[i % xnums].end());
				single.insert(single.end(),y[i % ynums].begin(),y[i % ynums].end());
				ret.push_back(single);
			}
			return ret;
		}
	void labletf()
	{
		if(root != NULL)
		{
			innerLabletf(root);
		}
	}
	
	vector<string> getReverseRelop(string str)
	{
		vector<string> ret;
		ret.clear();
		if(str == "<")
		{
			ret.push_back("=");
			ret.push_back(">");
		}
		else if(str == "=")
		{
			ret.push_back("<");
			ret.push_back(">");
		}
		else if(str == ">")
		{
			ret.push_back("<");
			ret.push_back("=");
		}
		else if(str == "t")
		{
			ret.push_back("f");
		}
		else if(str == "f")
		{
			ret.push_back("t");
		}
		else
		{
			cout << "error" << endl;
		}
		return ret;
	}
	void innerLabletf(Tnode* root)
	{
		if(root->left != NULL)
		{
			innerLabletf(root->left);
		}
		if(root->right != NULL)
		{
			innerLabletf(root->right);
		}
		if(root->token.tname == SPE)
		{
			if(root->token.relopvalue == NONE)
			{
				vector<string> tvec;
				tvec.push_back("t");
				root->True.push_back(tvec);

				vector<string> fvec;
				fvec.push_back("f");
				root->False.push_back(fvec);
			}
			else
			{  //need refine later
				if(root->token.relopvalue == LT)
				{
					vector<string> vec;

					vec.clear();
				   	vec.push_back("<");
					root->True.push_back(vec);

					//false
					vec.clear();
				   	vec.push_back(">");
					root->False.push_back(vec);

					vec.clear();
					vec.push_back("=");
					root->False.push_back(vec);
				}
				else if(root->token.relopvalue == LE)
				{
					vector<string> vec;

					vec.clear();
				   	vec.push_back("<");
					root->True.push_back(vec);

					vec.clear();
				   	vec.push_back("=");
					root->True.push_back(vec);

					//false
					vec.clear();
				   	vec.push_back(">");
					root->False.push_back(vec);

					
			
				}
				else if(root->token.relopvalue == NE)
				{
					vector<string> vec;

					vec.clear();
				   	vec.push_back("<");
					root->True.push_back(vec);

					vec.clear();
				   	vec.push_back(">");
					root->True.push_back(vec);

					//false
					vec.clear();
				   	vec.push_back("=");
					root->False.push_back(vec);

				}
				else if(root->token.relopvalue == EQ)
				{
					vector<string> vec;

					vec.clear();
				   	vec.push_back("=");
					root->True.push_back(vec);

					

					//false
					vec.clear();
				   	vec.push_back("<");
					root->False.push_back(vec);
					vec.clear();
				   	vec.push_back(">");
					root->False.push_back(vec);
				}
				else if(root->token.relopvalue == GT)
				{
					vector<string> vec;

					vec.clear();
				   	vec.push_back(">");
					root->True.push_back(vec);

					

					//false
					vec.clear();
				   	vec.push_back("<");
					root->False.push_back(vec);
					vec.clear();
				   	vec.push_back("=");
					root->False.push_back(vec);
				}
				else if(root->token.relopvalue == GE)
				{
					vector<string> vec;

					vec.clear();
				   	vec.push_back(">");
					root->True.push_back(vec);

					vec.clear();
				   	vec.push_back("=");
					root->True.push_back(vec);

					

					//false
					vec.clear();
				   	vec.push_back("<");
					root->False.push_back(vec);
				}

			}
		}
		else if(root->token.tname == BOLOP)
		{
			if(root->token.bolopvalue == NOT)
			{
			
				root->True = root->left->False;
				
				root->False = root->left->True;
			}
			else if(root->token.bolopvalue == AND)
			{
				vector<vector<string> > ret;
				ret = ONTO(root->left->True,root->right->True);

				root->True = ret;

				ret.clear();
				int lsz = root->left->False.size();
				//exists true[0]
				vector<string> singleRightTrue = root->right->True[0];
				for(int i=0;i<lsz;i++)
				{
					vector<string> tmp = root->left->False[i];
					tmp.insert(tmp.end(),singleRightTrue.begin(),singleRightTrue.end());
					ret.push_back(tmp);
				}

				int rsz = root->right->False.size();
				vector<string> singleLeftTrue = root->left->True[0];
				
				for(int i=0;i<rsz;i++)
				{
					vector<string> tmp = singleLeftTrue;
					vector<string> rfalse = root->right->False[i];
					tmp.insert(tmp.end(),rfalse.begin(),rfalse.end());
					ret.push_back(tmp);
				}
				root->False = ret;
			}
			else if(root->token.bolopvalue == OR)
			{
				vector<vector<string> > ret;
				ret = ONTO(root->left->False,root->right->False);

				root->False = ret;

				ret.clear();
				int lsz = root->left->True.size();
				//exists true[0]
				vector<string> singleRightFalse = root->right->False[0];
				for(int i=0;i<lsz;i++)
				{
					vector<string> tmp = root->left->True[i];
					tmp.insert(tmp.end(),singleRightFalse.begin(),singleRightFalse.end());
					ret.push_back(tmp);
				}

				int rsz = root->right->True.size();
				vector<string> singleLeftFalse = root->left->False[0];
				
				for(int i=0;i<rsz;i++)
				{
					vector<string> tmp = singleLeftFalse;
					vector<string> rtrue = root->right->True[i];
					tmp.insert(tmp.end(),rtrue.begin(),rtrue.end());
					ret.push_back(tmp);
				}
				root->True = ret;
			}
		}
		else
		{
			cout << "error" << endl;
		}
		
		
	}

	void printResult()
	{
		cout << "The constraints are: " << endl;
		vector<vector<string> > t = root->True;
		vector<vector<string> > f = root->False;
		int tsz = t.size();
		int fsz = f.size();
		for(int i=0;i<tsz;i++)
		{
			int len = t[i].size();
			printf("(");	
			for(int j=0;j<len;j++)
			{
				if(j == len -1)
				{
					cout << t[i][j] << ")";
				}
				else
				{
					cout << t[i][j] << ",";
				}
			}
			cout << endl;
		}
		for(int i=0;i<fsz;i++)
		{
			int len = f[i].size();
			printf("(");	
			for(int j=0;j<len;j++)
			{
				if(j == len -1)
				{
					cout << f[i][j] << ")";
				}
				else
				{
					cout << f[i][j] << ",";
				}
			}
			cout << endl;
		}

	}
private:
	Tnode* root;
};
class SA
{
public:
	void read(string filename)
	{
		la.read(filename);
	}
	void getTokens()
	{
		tokens.clear();
		Token t;
		while(la.getNextToken(t))
		{
			tokens.push_back(t);
		}
	}

   	void getReExp()
	{
		boexpNums = 0; 
		ReExp.clear();
		getTokens();
	    int sz = tokens.size();
		int cur = 0;
		while(cur < sz)
		{
			if(tokens[cur].tname == BOLOP)
			{
				ReExp.push_back(tokens[cur]);
				cur ++;
			}
			else if(tokens[cur].tname == LBRACKET)
			{
				ReExp.push_back(tokens[cur]);
				cur ++;
			}
			else if(tokens[cur].tname == RBRACKET)
			{
				ReExp.push_back(tokens[cur]);
				cur ++;
			}
			else
			{
				bool has = false;
				RELOPVALUE relopvalue;
				while(cur < sz && (tokens[cur].tname == ID ||
							       tokens[cur].tname == NUMBER ||
								   tokens[cur].tname == RELOP))
				{
					boolExp[boexpNums].push_back(tokens[cur]);
					if(tokens[cur].tname == RELOP)
					{
						has = true;
						relopvalue = tokens[cur].relopvalue;
					}
					cur ++;
				}
				Token token;
				token.val = boexpNums;
				token.tname = SPE;
				if(has)
				{
					token.relopvalue = relopvalue;
				}
				else
				{
					token.relopvalue = NONE;
				}
				boexpNums ++;
				
				ReExp.push_back(token);
			}
		}
	}
	vector<Token> getSuReExp()
	{
		getReExp();
		SuReExp.clear();
		stack<Token> op;
		stack<Token> nums;
		while(!op.empty())
		{
			op.pop();
		}
		while(!nums.empty())
		{
			nums.pop();
		}
		int sz = ReExp.size();
		for(int i=0;i<sz;i++)
		{
			TOKENNAME tname = ReExp[i].tname;
			if(tname == SPE)
			{
				nums.push(ReExp[i]);
			}
			else if(tname == LBRACKET)
			{
				op.push(ReExp[i]);
			}
			else if(tname == BOLOP)
			{
				if(op.empty())
				{
					op.push(ReExp[i]);
				}
				else
				{
					while(!op.empty() && op.top().tname != LBRACKET && op.top().bolopvalue >= ReExp[i].bolopvalue)
					{
						Token token = op.top();
						nums.push(token);
						op.pop();
					}
					op.push(ReExp[i]);
				}
			}
			else if(tname == RBRACKET)
			{
				while(!op.empty() && op.top().tname != LBRACKET)
				{
					Token token = op.top();
					nums.push(token);
					op.pop();
				}
				op.pop();
			}
		}
		while(!op.empty())
		{
			nums.push(op.top());
			op.pop();
		}
		stack<Token> val;
		while(!nums.empty())
		{
			val.push(nums.top());
			nums.pop();
		}
		while(!val.empty())
		{
			SuReExp.push_back(val.top());
			val.pop();
		}
	//	printReExp(SuReExp);
		return SuReExp;
	}

	void printBoolExp(int idx)
		{
		/*	int sz = boolExp[idx].size();
			for(int i=0;i<sz;i++)
			{
				cout << boolExp[idx][i].tname << " ";
			}
			cout << endl;*/
		}

	void printReExp(vector<Token>& ReExp)
	{
		int sz = ReExp.size();
		for(int i=0;i<sz;i++)
		{
			Token t = ReExp[i];
			if(t.tname == ID)
			{
				cout << t.id << endl;
			}
			else if(t.tname == NUMBER)
			{
				cout << t.val << endl;
			}
			else if(t.tname == RELOP)
			{
				cout << t.relopvalue << endl;
			}
			else if(t.tname == BOLOP)
			{
				cout << t.bolopvalue << endl;
			}
			else if(t.tname == LBRACKET)
			{
				cout << "(" << endl;
			}
			else if(t.tname == RBRACKET)
			{
				cout << ")" << endl;
			}
			else if(t.tname == SPE)
			{
				cout <<"SPE: " <<  t.val << endl;
				printBoolExp(t.val);
			}
			else 
			{
				cout << "error" << endl;
			}
		}
			}

	void printBoolExp()
	{
		for(int i=0;i<boexpNums;i++)
		{
			cout << i << ": " ;
			int sz = boolExp[i].size();
			for(int j=0;j<sz;j++)
			{
				if(boolExp[i][j].tname == NUMBER)
				{
					cout << " " << boolExp[i][j].val << " ";
				}
				else if(boolExp[i][j].tname == ID)
				{
					cout << " " << boolExp[i][j].id << " "; 
				}
			}
			cout << endl;
		}
	}
private:
	vector<Token> tokens;
	vector<Token> ReExp;
	vector<Token> SuReExp;
	LA la;
	vector<Token> boolExp[maxn];
	int boexpNums;
};
int main()
{

	SA sa;
	ExpTree tree;
	Tnode* root;
	sa.read("D:\\in.txt");
 	vector<Token> sureexp= sa.getSuReExp();
	sa.printBoolExp();
	cout << endl;
	tree.build(root,sureexp);
	tree.labletf();
	tree.printResult();
	
 while(1);
}








