#pragma once

void Update_Instances()
{
	for (int i = 0; i <= origin_Nodes_Num; i++)
	{
		Edge[i].clear();
	}
	Edge_x_y.clear();

	int x, y, edgenum = 0;

	for (int i = 0; i < new_Edge_x_y.size(); i++)
	{
		x = new_Edge_x_y[i].x; y = new_Edge_x_y[i].y;
		if (!important_edge[i]) continue;
		if (!important_node[x] || !important_node[y]) continue;

		Edge_x_y.push_back(new_Edge_x_y[i]);
		Node_Edge node;
		node.id=i;
		node.v=y;
		Edge[x].push_back(node);
		node.v=x;
		Edge[y].push_back(node);
		edgenum++;
	}
	Edges_Num = edgenum;
}

int num;//BCC���� 
int dfs_clock;
int stk[Max_Nodes_Num + 5], top;//ջ 
vector<int>bcc[Max_Nodes_Num + 5];
int tarjan(int u, int fa)
{
	int lowu = dfn[u] = ++dfs_clock;
	for (int i = 0; i < Edge[u].size(); i++)
	{
		int v = Edge[u][i].v;
		if (!dfn[v])
		{
			stk[++top] = v;//�������ĵ���ջ 
			int lowv = tarjan(v, u);
			lowu = min(lowu, lowv);
			if (lowv >= dfn[u])//�Ǹ���� 
			{
				num++;
				while (stk[top] != v)//�����ջֱ��Ŀ��� 
					bcc[num].push_back(stk[top--]);
				bcc[num].push_back(stk[top--]);//Ŀ����ջ 
				bcc[num].push_back(u);//��Ҫ���˽���ǰ�����bcc 
			}
		}
		else if (v != fa)
			lowu = min(lowu, dfn[v]);
	}
	return lowu;
}


void Inital_Graph_Tarjan()
{	
	for (int i = 0; i < min(2, Terminals_Nodes_Num); i++)
	{
		int x = Terminals_Nodes[i];
		for (int j = i+1; j < Terminals_Nodes_Num; j++)
		{
			int y = Terminals_Nodes[j];
			Node_Edge node;
			node.id=-1;
			node.v=y; 
			Edge[x].push_back(node);
			node.v=x;
			Edge[y].push_back(node);
		}
	}

	for (int i = 1; i <= Nodes_Num; i++)
	{
		dfn[i] = 0;
		low[i] = 0;
		bcc[i].clear();
	}
	dfs_clock = num = 0;

	for (int i = 1; i <= Nodes_Num; i++)//����n����tarjan 
	if (!dfn[i])
	{
		stk[top = 1] = i;
		tarjan(i, i);
	}

	int flag = 0;
	for (int i = 1; i <= num; i++)
	{
		int flagnum = 0;
		for (int j = 0; j < bcc[i].size(); j++)
			if (!If_Steiner_Nodes[bcc[i][j]])
				flagnum++;
		if (flagnum == Terminals_Nodes_Num)
		{
			flag = i;
			break;
		}
	}


	for (int i = 0; i < bcc[flag].size(); i++)
		important_node[bcc[flag][i]] = 2;

	for (int i = 1; i <= Nodes_Num; i++)
	{
		if (important_node[i] == 2)
			important_node[i] = 1;
		else important_node[i] = 0;
	}
	
	
	//cout << Nodes_Num<<"   "<<bcc[flag].size() << endl;

}


int Reduction_belong[Max_Nodes_Num + 5];
int Reduction_find_father(int x)
{
	return Reduction_belong[x] = (x == Reduction_belong[x] ? x : Reduction_find_father(Reduction_belong[x]));
}

void Joint_neibor_Terminals()
{
	
	for (int i = 0; i < Terminals_Nodes_Num; i++)
	{
		Reduction_belong[Terminals_Nodes[i]] = Terminals_Nodes[i];
	}

	for (int i = 0; i < Edges_Num; i++)
	{
		int	x = Edge_x_y[i].x, y = Edge_x_y[i].y;
		if (!If_Steiner_Nodes[x] && !If_Steiner_Nodes[y])
		{
			int fx = Reduction_find_father(x), fy = Reduction_find_father(y);
			if (fx != fy)
				Reduction_belong[fx] = fy;
		}
	}

	for (int ii = 0; ii < Terminals_Nodes_Num; ii++)
	{
		int u = Terminals_Nodes[ii];
		int fu = Reduction_belong[u] = Reduction_find_father(u);
		if (u == fu) continue;

		for (int i = 0; i < Edge[u].size(); i++)
		//if (If_Steiner_Nodes[Edge[u][i].v])
		{
			int flag = 1;
			for (int j = 0; j < Edge[fu].size(); j++)
			{
				if (Edge[u][i].v == Edge[fu][j].v)
				{
					flag = 0;
					break;
				}
			}
			important_edge[Edge[u][i].id]=0;
			if (flag&&Edge[u][i].v!=fu)
			{
				Node_Edge_x_y nodexy;
				nodexy.x=fu;
				nodexy.y=Edge[u][i].v;
				nodexy.val=1;
				new_Edge_x_y.push_back(nodexy);

				Node_Edge node;
				node.id=new_Edge_x_y.size()-1;
				node.v=Edge[u][i].v;
				Edge[fu].push_back(node);
			}
		}
		important_terminal[u]=0;

		Node_Edge_x_y nodexy;
		nodexy.x=fu;
		nodexy.y=u;
		nodexy.val=1;
		new_Edge_x_y.push_back(nodexy);
	}
}


void Reduction_Tarjan(int i, int Father)
{
	father[i] = Father;
	dfn[i] = low[i] = tim++;
	for (int j = 0; j < Edge[i].size(); ++j)
	{
		int k = Edge[i][j].v;
		if (!important_node[k]) continue;
		if (dfn[k] == -1)
		{
			Reduction_Tarjan(k, i);
			low[i] = min(low[i], low[k]);
		}
		else if (Father != k)
			low[i] = min(low[i], dfn[k]);
	}
}
void Reduction_count()
{
	int rootson = 0;
	for (int i = 1; i <= Nodes_Num; i++)
		if (important_node[i])
		{
			dfn[i] = -1;
			father[i] = 0;
			low[i] = -1;
			is_cut[i] = false;
		}

	tim = 0;
	Reduction_Tarjan(Terminals_Nodes[0], 0);
	for (int i = 1; i <= Nodes_Num; i++)
		if (important_node[i])
		{
			if (i == Terminals_Nodes[0]) continue;
			int v = father[i];
			if (v == Terminals_Nodes[0])
				rootson++;
			else {
				if (low[i] >= dfn[v])
					is_cut[v] = true;
			}
		}
	if (rootson > 1)
		is_cut[Terminals_Nodes[0]] = true;
}

bool Check_Cut_Edges_with_2_Steiners_as_Endpoint(int x, int y)
{
	bool flag = false;
	for (int j = 0; j < Edge[x].size(); j++)
	{
		int v = Edge[x][j].v;
		if (If_Steiner_Nodes[v]) continue;
		visted[v] = 1;
	}

	for (int j = 0; j < Edge[y].size(); j++)
	{
		int v = Edge[y][j].v;
		if (If_Steiner_Nodes[v]) continue;
		if (visted[v])
		{
			flag = true;
			break;
		}
	}
	for (int j = 0; j < Edge[x].size(); j++)
	{
		int v = Edge[x][j].v;
		if (If_Steiner_Nodes[v]) continue;
		visted[v] = 0;
	}
	return flag;
}
void Cut_Edges_with_2_Steiners_as_Endpoint()
{
	int x, y,id;
	for (int i = 1; i <= Nodes_Num; i++)
		visted[i] = 0;
	for (int i = 0; i < new_Edge_x_y.size(); i++)
	{
		x=new_Edge_x_y[i].x;
		y=new_Edge_x_y[i].y;		
		if (!important_edge[i]) continue;
		if (!important_node[x] || !important_node[y]) continue;

		if (!If_Steiner_Nodes[x] || !If_Steiner_Nodes[y]) continue;

		
		if (Check_Cut_Edges_with_2_Steiners_as_Endpoint(x,y))
		{
			important_edge[i]=0;
		}
	}

}



int edge_father[Max_Edges_Num+5];
int find_edge_father(int x)
{
	return edge_father[x]==x?x:edge_father[x]=find_edge_father(edge_father[x]);
}
void Joint_path()
{
	for(int i=1;i<=Nodes_Num;i++)
	{
		degree[i]=Edge[i].size();
	}

	for(int i=1;i<=Nodes_Num;i++)
	if(If_Steiner_Nodes[i]&&degree[i]==2)
	{
		int x=Edge[i][0].v;
		int y=Edge[i][1].v;
		int idxi=Edge[i][0].id;
		int idyi=Edge[i][1].id;
		important_edge[idxi]=0;
		important_edge[idyi]=0;
		if(x==y) {
			continue;
		}
		int wxi=new_Edge_x_y[idxi].val;
		int wyi=new_Edge_x_y[idyi].val;
		Node_Edge_x_y node;
		node.x=x;
		node.y=y;
		node.val=wxi+wyi;
		new_Edge_x_y.push_back(node);
		
		edge_father[idxi]=edge_father[idyi]=new_Edge_x_y.size()-1;

		for(int j=0;j<Edge[x].size();j++)
		{
			int v=Edge[x][j].v;
			if(v==i)
			{
				Edge[x][j].id=edge_father[idxi];
				Edge[x][j].v=y;
				break;
			}
		}

		for(int j=0;j<Edge[y].size();j++)
		{
			int v=Edge[y][j].v;
			if(v==i)
			{
				Edge[y][j].id=edge_father[idyi];
				Edge[y][j].v=x;
				break;
			}
		}
	}
}

bool comp_for_delete_path(const Node_Edge &a,const Node_Edge &b)
{
	if(a.v!=b.v) return a.v<b.v;
	else return new_Edge_x_y[a.id].val<new_Edge_x_y[b.id].val;
}
void Delete_path1()
{
	for(int i=1;i<=Nodes_Num;i++)
	if(If_Steiner_Nodes[i])
	{
		int idd=-1,mindis;
		for(int j=0;j<Edge[i].size();j++)
		{
			int v=Edge[i][j].v;
			if(If_Steiner_Nodes[v]) continue;
			int id=Edge[i][j].id;
			if(idd==-1)
			{
				mindis=new_Edge_x_y[id].val;
				idd=id;
				continue;
			}
			if(mindis>new_Edge_x_y[id].val)
			{
				important_edge[idd]=0;
				mindis=new_Edge_x_y[id].val;
				idd=id;
				continue;
			}
			else {
				important_edge[id]=0;
			}
		}
	}

	/*
	for(int i=1;i<=Nodes_Num;i++)
	{
		degree[i]=Edge[i].size();
	}
	for(int i=1;i<=Nodes_Num;i++)
	if(degree[i]>=2)
	{
		sort(Edge[i].begin(),Edge[i].end(),comp_for_delete_path);
		for(int j=1;j<degree[i];j++)
		if(Edge[i][j].v>=i)
		if(Edge[i][j].v==Edge[i][j-1].v||Edge[i][j].v==i)
		{
			important_edge[Edge[i][j].id]=0;
		}
	}
	*/
}

queue<int>Voronoi_diagram_que;
int Voronoi_diagram_dis[Max_Nodes_Num + 5];
int Voronoi_diagram_path[Max_Nodes_Num + 5];
int Voronoi_diagram_belong[Max_Nodes_Num + 5];
void Inital_Voronoi_diagram()
{
	while (!Voronoi_diagram_que.empty())
		Voronoi_diagram_que.pop();

	for (int i = 1; i <= Nodes_Num; i++)
	{
		Voronoi_diagram_dis[i] = inf;
		Voronoi_diagram_path[i] = 0;
		Voronoi_diagram_belong[i] = 0;
		visted[i]=0;
	}
	for (int i = 1; i <= Nodes_Num; i++)
		if (!If_Steiner_Nodes[i])
		{
			Voronoi_diagram_que.push(i);
			Voronoi_diagram_dis[i] = 0;
			Voronoi_diagram_belong[i] = i;
			visted[i]=1;
		}
	while (!Voronoi_diagram_que.empty())
	{
		int u = Voronoi_diagram_que.front();
		Voronoi_diagram_que.pop();
		visted[u]=0;
		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i].v;
			int w=new_Edge_x_y[Edge[u][i].id].val;
			if(Voronoi_diagram_dis[v]>Voronoi_diagram_dis[u] + w)
			{
				Voronoi_diagram_path[v] = u;
				Voronoi_diagram_dis[v] = Voronoi_diagram_dis[u] + w;
				Voronoi_diagram_belong[v] = Voronoi_diagram_belong[u];
				if (!visted[v])
				{
					visted[v]=1;
					Voronoi_diagram_que.push(v);
				}
			}	
		}
	}
}

void Delete_path2()
{
	Inital_Voronoi_diagram();
	for (int i = 0; i < new_Edge_x_y.size(); i++)
	{
		int x = new_Edge_x_y[i].x; 
		int y = new_Edge_x_y[i].y;
		if (!important_edge[i]) continue;
		if (!important_node[x] || !important_node[y]) continue;

		if(!If_Steiner_Nodes[x]||!If_Steiner_Nodes[y])
			continue;
		
		if(Voronoi_diagram_belong[x]!=Voronoi_diagram_belong[y])
			continue;

		int wxy=new_Edge_x_y[i].val;
		if(wxy>=max(Voronoi_diagram_dis[x],Voronoi_diagram_dis[y]))///???
			important_edge[i]=0;
		
	}
}

int dis[Max_Nodes_Num+5];
queue<int>min_distances_que;
int mindistanceid[Max_Edges_Num+5];
int min_distances(int x,int y,int wxy,int id)
{
	if(mindistanceid[id]!=0)
		return mindistanceid[id];
	for(int i=1;i<=Nodes_Num;i++)
	{
		dis[i]=inf;
		visted[i]=0;
	}
	dis[x]=0;
	while(!min_distances_que.empty())
		min_distances_que.pop();
	min_distances_que.push(x);
	visted[x]=1;
	int step=0;
	while(!min_distances_que.empty())
	{
		int u=min_distances_que.front();
		min_distances_que.pop();
		visted[u]=0;
		for(int i=0;i<Edge[u].size();i++)
		{
			step++;
			int v=Edge[u][i].v;
			int w=new_Edge_x_y[Edge[u][i].id].val;
			if(dis[u]+w<dis[v])
			{
				dis[v]=dis[u]+w;
				if(!visted[v])
				{
					min_distances_que.push(v);
					visted[v]=1;
				}
			}
		}

		if(dis[y]<wxy||step>Edges_Num) 
		//if(dis[y]<wxy)
			break;
	}
	return mindistanceid[id]=dis[y];
}


void Delete_path3()
{
	for (int i = 0; i < new_Edge_x_y.size(); i++)
	{
		int x = new_Edge_x_y[i].x; 
		int y = new_Edge_x_y[i].y;
		if (!important_edge[i]) continue;
		if (!important_node[x] || !important_node[y]) continue;

		int wxy=new_Edge_x_y[i].val;
		if(wxy>min_distances(x,y,wxy,i))
		{
			//printf("%d ---- %d :%d   %d\n",x,y,wxy,min_distances(x,y,wxy));
			important_edge[i]=0;
		}
		
	}
}


void Huanyuan_graph()
{
	for (int i = 0; i <= origin_Nodes_Num; i++)
	{
		Edge[i].clear();
	}
	Edge_x_y.clear();

	int x, y, edgenum = 0;

	for (int i = 0; i < new_Edge_x_y.size(); i++)
	{
		x = new_Edge_x_y[i].x; y = new_Edge_x_y[i].y;
		if(new_Edge_x_y[i].val>1) continue;
		if (!important_node[x] || !important_node[y]) continue;

		int ef=find_edge_father(i);
		//cout<<i<<": "<<important_edge[i]<<"   "<<ef<<": "<<important_edge[ef]<<endl;
		if (!important_edge[i]&&!important_edge[ef]) continue;

		//cout<<i<<": "<<important_edge[i]<<"   "<<ef<<": "<<important_edge[ef]<<endl;
		Edge_x_y.push_back(new_Edge_x_y[i]);
		Node_Edge node;
		node.id=i;
		node.v=y;
		Edge[x].push_back(node);
		node.v=x;
		Edge[y].push_back(node);
		edgenum++;
	}
	Edges_Num = edgenum;

}


void Reduction(int &presolved_nodes, int &presolved_edges,int &presolved_terminals,int &cutnodes)///Nodes_Num,Edge[],Terminal nodes,Ndges_Num
{
	for (int i = 1; i <= Nodes_Num; i++)
	{
		important_node[i] = 1;
		if(!If_Steiner_Nodes[i])
			important_terminal[i]=1;
		else important_terminal[i]=0;
	}
	for(int i=0;i<=Max_Edges_Num;i++)
		important_edge[i]=1;
	new_Edge_x_y.clear();
	for(int i=0;i<origin_Edges_Num;i++)
	{
		new_Edge_x_y.push_back(origin_Edge_x_y[i]);
	}

	Update_Instances();
	
	Joint_neibor_Terminals();//ok

	Update_Instances();
	
	Cut_Edges_with_2_Steiners_as_Endpoint();//ok

	Update_Instances();

	Inital_Graph_Tarjan();//ok
	
	Update_Instances();
	//////////////////////////////

	for(int i=0;i<=Max_Edges_Num;i++)
	{
		edge_father[i]=i;
		mindistanceid[i]=0;
	}

	int times=10;
	int edgenumpre=Edges_Num;
	while(times--)
	{
		Joint_path();
/*
		cout<<new_Edge_x_y.size()<<endl;
		for(int i=0;i<new_Edge_x_y.size();i++)
		if(important_edge[i])
		{
			printf("**%d  %d   %d  flag=%d**\n",new_Edge_x_y[i].x,new_Edge_x_y[i].y,new_Edge_x_y[i].val,important_edge[i]);
		}
*/
		Update_Instances();
		Delete_path2();
		Update_Instances();
		
		Joint_path();
		Update_Instances();
		Delete_path3();
		Update_Instances();

		if(edgenumpre==Edges_Num)
			break;
		else edgenumpre=Edges_Num;
	}
	Huanyuan_graph();

	

	////////////////////////////////////////////////////////////////////////////////////////
	//for (int i = 1; i <= Nodes_Num; i++)
	//	if (important_node[i])
	//		printf("****%d\n", i);

	Reduction_count();
	
	int numcut = 0;
	for(int i=1;i<=Nodes_Num;i++)
	if (important_node[i]&&If_Steiner_Nodes[i])
	{
		if (is_cut[i]){
			numcut++;
		}
	}
	//printf("*cutcutcutcut:%d\n", numcut);
	cutnodes = numcut;

	presolved_nodes = 0, presolved_edges = 0, presolved_terminals = Terminals_Nodes_Num;
//	printf("\n(%d %d)\n", presolved_nodes, presolved_edges);
	for (int i = 1; i <= Nodes_Num; i++)
	{
		if(Edge[i].size()>0)
		{
			/*
			for(int j=0;j<Edge[i].size();j++)
				printf("%d-----%d   edge_id=%d     weight=%d\n",i,Edge[i][j].v,Edge[i][j].id,new_Edge_x_y[Edge[i][j].id].val);
			*/
			presolved_edges+=Edge[i].size();
			presolved_nodes++;
		}
		if (!If_Steiner_Nodes[i] && !important_terminal[i])
		{
			presolved_nodes--;
			presolved_terminals--;
		}
	}
	//printf("\n(%d %d)\n", presolved_nodes, presolved_edges); 
	presolved_edges /= 2;

}
