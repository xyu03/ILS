

/*-------------------------Key-Vertex elimination----------------------------*/

int Ask_Lca_Tree_with_Keyvertex_for_delete(int x, int y, int &fx, int &fy)
{
	if (Tree_with_Keyvertex_deep[x] < Tree_with_Keyvertex_deep[y])
		swap(x, y);

	for (int i = 20; i >= 0; i--)
	{
		int xx = Tree_with_Keyvertex_father[x][i].v;
		if (Tree_with_Keyvertex_deep[xx] >= Tree_with_Keyvertex_deep[y])
		{
			x = xx;
		}
	}
	if (x == y)
		return -1;

	//x=Tree_with_Keyvertex_father[x][0].v;
	for (int i = 20; i >= 0; i--)
	{
		int xx = Tree_with_Keyvertex_father[x][i].v;
		int yy = Tree_with_Keyvertex_father[y][i].v;
		if (xx!=yy)
		{
			x = xx;
			y = yy;
		}
	}

	fx=x;
	fy=y;
	
	return Tree_with_Keyvertex_father[x][0].v;
}

int Ask_Lca_Tree_with_Keyvertex_for_delete_for_Hv(int x, int y, int &fx, int &fy)
{
	if (Now_Tree_deep[x] < Now_Tree_deep[y])
		swap(x, y);

	int bx=Belong_keyvertex[x],by=Belong_keyvertex[y];

	for (int i = 20; i >= 0; i--)
	{
		int xx = Now_Tree_father[x][i];
		if (Now_Tree_deep[xx] >= Now_Tree_deep[y])
		{
			x = xx;
		}
	}

	if (x == y)
	{
		if(by==y) return y;
		
		for (int i = 20; i >= 0; i--)
		{
			int xx = Tree_with_Keyvertex_father[bx][i].v;
			if (Tree_with_Keyvertex_deep[xx] >= Tree_with_Keyvertex_deep[by]&&xx!=by)
			{
				bx = xx;
			}
		}
		return bx;
	}

	//printf("?????  %d   %d\n",Now_Tree_deep[x],Now_Tree_deep[y]);
		
	//x=Now_Tree_father[x][0];
	

	for (int i = 20; i >= 0; i--)
	{
		int xx = Now_Tree_father[x][i];
		int yy = Now_Tree_father[y][i];
		if (xx!=yy)
		{
			x = xx;
			y = yy;
		}
	}
	int nf=Belong_keyvertex[Now_Tree_father[x][0]];
	if(nf!=Now_Tree_father[x][0])
	{
		cout<<Now_Tree_father[x][0]<<"    "<<Now_Tree_father[y][0]<<endl;
		cout<<Now_Tree[Now_Tree_father[x][0]].size()<<endl;
		cout<<If_Keyvertex[Now_Tree_father[x][0]]<<endl;
		printf("*****%d  %d******\n",nf,Now_Tree_father[x][0]);
		//printf("*****%d  %d******\n",nf,Now_Tree_father[x][0]);
	}
	
	return nf;
}

queue<Node_HLv>Rv;


struct Node_Repair{
	int u,dis;
	bool operator <(const Node_Repair &a)const{
		return dis>a.dis;
	}
};

priority_queue<Node_Repair>Inital_Voronoi_que_Repair;
vector<int>Voronoi_Repair_Vertex;
void Repair_Voronoi_region_solve_vertex(int node,bool flag)
{
	If_Selected[node]=flag;
}

void Repair_Voronoi_region_add_vertex_to_que(int v)
{
	if(If_Selected[Voronoi_belong[v]])
	{
		Node_Repair nr;
		nr.u=v;
		nr.dis=Voronoi_dis[v];
		Inital_Voronoi_que_Repair.push(nr);
	}
	
}

void Repair_Voronoi_region_solve1(int keyv,bool flag)
{
	Repair_Voronoi_region_solve_vertex(keyv,flag);

	int node = Cur_Father[keyv];
	while (!If_Keyvertex[node])
	{
		Repair_Voronoi_region_solve_vertex(node,flag);
		node = Cur_Father[node];
	}

	for(int i=0;i<Tree_with_Keyvertex[keyv].size();i++)
	{
		int node =Tree_with_Keyvertex[keyv][i].v;
		node = Cur_Father[node];
		while (!If_Keyvertex[node])
		{
			Repair_Voronoi_region_solve_vertex(node,flag);
			node = Cur_Father[node];
		}
	}
}

queue<int>Repair_Voronoi_region_solve2_que;
int Repair_Voronoi_region_solve2_que_book[Max_Nodes_Num+5];

void Repair_Voronoi_region_solve2(int keyv)
{
	while (!Inital_Voronoi_que_Repair.empty())
		Inital_Voronoi_que_Repair.pop();


	while(!Repair_Voronoi_region_solve2_que.empty())
		Repair_Voronoi_region_solve2_que.pop();

	Repair_Voronoi_region_solve2_que.push(keyv);
	Repair_Voronoi_region_solve2_que_book[keyv]=1;

	while(!Repair_Voronoi_region_solve2_que.empty())
	{
		int u=Repair_Voronoi_region_solve2_que.front();
		Repair_Voronoi_region_solve2_que.pop();
		for(int i=0;i<Edge[u].size();i++)
		{
			int v=Edge[u][i].v;
			if(!Repair_Voronoi_region_solve2_que_book[v]&&!If_Selected[Voronoi_belong[v]])
			{
				Repair_Voronoi_region_solve2_que.push(v);
				Repair_Voronoi_region_solve2_que_book[v]=1;
			}
			else
			{
				if(!Repair_Voronoi_region_solve2_que_book[v])
				{
					Repair_Voronoi_region_solve2_que_book[v]=1;
					Repair_Voronoi_region_add_vertex_to_que(v);
				}
			}
		}
	}
}

void Repair_Voronoi_region(int keyv)
{
	Voronoi_Repair_Vertex.clear();
	
	Repair_Voronoi_region_solve1(keyv,false);


	Repair_Voronoi_region_solve2(keyv);

	while (!Inital_Voronoi_que_Repair.empty())
	{
		Node_Repair nr= Inital_Voronoi_que_Repair.top();
		Inital_Voronoi_que_Repair.pop();
		if(nr.dis>Voronoi_dis[nr.u])
			continue;
		int u=nr.u;
		Voronoi_Repair_Vertex.push_back(u);
		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i].v;
			if (Voronoi_dis[v]>Voronoi_dis[u]+1||!If_Selected[Voronoi_belong[v]])
			{
				Voronoi_belong[v] = Voronoi_belong[u];
				Voronoi_path[v] = u;
				Voronoi_dis[v] = Voronoi_dis[u] + 1;
				Node_Repair nrk;
				nrk.dis=Voronoi_dis[v];
				nrk.u=v;
				Inital_Voronoi_que_Repair.push(nrk);
			}
		}
	}


	for(int i=0;i<Voronoi_Repair_Vertex.size();i++)
	{
		int v=Voronoi_Repair_Vertex[i];
		Repair_Voronoi_region_solve2_que_book[v]=0;
	}

	Repair_Voronoi_region_solve1(keyv,true);
	
}
void Init_Rv(int u)
{
	while(!Rv.empty())
		Rv.pop();
	Repair_Voronoi_region(u);

	for(int i=0;i<Voronoi_Repair_Vertex.size();i++)
	{
		int v1=Voronoi_Repair_Vertex[i];
		for(int j=0;j<Edge[v1].size();j++)
		{
			int v2=Edge[v1][j].v;
			
			int vv1 = Voronoi_belong[v1];
			int vv2 = Voronoi_belong[v2];

			if (Belong_keyvertex[vv1] == Belong_keyvertex[vv2]|| Belong_keyvertex[vv1]==0||Belong_keyvertex[vv2]==0)
				continue;
			int x = Belong_keyvertex[vv1];
			int y = Belong_keyvertex[vv2];
			int fx=0,fy=0,fxy=0;

			fxy=Ask_Lca_Tree_with_Keyvertex_for_delete(x, y, fx, fy);

			if(fxy==u)
			{
				Node_HLv nodelv;
				nodelv.x=fx; nodelv.y=fy; 
				nodelv.dis=Voronoi_dis[v1] + Voronoi_dis[v2]+1;
				nodelv.v1=v1; nodelv.v2=v2;
				Rv.push(nodelv);
			}
			else if(Tree_with_Keyvertex_deep[fxy]>Tree_with_Keyvertex_deep[u])
			{
				int fxu=Ask_Lca_Tree_with_Keyvertex_for_delete(x,u,fx,fy);
				int fyu=Ask_Lca_Tree_with_Keyvertex_for_delete(y,u,fx,fy);
				if(fxu==u&&fyu!=u)
				{
					Node_HLv nodelv;
					nodelv.x=fx; nodelv.y=Root; 
					nodelv.dis=Voronoi_dis[v1] + Voronoi_dis[v2]+1;
					nodelv.v1=v1; nodelv.v2=v2;
					Rv.push(nodelv);
				}
				else if(fyu==u&&fxu!=u)
				{
					Node_HLv nodelv;
					nodelv.x=fy; nodelv.y=Root; 
					nodelv.dis=Voronoi_dis[v1] + Voronoi_dis[v2]+1;
					nodelv.v1=v1; nodelv.v2=v2;
					Rv.push(nodelv);
				}

			}
		}
	}
	for(int i=0;i<Voronoi_Repair_Vertex.size();i++)
	{
		int u=Voronoi_Repair_Vertex[i];
		Voronoi_dis[u]=Voronoi_dis_copy[u];
		Voronoi_path[u]=Voronoi_path_copy[u];
		Voronoi_belong[u]=Voronoi_belong_copy[u];
	}
}


void Init_Lv()
{
	for(int i=1;i<=Nodes_Num;i++)
	{
		L[i]=NULL;
	}
	
	for (int i = 0; i < Edges_Num; i++)
	{
		int v1 = Edge_x_y[i].x;
		int v2 = Edge_x_y[i].y;
		
		int vv1 = Voronoi_belong[v1];
		int vv2 = Voronoi_belong[v2];
		
		if (Belong_keyvertex[vv1] == Belong_keyvertex[vv2]|| Belong_keyvertex[vv1]==0||Belong_keyvertex[vv2]==0)
			continue;
		int x = Belong_keyvertex[vv1];
		int y = Belong_keyvertex[vv2];
		int fx=0,fy=0,fxy=0;

		fxy=Ask_Lca_Tree_with_Keyvertex_for_delete(x, y, fx, fy);
		if(fxy==-1) continue;

		Node_HLv nodelv;
		nodelv.x=fx; nodelv.y=fy; 
		nodelv.dis=Voronoi_dis[v1] + Voronoi_dis[v2]+1;
		nodelv.v1=v1; nodelv.v2=v2;
		L[fxy]=insert(nodelv,L[fxy]);
	}
}



void Init_Hv()
{
	for(int i=1;i<=Nodes_Num;i++)
	{
		H[i]=NULL;
	}
	for (int i = 0; i < Edges_Num; i++)
	{
		int v1 = Edge_x_y[i].x;
		int v2 = Edge_x_y[i].y;
		
		int vv1 = Voronoi_belong[v1];
		int vv2 = Voronoi_belong[v2];
		
		if (Belong_keyvertex[vv1] == Belong_keyvertex[vv2]|| Belong_keyvertex[vv1]==0||Belong_keyvertex[vv2]==0)
			continue;
		int x = Belong_keyvertex[vv1];
		int y = Belong_keyvertex[vv2];
		int fx=0,fy=0,fxy=0;

		fxy=Ask_Lca_Tree_with_Keyvertex_for_delete_for_Hv(vv1, vv2, fx, fy);
		//fxy=Ask_Lca_Tree_with_Keyvertex_for_delete(x, y, fx, fy);
		if(fxy==-1) continue;

		Node_HLv nodehv;
		nodehv.fx=fxy; 
		nodehv.dis=Voronoi_dis[v1] + Voronoi_dis[v2]+1;
		nodehv.v1=v1; nodehv.v2=v2;
		H[x]=insert(nodehv,H[x]);
		H[y]=insert(nodehv,H[y]);
	}
}

struct Node_Edge_Keyvertex_for_delete{
	int x,y;
	int val;
	int v1,v2;
	int flag;///1:H and L  2:R
};

struct Node_candidate_keyvertices{
	int keyvertex;
	vector<Node_Edge_Keyvertex_for_delete>node; 
};
vector<Node_candidate_keyvertices>Candidate_Keyvertices_to_delete;


vector<Node_Edge_Keyvertex_for_delete>Tree_with_Keyvertex_dfs_for_delete_edges;

bool cmp_for_delete(const Node_Edge_Keyvertex_for_delete &a,const Node_Edge_Keyvertex_for_delete&b)
{
	return a.val<b.val;
}
int MST_father[Max_Nodes_Num+5];
int MST_father_find(int x)
{
	return x==MST_father[x]?x:MST_father[x]=MST_father_find(MST_father[x]);
}
void MST_check_for_for_delete_edges(int keyv,int num,int sumdis)
{
	MST_father[Root]=Root;
	for(int i=0;i<Tree_with_Keyvertex[keyv].size();i++)
	{
		MST_father[Tree_with_Keyvertex[keyv][i].v]=Tree_with_Keyvertex[keyv][i].v;
	}

	Node_candidate_keyvertices kkkk;
	kkkk.keyvertex=keyv;
	int ansdis=0;
	sort(Tree_with_Keyvertex_dfs_for_delete_edges.begin(),Tree_with_Keyvertex_dfs_for_delete_edges.end(),cmp_for_delete);
	for(int i=0;i<Tree_with_Keyvertex_dfs_for_delete_edges.size();i++)
	{
		int x=Tree_with_Keyvertex_dfs_for_delete_edges[i].x;
		int y=Tree_with_Keyvertex_dfs_for_delete_edges[i].y;
		
		int fx=MST_father_find(x);
		int fy=MST_father_find(y);
		if(fx!=fy)
		{
			num--;
			MST_father[fx]=fy;
			ansdis+=Tree_with_Keyvertex_dfs_for_delete_edges[i].val;
			kkkk.node.push_back(Tree_with_Keyvertex_dfs_for_delete_edges[i]);
			if(ansdis>=sumdis) break;
		}
	}

	if(ansdis<sumdis&&num==1)
	{
		Candidate_Keyvertices_to_delete.push_back(kkkk);
	}
}
void Tree_with_Keyvertex_dfs_for_delete(int u)
{
	//cout << u << endl;
	int sumdis=Tree_with_Keyvertex_father[u][0].dis;
	int sz=Tree_with_Keyvertex[u].size();
	for (int i = 0; i < sz; i++)
	{
		sumdis+=Tree_with_Keyvertex[u][i].dis;
		int v = Tree_with_Keyvertex[u][i].v;
		Tree_with_Keyvertex_dfs_for_delete(v);
	}
	Tree_with_Keyvertex_dfs_for_delete_edges.clear();
	

	for (int i = 0; i < sz; i++)
	{
		int v = Tree_with_Keyvertex[u][i].v;

		Node_HLv nodehv;
		Node_HLv nodelv;

		if(If_Steiner_Nodes[u])
		{
			while(1)
			{
				nodehv=find_min(H[v]);
				if(nodehv.fx==-1) break;
				H[v]=delete_min(H[v]);

				int x=nodehv.fx;
				if(Tree_with_Keyvertex_deep[x]<Tree_with_Keyvertex_deep[u])
				{
					Node_Edge_Keyvertex_for_delete node;
					node.v1=nodehv.v1;
					node.v2=nodehv.v2;
					node.val=nodehv.dis;
					node.x=Root;
					node.y=v;
					node.flag=1;
					Tree_with_Keyvertex_dfs_for_delete_edges.push_back(node);
					

					if(Tree_with_Keyvertex_deep[x]<Tree_with_Keyvertex_deep[u]-1)
					{
						H[u]=insert(nodehv,H[u]);
					}
					break;
				}
			}
		}
		
		H[u]=merge(H[u],H[v]);

		if(If_Steiner_Nodes[u])
		{
			while(1)
			{
				nodelv=find_min(L[u]);
				if(nodelv.fx==-1) break;
				L[u]=delete_min(L[u]);

				Node_Edge_Keyvertex_for_delete node;
				node.v1=nodelv.v1;
				node.v2=nodelv.v2;
				node.val=nodelv.dis;
				node.x=nodelv.x;
				node.y=nodelv.y;
				node.flag=1;
				Tree_with_Keyvertex_dfs_for_delete_edges.push_back(node);
			}
		}

	}

	if(If_Steiner_Nodes[u])
	{
		Init_Rv(u);
		while(!Rv.empty())
		{
			Node_HLv nodelv;
			nodelv=Rv.front();
			Rv.pop();
			Node_Edge_Keyvertex_for_delete node;
			node.v1=nodelv.v1;
			node.v2=nodelv.v2;
			node.val=nodelv.dis;
			node.x=nodelv.x;
			node.y=nodelv.y;
			node.flag=2;
			Tree_with_Keyvertex_dfs_for_delete_edges.push_back(node);
		}
		MST_check_for_for_delete_edges(u,sz+1,sumdis);
	}

}


void Ask_for_Cadidate_KeyVertex_to_Elimination()
{

	Candidate_Keyvertices_to_delete.clear();
	Inital_Voronoi_region();

	Inital_Tree_with_Keyvertex();

	Init_Lv();

	Init_Hv();
	
	for(int i=1;i<=Nodes_Num;i++)
	{
		Repair_Voronoi_region_solve2_que_book[i]=0;
		MST_father[i]=i;
	}

	Tree_with_Keyvertex_dfs_for_delete(Root);
}


void Delete_KeyVertex(int index)
{
	int keyv= Candidate_Keyvertices_to_delete[index].keyvertex;

	If_Selected[keyv] = false;
	Cur_Solution_Sum_of_Nodes--;
	Cur_Solution_Chain[Cur_Solution_Chain[keyv].pre].nxt = Cur_Solution_Chain[keyv].nxt;
	Cur_Solution_Chain[Cur_Solution_Chain[keyv].nxt].pre = Cur_Solution_Chain[keyv].pre;


	int node =keyv;
	node = Cur_Father[node];
	while (!If_Keyvertex[node])
	{
		If_Selected[node] = false;
		
		Cur_Solution_Sum_of_Nodes--;
		Cur_Solution_Chain[Cur_Solution_Chain[node].pre].nxt = Cur_Solution_Chain[node].nxt;
		Cur_Solution_Chain[Cur_Solution_Chain[node].nxt].pre = Cur_Solution_Chain[node].pre;
		
		node = Cur_Father[node];
	}

	for(int i=0;i<Tree_with_Keyvertex[keyv].size();i++)
	{
		int node =Tree_with_Keyvertex[keyv][i].v;
		node = Cur_Father[node];
		while (!If_Keyvertex[node])
		{
			If_Selected[node] = false;
			
			Cur_Solution_Sum_of_Nodes--;
			Cur_Solution_Chain[Cur_Solution_Chain[node].pre].nxt = Cur_Solution_Chain[node].nxt;
			Cur_Solution_Chain[Cur_Solution_Chain[node].nxt].pre = Cur_Solution_Chain[node].pre;
			
			node = Cur_Father[node];
		}
	}
	

	for(int i=0;i<Candidate_Keyvertices_to_delete[index].node.size();i++)
	if(Candidate_Keyvertices_to_delete[index].node[i].flag==1)
	{
		int v1=Candidate_Keyvertices_to_delete[index].node[i].v1;
		int v2=Candidate_Keyvertices_to_delete[index].node[i].v2;
		while (!If_Selected[v1])
		{
			If_Selected[v1] = true;
			
			Cur_Solution_Sum_of_Nodes++;
			Cur_Solution_Chain[v1].nxt = Cur_Solution_Chain[Root].nxt;
			Cur_Solution_Chain[v1].pre =Root;

			Cur_Solution_Chain[Root].nxt = v1;
			Cur_Solution_Chain[Cur_Solution_Chain[v1].nxt].pre = v1;
			
			v1 = Voronoi_path[v1];
		}

		while (!If_Selected[v2])
		{
			If_Selected[v2] = true;
			
			Cur_Solution_Sum_of_Nodes++;
			Cur_Solution_Chain[v2].nxt = Cur_Solution_Chain[Root].nxt;
			Cur_Solution_Chain[v2].pre = Root;

			Cur_Solution_Chain[Root].nxt = v2;
			Cur_Solution_Chain[Cur_Solution_Chain[v2].nxt].pre = v2;
			
			v2 = Voronoi_path[v2];
		}
	}


	Repair_Voronoi_region(keyv);
	Repair_Voronoi_region_solve1(keyv,false);
	
	for(int i=0;i<Candidate_Keyvertices_to_delete[index].node.size();i++)
	if(Candidate_Keyvertices_to_delete[index].node[i].flag==2)
	{
		int v1=Candidate_Keyvertices_to_delete[index].node[i].v1;
		int v2=Candidate_Keyvertices_to_delete[index].node[i].v2;
		while (!If_Selected[v1])
		{
			If_Selected[v1] = true;
			
			Cur_Solution_Sum_of_Nodes++;
			Cur_Solution_Chain[v1].nxt = Cur_Solution_Chain[Root].nxt;
			Cur_Solution_Chain[v1].pre =Root;

			Cur_Solution_Chain[Root].nxt = v1;
			Cur_Solution_Chain[Cur_Solution_Chain[v1].nxt].pre = v1;
			
			v1 = Voronoi_path[v1];
		}

		while (!If_Selected[v2])
		{
			If_Selected[v2] = true;
			
			Cur_Solution_Sum_of_Nodes++;
			Cur_Solution_Chain[v2].nxt = Cur_Solution_Chain[Root].nxt;
			Cur_Solution_Chain[v2].pre = Root;

			Cur_Solution_Chain[Root].nxt = v2;
			Cur_Solution_Chain[Cur_Solution_Chain[v2].nxt].pre = v2;
			
			v2 = Voronoi_path[v2];
		}
		
	}
	
}

