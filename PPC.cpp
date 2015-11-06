#include <iostream>
#include <cstring>
#include <cmath>
#include <queue>
#include <stack>
#include <list>
#include <map>
#include <set>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
using namespace std;


const int MAXN = 110;
const int MAXM = 100010;
struct Path
{
	vector<int>p;
	bool operator<(const Path& path) const
	{
		return p.size() < path.p.size();
	}
	bool tour(const Path& path)
	{
		int l1 = p.size();
		int l2 = path.p.size();
		if(l1 < l2) return false;
		for(int i=l2-1;i>=0;i--)
		{
			int dis = l2 - i;
			if(path.p[i] != p[l1- dis])
			{
				return false;
			}
		}
		return true;
	}
	void addElem(int x)
	{
		p.push_back(x);
	}
	void print()
	{
		int sz = p.size();
		for(int i=0;i<sz;i++)
		{
			if(i == 0) printf("[");
			printf("%d%s",p[i]+1,i==sz-1?"":",");
			if(i == sz -1) printf("]");
		}


	}
};

class Graph
{
public:
	void initG()
	{
		for(int i=0;i<MAXN;i++)
		{
			G[i].clear();
		}
	}
	int getPos(const Path&path,int x)
	{
		int sz = path.p.size();
		int ret = -1;
		for(int i=0;i<sz;i++)
		{
			if(path.p[i] == x) return i;
		}
		return ret;
	}
	//图的格式
	// 2 3  表示2条边,3个节点 下面输入2条边
	// 1 2  表示1->2
	// 2 3  表示2->3 
	void read(string filename)
	{
	//	cout << filename << endl;
		freopen(filename.c_str(),"r",stdin);
		initG();
		scanf("%d%d",&m,&n); //m是边的数量,n是点的数量
		for(int i=0;i<m;i++)
		{
			int u,v;
			scanf("%d%d",&u,&v);
			u --;
			v --;
			G[u].push_back(v);
		}
		fclose(stdin);
//		cout << "readok" << endl;
	}
	void init()
	{
		cur.clear();
		pre.clear();
		pp.clear();
	}
	void getPP()
	{
		init();
		for(int i=0;i<n;i++)
		{
			Path p;
			p.addElem(i);
			pre.push_back(p);
		}
		bool update = true;
		while(update)
		{
			update = false;
			cur.clear();
			for(int i=0,sz=pre.size();i<sz;i++)
			{
				Path path = pre[i];
				int len = path.p.size();
				int lastval = path.p[len-1];
				if(G[lastval].size() == 0)
				{
					pp.push_back(path);
				}
				else
				{

				bool found = false;
				for(int j=0,size=G[lastval].size();j<size;j++)
				{
					int position = getPos(path,G[lastval][j]);
					if(position == -1)
					{
						Path tmp = path;
						tmp.addElem(G[lastval][j]);
						cur.push_back(tmp);
						found = true;
					}	
					else if(position == 0)
					{
						Path tmp = path;
						tmp.addElem(G[lastval][j]);
						pp.push_back(tmp);
						found = true;
						
					}
					else
					{
						//do nothing means discard this path
					}

				}
				if (!found)
				{
					pp.push_back(path);
				}
				}
			}
			pre.clear();
			if(cur.size() > 0) update =true;
		//	cout << "#cur.size():" << cur.size() << endl;
			pre = cur;
		//	cout << "#pre.size():" << pre.size() << endl;

		}
	}
	void uniquePP()
	{
		ret.clear();
		sort(pp.begin(),pp.end());
		int sz = pp.size();
		for(int i=0;i<sz;i++)
		{
			bool flag = true;
			for(int j=i+1;j<sz;j++)
			{
				if(pp[j].tour(pp[i]))
				{
					flag = false;
					break;
				}
			}
			if(flag)
			{
				ret.push_back(pp[i]);
			}
		}
	}
	void printPP()
	{
		getPP();
		uniquePP();
		int sz = ret.size();
		printf("There are %d prime paths:\n",sz);
		for(int i=0;i<sz;i++)
		{
			if(i == 0) printf("{");
			ret[i].print();
			printf("%s",i==sz-1?"}\n":",");
		}
	}

private:
	vector<Path> cur;
	vector<Path> pre;
	vector<Path> pp;
	vector<Path> ret;
	vector<int> G[MAXN];   //存储图的信息
	int m,n;             //m是边的数量,n是点的数量
};
int main()
{
	Graph g;
	g.read("in.txt");
	g.printPP();

	while(1);
	return 0;

}
