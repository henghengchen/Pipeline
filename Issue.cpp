#include "PipleLing.h"
#include "Issue.h"
#include <stdio.h>

//没有跳过指令的情况
//RAW-----已发射指令没有将该指令的源寄存器作为目的寄存器
//WAW-----功能单元有空位，并且已发射的指令没有与该指令使用同一个目的寄存器
//跳过指令的情况
//RAW-----已发射指令和前面的未发射都没有将该指令的源寄存器作为目的寄存器
//WAW-----功能单元有空位，并且已发射的指令和前面的未发射指令都没有与该指令使用同一个目的寄存器
//WAR-----功能单元有空位+前面的未发射指令的源寄存器与该指令的目的寄存器不同
void Issue::run(ScoreBoardingTable *scoreboardingtable,char registers[][32],PRE_Issue *Pre_Issue_front,PRE_Issue *Pre_Issue,PRE_MEN *Pre_Men,PRE_ALU *Pre_Alu,PRE_ALUB *Pre_Alub){
	//printf("Issue----run\n");
	int issued_instruction_number=0;
	int pre_issue_index=0;//取指令的下标
	//首先Pre_Alu还有空位，其次还未发射超过两条指令，最后Pre_Issue里面还有指令可以取
	while(issued_instruction_number<2&&Pre_Issue->rear!=0&&pre_issue_index<Pre_Issue->rear){//循环发射两条指令
		//printf("目的寄存器R%d:%d  原寄存器R%d:%d  原寄存器R%d:%d\n",Pre_Issue->instruction_content[pre_issue_index][0],scoreboardingtable->register_result_status[Pre_Issue->instruction_content[pre_issue_index][0]],
					//Pre_Issue->instruction_content[pre_issue_index][1],scoreboardingtable->register_result_status[Pre_Issue->instruction_content[pre_issue_index][1]],
					//Pre_Issue->instruction_content[pre_issue_index][2],scoreboardingtable->register_result_status[Pre_Issue->instruction_content[pre_issue_index][2]]);
		//printf("optype0=%d,optype1=%d,optype2=%d\n",Pre_Issue->op_type[pre_issue_index][0],Pre_Issue->op_type[pre_issue_index][1],Pre_Issue->op_type[pre_issue_index][2]);
					//先作为第二类指令分析
	    if(Pre_Issue->op_type[pre_issue_index][0]==1&&Pre_Issue->op_type[pre_issue_index][1]==0){//ADD
	    	//printf("Issue-----run-------2-------ADD\n");
	    	int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
			int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
			int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
			//printf("目的寄存器：%d  寄存器是否占用：%d\n",destination,scoreboardingtable->register_result_status[destination]);
			//printf("源寄存器是否占用：%d\n",scoreboardingtable->register_result_status[original_1]);
			//printf("判断结果：%d\n",scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1);
			if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
				if(pre_issue_index==0){//最前面的指令
					//添加到Pre_ALU
					//printf("Issue-----run-------2-------ADD---------最前面的指令\n");
					Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
					Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
					Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
					Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
					}
					Pre_Alu->rear++;
					//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
					for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
						Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue->rear--;
					Pre_Issue->instructions[Pre_Issue->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
					}
					//
					for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue_front->rear--;
					Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
					}
					//已发射指令数目加1
					issued_instruction_number++;
					//更改Scoreborading
					scoreboardingtable->register_result_status[destination]=1;
				}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
					//printf("Issue-----run-------2-------ADD---------不是排在最前面的\n");
					int cannotissued=0;
					for(int i=0;i<pre_issue_index;i++){
						if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
							//WAR
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
							//WAW
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
						if(Pre_Issue->instruction_content[i][0]!=-1){
							//RAW
							if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
					}
					if(!cannotissued){
						//添加到Pre_ALU
						Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
						Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
						Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
						Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
						for(int i=0;i<3;i++){
							Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
						}
						Pre_Alu->rear++;
						//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
						for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
							Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue->rear--;
						Pre_Issue->instructions[Pre_Issue->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
						}
						//
						for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue_front->rear--;
						Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
						}
						//已发射指令数目加1
						issued_instruction_number++;
						//更改Scoreborading
						scoreboardingtable->register_result_status[destination]=1;
					}
				}
			}else{//不能发射
				//printf("Issue-----run-------2-------ADD---------不能发射\n");
				pre_issue_index++;
			}
	    }else if(Pre_Issue->op_type[pre_issue_index][0]==1&&Pre_Issue->op_type[pre_issue_index][1]==1){//SUB
			//printf("Issue-----run-------2-------SUB\n");
			int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
			int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
			int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
			if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
				if(pre_issue_index==0){//最前面的指令
					//添加到Pre_ALU
					Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
					Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
					Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
					Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
					}
					Pre_Alu->rear++;
					//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
					for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
						Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue->rear--;
					Pre_Issue->instructions[Pre_Issue->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
					}
					//
					for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue_front->rear--;
					Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
					}
					//已发射指令数目加1
					issued_instruction_number++;
					//更改Scoreborading
					scoreboardingtable->register_result_status[destination]=1;
				}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
					int cannotissued=0;
					for(int i=0;i<pre_issue_index;i++){
						if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
							//WAR
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
							//WAW
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
						if(Pre_Issue->instruction_content[i][0]!=-1){
							//RAW
							if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
					}
					if(!cannotissued){
						//添加到Pre_ALU
						Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
						Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
						Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
						Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
						for(int i=0;i<3;i++){
							Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
						}
						Pre_Alu->rear++;
						//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
						for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
							Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue->rear--;
						Pre_Issue->instructions[Pre_Issue->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
						}
						//
						for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue_front->rear--;
						Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
						}
						//已发射指令数目加1
						issued_instruction_number++;
						//更改Scoreborading
						scoreboardingtable->register_result_status[destination]=1;
					}
				}
			}else{//不能发射
				pre_issue_index++;
			}
	    }else if(Pre_Issue->op_type[pre_issue_index][0]==1&&Pre_Issue->op_type[pre_issue_index][1]==2){//MUL
			//printf("Issue-----run-------2-------MUL\n");
			int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
			int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
			int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
			if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alub->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
				if(pre_issue_index==0){//最前面的指令
					//添加到Pre_ALUB
					Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
					Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
					Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
					Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
					for(int i=0;i<3;i++){
						Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
					}
					Pre_Alub->rear++;
					//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
					for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
						Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue->rear--;
					Pre_Issue->instructions[Pre_Issue->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
					}
					//
					for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue_front->rear--;
					Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
					}
					//已发射指令数目加1
					issued_instruction_number++;
					//更改Scoreborading
					scoreboardingtable->register_result_status[destination]=1;
				}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
					int cannotissued=0;
					for(int i=0;i<pre_issue_index;i++){
						if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
							//WAR
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
							//WAW
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
						if(Pre_Issue->instruction_content[i][0]!=-1){
							//RAW
							if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
					}
					if(!cannotissued){
						//添加到Pre_ALU
						Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
						Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
						Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
						Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
						}
						Pre_Alu->rear++;
						//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
						for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
							Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue->rear--;
						Pre_Issue->instructions[Pre_Issue->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
						}
						//
						for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue_front->rear--;
						Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
						}
						//已发射指令数目加1
						issued_instruction_number++;
						//更改Scoreborading
						scoreboardingtable->register_result_status[destination]=1;
					}
				}
			}else{//不能发射
				pre_issue_index++;
			}
	    }else if(Pre_Issue->op_type[pre_issue_index][0]==1&&Pre_Issue->op_type[pre_issue_index][1]==3){//AND
			//printf("Issue-----run-------2-------AND\n");
			int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
			int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
			int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
			if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
				if(pre_issue_index==0){//最前面的指令
					//添加到Pre_ALU
					Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
					Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
					Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
					Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
					}
					Pre_Alu->rear++;
					//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
					for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
						Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue->rear--;
					Pre_Issue->instructions[Pre_Issue->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
					}
					//
					for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue_front->rear--;
					Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
					}
					//已发射指令数目加1
					issued_instruction_number++;
					//更改Scoreborading
					scoreboardingtable->register_result_status[destination]=1;
				}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
					int cannotissued=0;
					for(int i=0;i<pre_issue_index;i++){
						if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
							//WAR
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
							//WAW
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
						if(Pre_Issue->instruction_content[i][0]!=-1){
							//RAW
							if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
					}
					if(!cannotissued){
						//添加到Pre_ALU
						Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
						Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
						Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
						Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
						for(int i=0;i<3;i++){
							Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
						}
						Pre_Alu->rear++;
						//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
						for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
							Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue->rear--;
						Pre_Issue->instructions[Pre_Issue->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
						}
						//
						for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue_front->rear--;
						Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
						}
						//已发射指令数目加1
						issued_instruction_number++;
						//更改Scoreborading
						scoreboardingtable->register_result_status[destination]=1;
					}
				}
			}else{//不能发射
				pre_issue_index++;
			}
	    }else if(Pre_Issue->op_type[pre_issue_index][0]==1&&Pre_Issue->op_type[pre_issue_index][1]==4){//NOR
			//printf("Issue-----run-------2-------NOR\n");
			int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
			int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
			int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
			if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
				if(pre_issue_index==0){//最前面的指令
					//添加到Pre_ALU
					Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
					Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
					Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
					Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
					}
					Pre_Alu->rear++;
					//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
					for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
						Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue->rear--;
					Pre_Issue->instructions[Pre_Issue->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
					}
					//
					for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue_front->rear--;
					Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
					}
					//已发射指令数目加1
					issued_instruction_number++;
					//更改Scoreborading
					scoreboardingtable->register_result_status[destination]=1;
				}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
					int cannotissued=0;
					for(int i=0;i<pre_issue_index;i++){
						if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
							//WAR
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
							//WAW
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
						if(Pre_Issue->instruction_content[i][0]!=-1){
							//RAW
							if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
					}
					if(!cannotissued){
						//添加到Pre_ALU
						Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
						Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
						Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
						Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
						for(int i=0;i<3;i++){
							Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
						}
						Pre_Alu->rear++;
						//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
						for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
							Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue->rear--;
						Pre_Issue->instructions[Pre_Issue->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
						}
						//
						for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue_front->rear--;
						Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
						}
						//已发射指令数目加1
						issued_instruction_number++;
						//更改Scoreborading
						scoreboardingtable->register_result_status[destination]=1;
					}
				}
			}else{//不能发射
				pre_issue_index++;
			}
	    }else if(Pre_Issue->op_type[pre_issue_index][0]==1&&Pre_Issue->op_type[pre_issue_index][1]==5){//SLT
			//printf("Issue-----run-------2-------SLT\n");
			int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
			int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
			int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
			if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
				if(pre_issue_index==0){//最前面的指令
					//添加到Pre_ALU
					Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
					Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
					Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
					Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
					}
					Pre_Alu->rear++;
					//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
					for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
						Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue->rear--;
					Pre_Issue->instructions[Pre_Issue->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
					}
					//
					for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
						Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
						for(int j=0;j<3;j++){
							Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
						}
						for(int k=0;k<4;k++){
							Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
						}
					}
					Pre_Issue_front->rear--;
					Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
					for(int i=0;i<3;i++){
						Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
					}
					for(int j=0;j<4;j++){
						Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
					}
					//已发射指令数目加1
					issued_instruction_number++;
					//更改Scoreborading
					scoreboardingtable->register_result_status[destination]=1;
				}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
					int cannotissued=0;
					for(int i=0;i<pre_issue_index;i++){
						if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
							//WAR
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
							//WAW
							if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
						if(Pre_Issue->instruction_content[i][0]!=-1){
							//RAW
							if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
								//有冒险，不能发射
								pre_issue_index++;
								cannotissued=1;
								break;
							}
						}
					}
					if(!cannotissued){
						//添加到Pre_ALU
						Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
						Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
						Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
						Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
						for(int i=0;i<3;i++){
							Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
						}
						Pre_Alu->rear++;
						//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
						for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
							Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue->rear--;
						Pre_Issue->instructions[Pre_Issue->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
						}
						//
						for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
							Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
							for(int j=0;j<3;j++){
								Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
							}
							for(int k=0;k<4;k++){
								Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
							}
						}
						Pre_Issue_front->rear--;
						Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
						for(int i=0;i<3;i++){
							Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
						}
						for(int j=0;j<4;j++){
							Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
						}
						//已发射指令数目加1
						issued_instruction_number++;
						//更改Scoreborading
						scoreboardingtable->register_result_status[destination]=1;
					}
				}
			}else{//不能发射
				pre_issue_index++;
			}
	    }else{ //作为第一类指令分析
			if(Pre_Issue->op_type[pre_issue_index][0]==0){
				if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==0){//SLL
					//printf("Issue-----run-------1-------SLL\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alub->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALUB
							Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
							Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alub->destinationandoperands[Pre_Alub->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
							for(int i=0;i<3;i++){
								Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alub->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];//l立即数
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==2){//SRL
					//printf("Issue-----run-------1-------SRL\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Alub->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALUB
							Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
							Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alub->destinationandoperands[Pre_Alub->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];
							//printf("Issue--sa=%d\n",Pre_Issue->instruction_content[pre_issue_index][3]);
							for(int i=0;i<3;i++){
								Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alub->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALUB
								Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
								Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alub->destinationandoperands[Pre_Alub->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];
								//printf("Issue--%d\n",Pre_Issue->instruction_content[pre_issue_index][3]);
								for(int i=0;i<3;i++){
									Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alub->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==3){//SRA
					//printf("Issue-----run-------1-------SRA\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alub->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALUB
							Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
							Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alub->destinationandoperands[Pre_Alub->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alub->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==32){//ADD
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					//printf("目的寄存器R%d是否占用：%d\n",destination,scoreboardingtable->register_result_status[destination]);
					//printf("源寄存器R%d是否占用：%d\n",original_1,scoreboardingtable->register_result_status[original_1]);
					//printf("源寄存器R%d是否占用：%d\n",original_2,scoreboardingtable->register_result_status[original_2]);
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALU
							//printf("Issue-----run-------1-------ADD---最前面的指令\n");
							Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
							Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alu->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						//printf("Issue-----run-------1-------ADD---不能发射\n");
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==34){//SUB
					//printf("Issue-----run-------1-------SUB\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALU
							Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
							Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alu->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==36){//AND
					//printf("Issue-----run-------1-------AND\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALU
							Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
							Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alu->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==37){//OR
					//printf("Issue-----run-------1-------OR\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALU
							Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
							Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alu->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==39){//NOR
					//printf("Issue-----run-------1-------NOR\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALU
							Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
							Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alu->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==0&&Pre_Issue->op_type[pre_issue_index][2]==42){//SLT
					//printf("Issue-----run-------1-------SLT\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alu->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALU
							Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
							Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alu->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALU
								Pre_Alu->instructions[Pre_Alu->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alu->destinationandoperands[Pre_Alu->rear][0]=destination;
								Pre_Alu->destinationandoperands[Pre_Alu->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alu->destinationandoperands[Pre_Alu->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alu->op_type[Pre_Alu->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alu->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==28&&Pre_Issue->op_type[pre_issue_index][2]==2){//MUL
					//printf("Issue-----run-------1-------MUL\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Alub->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_ALU
							Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
							Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
							Pre_Alub->destinationandoperands[Pre_Alub->rear][2]=binary2decimal(registers[original_2]);
							for(int i=0;i<3;i++){
								Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Alub->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_ALUB
								Pre_Alub->instructions[Pre_Alub->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Alub->destinationandoperands[Pre_Alub->rear][0]=destination;
								Pre_Alub->destinationandoperands[Pre_Alub->rear][1]=binary2decimal(registers[original_1]);
								Pre_Alub->destinationandoperands[Pre_Alub->rear][2]=binary2decimal(registers[original_2]);
								for(int i=0;i<3;i++){
									Pre_Alub->op_type[Pre_Alub->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Alub->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==35){//LW
					//printf("Issue-----run-------1-------LW\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[destination]==-1&&scoreboardingtable->register_result_status[original_1]==-1&&Pre_Men->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_MEM
							Pre_Men->instructions[Pre_Men->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Men->destinationandoperands[Pre_Men->rear][0]=destination;
							Pre_Men->destinationandoperands[Pre_Men->rear][1]=binary2decimal(registers[original_1]);
							Pre_Men->destinationandoperands[Pre_Men->rear][3]=Pre_Issue->instruction_content[pre_issue_index][3];
							for(int i=0;i<3;i++){
								Pre_Men->op_type[Pre_Men->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Men->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
							//更改Scoreborading
							scoreboardingtable->register_result_status[destination]=1;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_MEM
								Pre_Men->instructions[Pre_Men->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Men->destinationandoperands[Pre_Men->rear][0]=destination;
								Pre_Men->destinationandoperands[Pre_Men->rear][1]=binary2decimal(registers[original_1]);
								Pre_Men->destinationandoperands[Pre_Men->rear][2]=Pre_Issue->instruction_content[pre_issue_index][3];
								for(int i=0;i<3;i++){
									Pre_Men->op_type[Pre_Men->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Men->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
								//更改Scoreborading
								scoreboardingtable->register_result_status[destination]=1;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}else if(Pre_Issue->op_type[pre_issue_index][1]==43){//SW
				//printf("Issue-----run-------1-------SW\n");
					int destination=Pre_Issue->instruction_content[pre_issue_index][0];//目的寄存器
					int original_1=Pre_Issue->instruction_content[pre_issue_index][1];
					int original_2=Pre_Issue->instruction_content[pre_issue_index][2];
					if(scoreboardingtable->register_result_status[original_1]==-1&&scoreboardingtable->register_result_status[original_2]==-1&&Pre_Men->rear<2){//没有功能单元使用该寄存器--不存在RAW和WAW
						if(pre_issue_index==0){//最前面的指令
							//添加到Pre_MEM
							Pre_Men->instructions[Pre_Men->rear]=Pre_Issue->instructions[pre_issue_index];
							Pre_Men->destinationandoperands[Pre_Men->rear][0]=destination;
							Pre_Men->destinationandoperands[Pre_Men->rear][1]=binary2decimal(registers[original_1]);
							Pre_Men->destinationandoperands[Pre_Men->rear][2]=binary2decimal(registers[original_2]);
							Pre_Men->destinationandoperands[Pre_Men->rear][3]=Pre_Issue->instruction_content[pre_issue_index][3];
							for(int i=0;i<3;i++){
								Pre_Men->op_type[Pre_Men->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
							}
							Pre_Men->rear++;
							//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
							for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
								Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue->rear--;
							Pre_Issue->instructions[Pre_Issue->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
							}
							//
							for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
								Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
								for(int j=0;j<3;j++){
									Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
								}
								for(int k=0;k<4;k++){
									Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
								}
							}
							Pre_Issue_front->rear--;
							Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
							for(int i=0;i<3;i++){
								Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
							}
							for(int j=0;j<4;j++){
								Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
							}
							//已发射指令数目加1
							issued_instruction_number++;
						}else{//该指令不是排在最前面的----对排在其前面的未发射指令检查WAR、WAW、RAW
							int cannotissued=0;
							for(int i=0;i<pre_issue_index;i++){
								if(Pre_Issue->instruction_content[pre_issue_index][0]!=-1){
									//WAR
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][1]||Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
									//WAW
									if(Pre_Issue->instruction_content[pre_issue_index][0]==Pre_Issue->instruction_content[i][0]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
								if(Pre_Issue->instruction_content[i][0]!=-1){
									//RAW
									if(Pre_Issue->instruction_content[pre_issue_index][1]==Pre_Issue->instruction_content[i][0]||Pre_Issue->instruction_content[i][0]==Pre_Issue->instruction_content[pre_issue_index][2]){
										//有冒险，不能发射
										pre_issue_index++;
										cannotissued=1;
										break;
									}
								}
							}
							if(!cannotissued){
								//添加到Pre_MEM
								Pre_Men->instructions[Pre_Men->rear]=Pre_Issue->instructions[pre_issue_index];
								Pre_Men->destinationandoperands[Pre_Men->rear][0]=destination;
								Pre_Men->destinationandoperands[Pre_Men->rear][1]=binary2decimal(registers[original_1]);
								Pre_Men->destinationandoperands[Pre_Men->rear][2]=binary2decimal(registers[original_2]);
								Pre_Men->destinationandoperands[Pre_Men->rear][3]=Pre_Issue->instruction_content[pre_issue_index][3];
								for(int i=0;i<3;i++){
									Pre_Men->op_type[Pre_Men->rear][i]=Pre_Issue->op_type[pre_issue_index][i];
								}
								Pre_Men->rear++;
								//将已发射指令移除Pre_Issue，并将后面的指令依次向前移
								for(int i=pre_issue_index+1;i<Pre_Issue->rear;i++){
									Pre_Issue->instructions[i-1]=Pre_Issue->instructions[i];
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue->op_type[i-1][j]=Pre_Issue->op_type[i][j];
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue->instruction_content[i-1][k]=Pre_Issue->instruction_content[i][k];
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue->rear--;
								Pre_Issue->instructions[Pre_Issue->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue->op_type[Pre_Issue->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue->instruction_content[Pre_Issue->rear][j]=-1;
								}
								//
								for(int i=Pre_Issue->rear +1;i<Pre_Issue_front->rear;i++){
									Pre_Issue_front->instructions[i-1]=Pre_Issue_front->instructions[i];
									for(int j=0;j<3;j++){
										Pre_Issue_front->op_type[i-1][j]=Pre_Issue_front->op_type[i][j];
									}
									for(int k=0;k<4;k++){
										Pre_Issue_front->instruction_content[i-1][k]=Pre_Issue_front->instruction_content[i][k];
									}
								}
								Pre_Issue_front->rear--;
								Pre_Issue_front->instructions[Pre_Issue_front->rear]=-1;
								for(int i=0;i<3;i++){
									Pre_Issue_front->op_type[Pre_Issue_front->rear][i]=-1;
								}
								for(int j=0;j<4;j++){
									Pre_Issue_front->instruction_content[Pre_Issue_front->rear][j]=-1;
								}
								//已发射指令数目加1
								issued_instruction_number++;
							}
						}
					}else{//不能发射
						pre_issue_index++;
					}
				}
	        }
	    }
	}
	//printf("Issue----run-------end\n");
}

int Issue::binary2decimal(char *line)
{
	char original[32];
	get_originalorcomplement(original,line);
	int result=(original[31]-'0')+(original[30]-'0')*2+(original[29]-'0')*4+(original[28]-'0')*8+(original[27]-'0')*16+(original[26]-'0')*32+(original[25]-'0')*64+(original[24]-'0')*128+(original[23]-'0')*256+(original[22]-'0')*512+(original[21]-'0')*1024
		+(original[20]-'0')*2048+(original[19]-'0')*4096+(original[18]-'0')*8192+(original[17]-'0')*16384+(original[16]-'0')*32768+(original[15]-'0')*65536+(original[14]-'0')*131072+(original[13]-'0')*262144
		+(original[12]-'0')*524288+(original[11]-'0')*1048576+(original[10]-'0')*2097152+(original[9]-'0')*4194304+(original[8]-'0')*8388608+(original[7]-'0')*16777216+(original[6]-'0')*33554432+(original[5]-'0')*67108864
		+(original[4]-'0')*134217728+(original[3]-'0')*268435456+(original[2]-'0')*536870912+(original[1]-'0')*536870912*2;
	if(original[0]=='1'){
		int temp=2*result;
		result=result-temp;
	}
	return result;
}

void Issue::get_originalorcomplement(char * out,char *in){//得到原码或补码
	if(in[0]=='1'){
		for(int i=0;i<32;i++){
			if(i==0){
				out[i]=in[i];
			}else{
				if(in[i]=='0'){
					out[i]='1';
				}else{
					out[i]='0';
				}
				
			}
		}
		for(int i=31;i>=0;i--){
			if(out[i]=='0'){
				out[i]='1';
				break;
			}else{
				out[i]='0';
			}
		}
	}else{
		for(int i=0;i<32;i++){
			out[i]=in[i];
		}
	}
}

