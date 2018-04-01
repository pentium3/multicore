#include <iostream>
#include <cstring>
#include <fstream>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <algorithm>
using namespace std;
#define MAXN 15
#define INF 888888888

int dis[MAXN][MAXN];		//global var R
int N;						//global var R
int NODE;
int globalANS=INF;			//shared var RW
int globalpath[MAXN];
int unroll[MAXN*MAXN][2];
int unroll_num=0;
int unroll_per_node=0;
//int ALLPATH=0;
//int CNTPATH[NODE];

int path[MAXN];
int thread_id;

void unroll_permute()
{
	//path[0]=0  path[1]=unroll[0]  path[2]=unroll[1]
	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++)
			if(j!=i)
			{
				unroll[unroll_num][0]=i;
				unroll[unroll_num][1]=j;
				unroll_num++;
			}
	unroll_per_node=ceil((double)unroll_num/(double)NODE);
}

void calc_permute()
{
	//for each possible path:
	//    calc_distance
	//    update_if_necessary
	//    get_next_patch
	#pragma omp parallel num_threads(NODE) private(path,thread_id)
	{
		thread_id=omp_get_thread_num();		//0..NODE-1
		//int _i=233; for(int i=1;i<=thread_id*100000000;i++) { _i+=1; _i-=1; }	//delay
		//CNTPATH[thread_id]=0;
		int dx=thread_id*unroll_per_node,ux=min((thread_id+1)*unroll_per_node-1,unroll_num-1);
		int i,tmp=1;
		//cout<<"*********** thread id == "<<thread_id<<" *********** "<<dx<<"----"<<ux<<endl;

		//this thread will process unroll[dx..rx]
		for(int rx=dx;rx<=ux;rx++)
		{
			//cout<<rx<<endl;
			//all paths have prefix{path[0]=0 path[1]=unroll[rx][0] path[2]=unroll[rx][1]}
			path[0]=0;
			path[1]=unroll[rx][0];
			path[2]=unroll[rx][1];
			tmp=1;
			for(i=3;i<N;i++)
			{
				while((path[1]==tmp)||(path[2]==tmp)) tmp++;
				path[i]=tmp;
				tmp++;
			}
			
			do
			{
				//cout<<"  ";
				//for(int i=0;i<N;i++) cout<<path[i]<<" "; cout<<endl;
				//#pragma omp critical
				//{
				//	CNTPATH[thread_id]++;
				//	ALLPATH++;
				//}
				tmp=0;
				for(i=0;i<=N-2;i++)
					tmp+=dis[path[i]][path[i+1]];
				
				if(tmp<globalANS)
				{
					#pragma omp critical
					{
						globalANS=tmp;
						for(i=0;i<N;i++)
							globalpath[i]=path[i];
					}
				}
			}while(next_permutation(path+3,path+N));
			
		}
		#pragma omp barrier
	}
}

int main(int argc, char * argv[])
{
	N=atoi(argv[1]);
	NODE=atoi(argv[2]);
    ifstream ff(argv[3]);
    for(int i=0;i<N;i++)
    	for(int j=0;j<N;j++)
    		ff>>dis[i][j];
    ff.close();    

    unroll_permute();
    calc_permute();
    //cout<<N<<" "<<unroll_num<<" "<<ALLPATH<<endl;
    //for(int i=0;i<NODE;i++)	cout<<CNTPATH[i]<<" "; cout<<endl;
    //for(int i=0;i<unroll_num;i++)	cout<<unroll[i][0]<<unroll[i][1]<<endl;
    cout<<"Shortest path is: "<<endl;
	for(int i=0;i<N;i++)	cout<<globalpath[i]<<" "; cout<<endl;
	cout<<"total weight = "<<globalANS<<endl;

	return 0;
}
