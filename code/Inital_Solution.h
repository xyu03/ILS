
queue<int>Inital_Solution_que;
int Inital_Solution_que_book[Max_Nodes_Num + 5];
int Inital_Solution_dis[Max_Nodes_Num + 5];
int Inital_Solution_path[Max_Nodes_Num + 5];
int permu_edge_weight[Max_Edges_Num+5];
void Inital_Solution_BFS_SPFA()
{
	//cout << "dfs " << u << endl;

	while (!Inital_Solution_que.empty())
	{
		int u = Inital_Solution_que.front();
		Inital_Solution_que.pop();
		Inital_Solution_que_book[u] = 0;

		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i].v;
			int w=permu_edge_weight[Edge[u][i].id];
			//cout << v << endl;
			if (Inital_Solution_dis[v] > Inital_Solution_dis[u] + w)
			{
				//	cout << v << endl;
				Inital_Solution_dis[v] = Inital_Solution_dis[u] + w;
				Inital_Solution_path[v] = u;
				if (!Inital_Solution_que_book[v])
				{
					Inital_Solution_que_book[v] = 1;
					Inital_Solution_que.push(v);
				}
			}
		}
	}
}



void Inital_Solution_BFS(int solved_Nodes_Num)
{
	for(int i=0;i<new_Edge_x_y.size();i++)
	{
		permu_edge_weight[i]=rand()%100+200;
	}
	for (int i = 1; i <= Nodes_Num; i++) {
		Inital_Solution_dis[i] = inf;
		Inital_Solution_path[i] = 0;
		Inital_Solution_que_book[i] = 0;
	}
	while (!Inital_Solution_que.empty())
		Inital_Solution_que.pop();
	Inital_Solution_que.push(Root);
	Inital_Solution_dis[Root] = 0;
	Inital_Solution_que_book[Root] = 1;

	int solved_Selected_Num = 1;
	while (solved_Selected_Num < solved_Nodes_Num)
	{
		//cout << Terminals_Selected_Num << endl;
		Inital_Solution_BFS_SPFA();

		int min_node = 0, min_dis = inf;
		for (int i = 0; i < solved_Nodes_Num; i++)
		{
			int u = Terminals_Nodes[i];
			if (!If_Selected[u]&& Inital_Solution_dis[u]< min_dis)
			{
				min_node = u;
				min_dis = Inital_Solution_dis[u];
			}
		}

		int u = min_node;
		while (!If_Selected[u])
		{
			If_Selected[u] = true;
			Cur_Solution_Sum_of_Nodes++;
			Cur_Solution_Chain[u].pre = preNode;
			Cur_Solution_Chain[u].nxt = Cur_Solution_Chain[preNode].nxt;
			Cur_Solution_Chain[preNode].nxt = u;
			preNode = u;

			Inital_Solution_dis[u] = 0;
			Inital_Solution_que.push(u);

			u = Inital_Solution_path[u];
		}

		solved_Selected_Num++;
	}
}

void Generate_Inital_Solution()//// If_Selected,Cur_Solution_Sum_of_Nodes,Cur_Solution_Chain
{
	for (int i = 1; i <= Nodes_Num; i++) {
		If_Selected[i] = false;
		Inital_Solution_dis[i] = inf;
	}
	for(int i=Terminals_Nodes_Num-1;i>0;i--)
		swap(Terminals_Nodes[i],Terminals_Nodes[rand()%i]);

	Root = Terminals_Nodes[rand() % Terminals_Nodes_Num];
	//cout << Root << endl;
	Cur_Solution_Chain[Root].nxt = Cur_Solution_Chain[Root].pre = 0;
	preNode = Root;
	If_Selected[Root] = true;
	Cur_Solution_Sum_of_Nodes = 1;

	Inital_Solution_BFS(Terminals_Nodes_Num);
}



void Inital_Solution_BFS_SPFA_Combination()
{
	//cout << "dfs " << u << endl;

	while (!Inital_Solution_que.empty())
	{
		int u = Inital_Solution_que.front();
		Inital_Solution_que.pop();
		Inital_Solution_que_book[u] = 0;

		for (int i = 0; i < Edge[u].size(); i++)
		{
			int v = Edge[u][i].v;
			//cout << v << endl;
			if (Inital_Solution_dis[v] > Inital_Solution_dis[u] + Node_weight[u]+Node_weight[v])
			{
				//	cout << v << endl;
				Inital_Solution_dis[v] = Inital_Solution_dis[u] + Node_weight[u]+Node_weight[v];
				Inital_Solution_path[v] = u;
				if (!Inital_Solution_que_book[v])
				{
					Inital_Solution_que_book[v] = 1;
					Inital_Solution_que.push(v);
				}
			}
		}
	}
}

void Inital_Solution_BFS_Combination(int solved_Nodes_Num)
{

	for (int i = 1; i <= Nodes_Num; i++) {
		Inital_Solution_dis[i] = inf;
		Inital_Solution_path[i] = 0;
		Inital_Solution_que_book[i] = 0;
	}
	while (!Inital_Solution_que.empty())
		Inital_Solution_que.pop();
	Inital_Solution_que.push(Root);
	Inital_Solution_dis[Root] = 0;
	Inital_Solution_que_book[Root] = 1;

	int solved_Selected_Num = 1;
	while (solved_Selected_Num < solved_Nodes_Num)
	{
		//cout << Terminals_Selected_Num << endl;
		Inital_Solution_BFS_SPFA_Combination();

		int min_node = 0, min_dis = inf;
		for (int i = 0; i < solved_Nodes_Num; i++)
		{
			int u = Terminals_Nodes[i];
			if (!If_Selected[u]&& Inital_Solution_dis[u]< min_dis)
			{
				min_node = u;
				min_dis = Inital_Solution_dis[u];
			}
		}

		int u = min_node;
		while (!If_Selected[u])
		{
			If_Selected[u] = true;
			Cur_Solution_Sum_of_Nodes++;
			Cur_Solution_Chain[u].pre = preNode;
			Cur_Solution_Chain[u].nxt = Cur_Solution_Chain[preNode].nxt;
			Cur_Solution_Chain[preNode].nxt = u;
			preNode = u;

			Inital_Solution_dis[u] = 0;
			Inital_Solution_que.push(u);

			u = Inital_Solution_path[u];
		}

		solved_Selected_Num++;
	}
}

void Generate_Inital_Solution_Combination()//// If_Selected,Cur_Solution_Sum_of_Nodes,Cur_Solution_Chain
{
	for (int i = 1; i <= Nodes_Num; i++) {
		If_Selected[i] = false;
		Inital_Solution_dis[i] = inf;
	}
	for(int i=Terminals_Nodes_Num-1;i>0;i--)
		swap(Terminals_Nodes[i],Terminals_Nodes[rand()%i]);

	Root = Terminals_Nodes[rand() % Terminals_Nodes_Num];
	//cout << Root << endl;
	Cur_Solution_Chain[Root].nxt = Cur_Solution_Chain[Root].pre = 0;
	preNode = Root;
	If_Selected[Root] = true;
	Cur_Solution_Sum_of_Nodes = 1;

	Inital_Solution_BFS_Combination(Terminals_Nodes_Num);
}