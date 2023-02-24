#pragma once

bool LS_Delete()
{
	Ask_for_Cadidate_Nodes_to_Delete_Tarjan();
	//cout << "debug" << endl;
	int siz = Candidate_Nodes_to_Delete.size();
	if(siz>0){

		int tmp_node = rand() % siz;
		Delete_Node_from_Cur_Tree(Candidate_Nodes_to_Delete[tmp_node]);///delete 1 node
		return true;
	}
	return false;
}
bool LS_Swap2()
{
	if(Bipartite_Graph||!Ask_Swap2) 
		return false;
	Ask_for_Cadidate_2_1_Nodes_to_Swap();///swap 2 and 1 ?
	int siz1 = Candidate_x_y_z_to_Swap[0].size();
	if (siz1 != 0)
	{
		int tmp_node = rand() % siz1;
		Swap_Nodes_2_1(Candidate_x_y_z_to_Swap[0][tmp_node], Candidate_x_y_z_to_Swap[1][tmp_node], Candidate_x_y_z_to_Swap[2][tmp_node]);
		return true;
	}
	return false;
}

bool LS_Swap1()
{
	Ask_for_Cadidate_Nodes_to_Swap();///swap 1 and 1 ?
	int siz2 = Candidate_x_y_to_Swap[0].size();
	//cout<<"fuck "<<siz2<<endl;
	if (siz2 != 0)
	{
		int tmp_node = rand() % siz2;
		Swap_Nodes(Candidate_x_y_to_Swap[0][tmp_node], Candidate_x_y_to_Swap[1][tmp_node]);
		return true;
	}
	return false;
}

bool LS_Swap_path()
{
	//if(Nodes_Num<10*Terminals_Nodes_Num) 
	//	return false;
	Ask_for_Cadidate_Keypath_to_Swap();
	int siz0 = Candidate_v1_of_Path_to_Swap.size();
	if (siz0 != 0)
	{
		int tmp_node = rand() % siz0;
		Swap_KeyPath(tmp_node);
		return true;
	}
	return false;
}

bool LS_Delete_Keyvertex()
{
	Ask_for_Cadidate_KeyVertex_to_Elimination();
	int siz3=Candidate_Keyvertices_to_delete.size();
	if(siz3!=0)
	{
		int tmp_node=rand()%siz3;
		Delete_KeyVertex(tmp_node);
		return true;
	}
	return false;
}

bool MST_Pure()
{
	Ask_for_Cadidate_Nodes_to_Pure();
	//cout << "debug" << endl;
	int siz = Candidate_Nodes_to_Delete.size();
	if(siz>0){
		for(int i=0;i<siz;i++)
			Delete_Node_from_Cur_Tree(Candidate_Nodes_to_Delete[i]);///delete 1 node
		Candidate_Nodes_to_Delete.clear();
		return true;
	}
	return false;
}

void Local_Search()
{

	int num = 0;
	int LSbest = Cur_Solution_Sum_of_Nodes;
	int knum=0;

	while (clock() - begin_time < 1.0*Max_Limited_Time*CLOCKS_PER_SEC)
	{
		//cout<<knum++<<endl;
		
		MST_Pure();
		if(!LS_Delete())
		if(!LS_Delete_Keyvertex())
		if(!LS_Swap_path())
		//if(!LS_Swap2())
		//if(!LS_Swap1())
			break;
		

		if (Cur_Solution_Sum_of_Nodes < LSbest)
		{
			LSbest = Cur_Solution_Sum_of_Nodes;
			num = 0;
		}
		else {
			num++;
			if (num > Max_not_Improve_Num) break;
		}
	}
}


int Solve_Instance(long long &sum, long long &runnum)
{
	Best_Solution.Sum_of_Nodes=inf;
	Generate_Inital_Solution();
	MST_Pure();

	printf("******%d*********\n", Cur_Solution_Sum_of_Nodes - 1);

	Store_Best_Solution();

	runnum = sum = 0;
	
	double bestanswer_time;

	int best_answer_num = 0;
	/*
	int iteration_times=8,iter;
	while (clock() - begin_time < 1.0*Max_Limited_Time*CLOCKS_PER_SEC)
	{
		Pool_Size=(int)sqrt(iteration_times/2);
		iter=iteration_times;
		while (iter>0&&clock() - begin_time < 1.0*Max_Limited_Time*CLOCKS_PER_SEC)
		{
			iter--;
			Generate_Inital_Solution();
			MST_Pure();

			Local_Search();
			sum += 1ll*Cur_Solution_Sum_of_Nodes - 1;
			runnum++;	

			if (Cur_Solution_Sum_of_Nodes < Best_Solution.Sum_of_Nodes)
			{
				best_answer_num = 1;
				Store_Best_Solution();
				bestanswer_time=(clock()-begin_time)/CLOCKS_PER_SEC;
				printf("%s  time:%.2f     LS %lld answer:%d     Best answer:%d\n",Input_File_Name, bestanswer_time,runnum, Cur_Solution_Sum_of_Nodes - 1, Best_Solution.Sum_of_Nodes - 1);
			}
			else
				if (Cur_Solution_Sum_of_Nodes == Best_Solution.Sum_of_Nodes)
					best_answer_num++;
			
			Add_Cur_Solution_to_Pool();

			if(Pool.size()==Pool_Size)
			{
				int tt=Pool_Size/2;
				while(tt--)
				{
				

					Combining_Solution();
					MST_Pure();

					Local_Search();

					sum += 1ll*Cur_Solution_Sum_of_Nodes - 1;
					runnum++;	

					if (Cur_Solution_Sum_of_Nodes < Best_Solution.Sum_of_Nodes)
					{
						best_answer_num = 1;
						Store_Best_Solution();
						bestanswer_time=(clock()-begin_time)/CLOCKS_PER_SEC;
						printf("%s  time:%.2f     LS %lld answer:%d     Best answer:%d\n",Input_File_Name, bestanswer_time,runnum, Cur_Solution_Sum_of_Nodes - 1, Best_Solution.Sum_of_Nodes - 1);
					}
					else
						if (Cur_Solution_Sum_of_Nodes == Best_Solution.Sum_of_Nodes)
							best_answer_num++;


					Add_Cur_Solution_to_Pool();
					
				}
			}
			iteration_times*=2;
		}

	}
	*/


	int iteration_times=64,iter;
	while (clock() - begin_time < 1.0*Max_Limited_Time*CLOCKS_PER_SEC)
	{
		Pool_Size=min((int)sqrt(iteration_times),64);
		iter=iteration_times;
		cout<<"Pool size:"<<Pool_Size<<endl;
		while (iter>0&&clock() - begin_time < 1.0*Max_Limited_Time*CLOCKS_PER_SEC)
		{
			iter--;
			Generate_Inital_Solution();

			if(Check_Cur_Solution()!=1)
			{
				printf("Inital solution error!\n\n");
			}
			MST_Pure();

			Local_Search();

			int prebest=Best_Solution.Sum_of_Nodes;
			Store_Best_Solution();

			sum += 1ll*Cur_Solution_Sum_of_Nodes;
			runnum++;

			if (Cur_Solution_Sum_of_Nodes < prebest)
			{
				best_answer_num = 1;
				bestanswer_time=(clock()-begin_time)/CLOCKS_PER_SEC;
				printf("%s  time:%.2f     LS %lld answer:%d     Best answer:%d\n",Input_File_Name, bestanswer_time,runnum, Cur_Solution_Sum_of_Nodes-1, Best_Solution.Sum_of_Nodes-1);
			}
			else
				if (Cur_Solution_Sum_of_Nodes == Best_Solution.Sum_of_Nodes)
					best_answer_num++;
			
			Add_Cur_Solution_to_Pool();

			
			if(Pool.size()==Pool_Size)
			{
				//int tt=min(7,Pool_Size);
				int tt=Pool_Size/4;
				while(tt--)
				{
					
					//Pre_Solution_Sum_of_Nodes=Cur_Solution_Sum_of_Nodes;
					//for(int u=Root;u!=0;u=Cur_Solution_Chain[u].nxt)
					//{
					//	Pre_Solution_Chain[u].pre=Cur_Solution_Chain[u].pre;
					//	Pre_Solution_Chain[u].nxt=Cur_Solution_Chain[u].nxt;
					//}
					

					Combining_Solution();
					if(Check_Cur_Solution()!=1)
					{
						printf("Combination solution error!\n\n");
					}
					MST_Pure();

					Local_Search();
					
					prebest=Best_Solution.Sum_of_Nodes;
					Store_Best_Solution();

					sum += 1ll*Cur_Solution_Sum_of_Nodes;
					runnum++;

					if (Cur_Solution_Sum_of_Nodes < prebest)
					{
						best_answer_num = 1;
						bestanswer_time=(clock()-begin_time)/CLOCKS_PER_SEC;
						printf("%s  time:%.2f     LS %lld answer:%d     Best answer:%d\n",Input_File_Name, bestanswer_time,runnum, Cur_Solution_Sum_of_Nodes-1, Best_Solution.Sum_of_Nodes-1);
					}
					else
						if (Cur_Solution_Sum_of_Nodes == Best_Solution.Sum_of_Nodes)
							best_answer_num++;


					Add_Cur_Solution_to_Pool();
					
					//if(Cur_Solution_Sum_of_Nodes<=Pre_Solution_Sum_of_Nodes)
					//{
					//	Add_Cur_Solution_to_Pool();
					//	if(Cur_Solution_Sum_of_Nodes<Pre_Solution_Sum_of_Nodes)
					//		break;
					//}

					//Cur_Solution_Sum_of_Nodes=Pre_Solution_Sum_of_Nodes;
					//for(int u=Root;u!=0;u=Pre_Solution_Chain[u].nxt)
					//{
					//	Cur_Solution_Chain[u].pre=Pre_Solution_Chain[u].pre;
					//	Cur_Solution_Chain[u].nxt=Pre_Solution_Chain[u].nxt;
					//}
					
				}
				//Add_Cur_Solution_to_Pool();
			}
			
			
		}
		if(Pool_Size==64)
		{
			iteration_times=64;
			Pool.clear();
		}
		else 
		iteration_times*=2;
	}





	int flag = Check_Best_Solution();

	printf("flag=%d\n", flag);
	sort(Best_Solution.Nodes.begin(), Best_Solution.Nodes.end());
	printf("Nodes:");
	for (int i = 0; i < Best_Solution.Sum_of_Nodes; i++)
	{
		printf("%d ", Best_Solution.Nodes[i]);
	}
	printf("\n");

	printf("The best answer is  %d\n", Best_Solution.Sum_of_Nodes - 1);
	printf("The best answer time: %.2f\n",bestanswer_time);
	printf("LS num is  %lld\nThe Sum is  %lld\n", runnum, sum);
	printf("The average answer is  %.2f\n", 1.0*sum / runnum);
	printf("Time is %f\n", (clock() - begin_time) / CLOCKS_PER_SEC);

	FILE *fp;
	fp = fopen(Output_File_Name, "w+");

    fprintf(fp, "******flag=%d******\n\n\n", flag);

	fprintf(fp, "Original Nodes Num:%d   Original Edges Num:%d   Original Terminals Num:%d\n", origin_Nodes_Num, origin_Edges_Num, origin_Terminals_Nodes_Num);
	fprintf(fp, "Presolved Nodes Num:%d   Presolved Edges Num:%d   Presolved Terminals Num:%d\n", presolved_nodes, presolved_edges, presolved_terminals);
	fprintf(fp, "Cut Nodes Num:%d\n\n", cutnodes);


	fprintf(fp, "The best answer is  %d\n", Best_Solution.Sum_of_Nodes - 1);
	fprintf(fp, "The best answer time: %.2f\n",bestanswer_time);
	fprintf(fp, "LS num is  %lld\nThe Sum is  %lld\n", runnum, sum);
	fprintf(fp, "Best Answer Num is: %d\n", best_answer_num);
	fprintf(fp, "The average answer is  %.2f\n", 1.0*sum / runnum);
	fprintf(fp, "Time is %f\n", (clock() - begin_time) / CLOCKS_PER_SEC);

	fprintf(fp, "\n\nVertices %d\n",Best_Solution.Sum_of_Nodes);
	for (int i = 0; i < Best_Solution.Sum_of_Nodes; i++)
	{
		fprintf(fp, "V %d\n", Best_Solution.Nodes[i]);
	}
	fprintf(fp, "\n\n");

	sort(Outputedge.begin(), Outputedge.end(), Outputedgecmp);

	fprintf(fp, "Edges %d\n",Best_Solution.Sum_of_Nodes-1);
	for (int i = 0; i < Best_Solution.Sum_of_Nodes - 1; i++)
	{
		fprintf(fp, "E %d  %d\n", Outputedge[i].x, Outputedge[i].y);
	}

	fclose(fp);

	return flag;
}
