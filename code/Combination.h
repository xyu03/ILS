
bool Add_Cur_Solution_to_Pool()
{
	Node_Solution cur_solution;
	cur_solution.Sum_of_Nodes=Cur_Solution_Sum_of_Nodes;
	cur_solution.Nodes.clear();
	for(int u=Root;u!=0;u=Cur_Solution_Chain[u].nxt)
	{
		cur_solution.Nodes.push_back(u);
	}
	if(Pool.size()<Pool_Size)
	{
		Pool.push_back(cur_solution);
		return true;
	}

	for(int i=1;i<=Nodes_Num;i++)
	{
		appearing_times[i]=0;
	}
	for(int u=Root;u!=0;u=Cur_Solution_Chain[u].nxt)
	{
		appearing_times[u]=1;
	}

	double maxval=0;
	int id=-1;
	for(int i=0;i<Pool_Size;i++)
	{
		if(Pool[i].Sum_of_Nodes>=cur_solution.Sum_of_Nodes)
		{
			int jiao=0,bing=0;
			for(int j=0;j<Pool[i].Nodes.size();j++){
				appearing_times[Pool[i].Nodes[j]]++;
			}
			for(int j=1;j<=Nodes_Num;j++)
			{
				if(appearing_times[j]==2)
					jiao++;
				if(appearing_times[j]!=0)
					bing++;
			}
			for(int j=0;j<Pool[i].Nodes.size();j++){
				appearing_times[Pool[i].Nodes[j]]--;
			}

			double kval=1.0*jiao/bing;
			kval=(1.0-kval)*(1.0-kval);
			kval=1.0/kval;
			if(kval>maxval)
			{
				maxval=kval;
				id=i;
			}
		}
	}

	if(id!=-1)
	{
		Pool[id].Sum_of_Nodes=cur_solution.Sum_of_Nodes;
		Pool[id].Nodes.clear();
		for(int i=0;i<cur_solution.Sum_of_Nodes;i++)
		{
			Pool[id].Nodes.push_back(cur_solution.Nodes[i]);
		}
		return true;
	}
	return false;
}

void Combining_Solution()
{
	for(int i=1;i<=Nodes_Num;i++)
	{
		appearing_times[i]=0;
		Node_weight[i]=1;
	}
	for(int u=Root;u!=0;u=Cur_Solution_Chain[u].nxt){
		appearing_times[u]++;
	}
	int id=rand()%Pool.size();
	for(int i=0;i<Pool[id].Sum_of_Nodes;i++){
		appearing_times[Pool[id].Nodes[i]]++;
	}

	for(int i=1;i<=Nodes_Num;i++)
	{
		if(appearing_times[i]==0){
			Node_weight[i]=100;
		}
		if(appearing_times[i]==1){
			Node_weight[i]=rand()%40+10;
		}
	}

	Generate_Inital_Solution_Combination();
	
}