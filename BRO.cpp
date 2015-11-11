/* **********************************************
Auther: fangyh09
Created Time: 2015/11/6 
File Name   : BRO.cpp
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






// equal
//less than more than 
enum TOKENNAME {ID,NUMBER,RELOP,BOLOP,LBRACKET,RBRACKET,SPE};  //identifier,number,relatio noperator,boolean operator, left bracket,right bracket,special
enum RELOPVALUE {LT,LE,EQ,NE,GT,GE,NONE};  // <,<=,=,!=,>,>=,null
enum BOLOPVALUE {OR,AND,NOT};  // || && !

struct Token
{
   TOKENNAME tname; //token类型 
   RELOPVALUE relopvalue;  //关系操作符值
   BOLOPVALUE bolopvalue;  //布尔操作符值
   string id;              //string 
   int val;  //not contain double...  //number     
};

//Expression 表达式
struct Exp 
{
	Token rop;  //操作符   
	Token left;  //左操作数
	Token right; //右操作数
};

//laxiacl analyzer
class LA
{
public:
	LA()
	{
		s = "";
		curpos = 0;
	}

	//从filename文件中读入string
	void read(string filename)
	{
		freopen(filename.c_str(),"r",stdin);
		getline(cin,s);
		fclose(stdin);
		freopen("CON","r",stdin);
	}
	/*
	   获得下一个token,如果成功返回true并且token中的属性赋值好引用返回,否则返回false

	*/
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
		if(s[curpos] == '(')   //(
		{
			token.tname = LBRACKET;
			curpos ++;
		}
		else if(s[curpos] == ')')  //)
		{
			token.tname = RBRACKET;
			curpos ++;
		}
		else if(s[curpos] == '=') // =
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
			if(curpos + 1 < sz && s[curpos+1] == '=') // <=
			{
				token.tname = RELOP;
				token.relopvalue = LE;
				curpos += 2;
			}
			else    // < 
			{
				token.tname = RELOP;
				token.relopvalue = LT;
				curpos ++;
			}
		}
		else if(s[curpos] == '>')
		{
			if(curpos +1 <sz && s[curpos+1] == '=') // >=
			{
				token.tname = RELOP;
				token.relopvalue = GE;
				curpos += 2;
			}
			else   // >
			{
				token.tname = RELOP;
				token.relopvalue = GT;
				curpos ++;
			}
		}
		else if(s[curpos] == '!')
		{
			if(curpos + 1 < sz && s[curpos+1]  == '=') //!=
			{
				token.tname = RELOP;
				token.relopvalue = NE;
				curpos += 2;
			}
			else  // !
			{
				token.tname = BOLOP;
				token.bolopvalue = NOT;
				curpos ++;
			}
		}
		else if(s[curpos] == '&') // &&
		{
			token.tname = BOLOP;
			token.bolopvalue = AND;
			curpos += 2;
		}
		else if(s[curpos] == '|') // ||
		{
			token.tname = BOLOP;
			token.bolopvalue = OR;
			curpos += 2;
		}
		else if(isalpha(s[curpos]) || s[curpos] == '_') //标识符
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
		else if(isdigit(s[curpos])) //数字
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
	string s;   //输入string
	int curpos; //当前指针位置
};
#define prln(x) cout << #x << x << endl

const int maxn = 110;
const int maxm = 110;

//布尔表达式
struct BoolExp
{
  vector<Token> vec[maxn];
};

//语法树的节点
struct Tnode
{  
	Token t;  
	Tnode* left;  //left children
	Tnode* right; //right children
	Token token; 
	vector<vector<string> > True; //表达式为真的 约束集合
	vector<vector<string> > False; //表达式为假的 约束集合
	
	Tnode(Token t) //constructor
	{
		token = t;
		left = right = NULL;
	}
	Tnode()
	{
		left = right = NULL;
		
	}
};


//表达式树
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
	//析构函数 释放内存
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

	//根据输入str和根节点t建立语法树
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
	//ONTO 函数
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

	//给每个节点的True集合与False集合赋值
	void labletf()
	{
		if(root != NULL)
		{
			innerLabletf(root);
		}
	}
	//get suffix expression according to the str
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
	//给每个节点的True集合与False集合赋值
	//代码需要些优化(提取子函数)...
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
		if(root->token.tname == SPE) //如果是叶子节点
		{
			if(root->token.relopvalue == NONE) //如果该节点没有关系操作符
			{
				vector<string> tvec;     //True集合中加入 "t"
				tvec.push_back("t");
				root->True.push_back(tvec);

				vector<string> fvec;
				fvec.push_back("f");    //False集合中加入 "f"
				root->False.push_back(fvec);
			}
			else
			{  //need refine later
				if(root->token.relopvalue == LT) // < 
				{
					vector<string> vec;

					vec.clear();
				   	vec.push_back("<");    //True集合中加入 "<"
					root->True.push_back(vec);

					//false
					vec.clear();
				   	vec.push_back(">");   //False集合中加入">","="
					root->False.push_back(vec);

					vec.clear();
					vec.push_back("=");
					root->False.push_back(vec);
				}
				else if(root->token.relopvalue == LE) // <=
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
				else if(root->token.relopvalue == NE) //!=
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
				else if(root->token.relopvalue == EQ) // =
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
				else if(root->token.relopvalue == GT) // >
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
				else if(root->token.relopvalue == GE) // >= 
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
		else if(root->token.tname == BOLOP) //如果是内部节点
		{
			if(root->token.bolopvalue == NOT) // !
			{
			
				root->True = root->left->False;
				
				root->False = root->left->True;
			}
			else if(root->token.bolopvalue == AND) // && 
			{

				//true集合的计算 {lefttrue} ONTO {rightture}
				vector<vector<string> > ret;
				ret = ONTO(root->left->True,root->right->True);

				root->True = ret;

				
				//false集合的计算

				ret.clear();
				int lsz = root->left->False.size();
				//exists true[0]
				vector<string> singleRightTrue = root->right->True[0]; //rightTrue
				//{leftFalse} XOR rightTrue
				for(int i=0;i<lsz;i++)
				{
					vector<string> tmp = root->left->False[i];
					tmp.insert(tmp.end(),singleRightTrue.begin(),singleRightTrue.end());
					ret.push_back(tmp);
				}

				int rsz = root->right->False.size();
				vector<string> singleLeftTrue = root->left->True[0]; //leftTrue
				//leftTrue XOR {rightFalse}
				for(int i=0;i<rsz;i++)
				{
					vector<string> tmp = singleLeftTrue;
					vector<string> rfalse = root->right->False[i];
					tmp.insert(tmp.end(),rfalse.begin(),rfalse.end());
					ret.push_back(tmp);
				}
				root->False = ret;
			}
			else if(root->token.bolopvalue == OR) // ||
			{
				//False集合的计算 {leftfalse} ONTO {rightfalse}
				vector<vector<string> > ret;
				ret = ONTO(root->left->False,root->right->False);

				root->False = ret;
				
				//True集合的计算 
				ret.clear();
				int lsz = root->left->True.size();
				//exists true[0]
				vector<string> singleRightFalse = root->right->False[0]; //rightfalse
				//{leftTrue} XOR rightFalse
				for(int i=0;i<lsz;i++)
				{
					vector<string> tmp = root->left->True[i];
					tmp.insert(tmp.end(),singleRightFalse.begin(),singleRightFalse.end());
					ret.push_back(tmp);
				}

				int rsz = root->right->True.size();
				vector<string> singleLeftFalse = root->left->False[0]; //leftfalse

				//leftFase XOR {rightTrue}
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
	//输出结果
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

//sytanx analyzer
class SA
{
public:
	//从filename读入需要解析的string
	void read(string filename)
	{
		la.read(filename);
	}
	//获得tokens
	void getTokens()
	{
		tokens.clear();
		Token t;
		while(la.getNextToken(t))
		{
			tokens.push_back(t);
		}
	}
	// 将token组装成关系表达式
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
	//get suffix expression
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
	//output for test
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
	//output for test
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
	vector<Token> tokens; //token集合
	vector<Token> ReExp;  //
	vector<Token> SuReExp; //suffix expression
	LA la;
	vector<Token> boolExp[maxn]; //叶子节点的关系表达式
	int boexpNums; //叶子节点的数量
};
int main()
{

	SA sa;
	ExpTree tree;
	Tnode* root;
	sa.read("D:\\in.txt");
 	vector<Token> sureexp= sa.getSuReExp(); //get suffix expression
	sa.printBoolExp();
	cout << endl;
	tree.build(root,sureexp); //建立语法树
	tree.labletf();    //给每个节点的true false集合赋值
	tree.printResult(); //输出结果
	
 while(1);
}








