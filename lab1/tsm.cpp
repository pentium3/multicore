#include <iostream>
#include <cstring>
#include <fstream>
#include <time.h>
using namespace std;
#define MAXN 110
#define INF 888888888

int dis[MAXN][MAXN];
int N;
int ANS;

bool vis[MAXN];
int path[MAXN];
int ANSpath[MAXN];

void dfs(int x,int tmp)
{
	if(x==N+1)
	{
		if(tmp<ANS)
		{
			ANS=tmp;
			for(int i=1;i<=N;i++)
				ANSpath[i]=path[i];
		}
	}
	else
	{
		if(tmp<=ANS)
		{
			for(int i=1;i<=N;i++)
			{
				if(!vis[i])
				{
					path[x]=i;
					vis[i]=true;
					tmp=tmp+dis[path[x-1]][path[x]];
					dfs(x+1,tmp);
					tmp=tmp-dis[path[x-1]][path[x]];
					vis[i]=false;
					path[x]=0;
				}
			}
		}
	}
}

int main()
{
    ifstream ff("cities10.txt");
    ff>>N;
    for(int i=1;i<=N;i++)
    	for(int j=1;j<=N;j++)
    		ff>>dis[i][j];
    ff.close();    

	ANS=INF;

    path[1]=1;
    vis[1]=true;
    dfs(2,0);

    cout<<ANS<<endl;
    for(int i=1;i<=N;i++)
    	cout<<ANSpath[i]-1<<" ";
    cout<<endl;
    
    cout << "Totle Time : " << (double)clock() /CLOCKS_PER_SEC<< "s" << endl;  
	return 0;
}
