/* **********************************************
Auther: fangyh09
Created Time: 2015-09-24 14:32:25
File Name   : F:\大三上\软件测试\作业\HW3\code\EPC.cpp
*********************************************** */
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstdio>
using namespace std;
    
const int MAXN = 110;
const int MAXM = MAXN * MAXN;

//edge_pair x->y->z
struct EP
{
  int x,y,z;
  EP(int _x,int _y,int _z)
  {
	  x = _x;
	  y = _y;
	  z = _z;
  }  
  EP() {}
};

class Graph
{
public:
	void init()
	{
		memset(G,0,sizeof(G));
	}
	//图的格式
	// 2 3  表示2条边,3个节点 下面输入2条边
	// 1 2  表示1->2
	// 2 3  表示2->3 
	void read(string filename)
	{
     freopen(filename.c_str(),"r",stdin);
		init();
		scanf("%d%d",&m,&n); //m是边的数量,n是点的数量
		for(int i=0;i<m;i++)
		{
			int u,v;
			scanf("%d%d",&u,&v);
			u --; //从0开始
			v --; //从0开始
			G[u][v] = 1; //G[u][v] 是1表示存在一条u->v的有向边.  否则不存在	
		}
	  fclose(stdin);
	}
	void getEP()
	{
		edgepair.clear();
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				for(int k=0;k<n;k++)
				{
					if(G[i][k] && G[k][j]) //G[i][k] = 1 && G[k][j] = 1
					{
						EP ep = EP(i,k,j);
						edgepair.push_back(ep);
					}
				}
			}
		}
	}
	void printEP()
	{
		getEP();
		printf("There are %d edge pairs:\n",edgepair.size());
		for(int i=0,sz=edgepair.size();i<sz;i++)
		{
			EP ep = edgepair[i];
			printf("[%d,%d,%d]\n",ep.x+1,ep.y+1,ep.z+1);
		}

	}
private:
	vector<EP> edgepair; //存储边对集合	
	int G[MAXN][MAXN];   //存储图的信息
	int m,n;             //m是边的数量,n是点的数量
};


int main()
{
    Graph g;	
	g.read("D:\\in.txt");
	g.printEP();
	
	while(1); //使得屏幕不马上退出
    return 0;
}

/* in.txt
9 7
1 2
1 7
2 3
2 4
3 2
4 5
4 6
5 6
6 1
*/
