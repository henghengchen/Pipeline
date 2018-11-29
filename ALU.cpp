#include "PipleLing.h"
#include "ALU.h"
#include <stdio.h>

void ALU::run(PRE_ALU *Pre_Alu_front,PRE_ALU *Pre_Alu,POST_ALU *Post_Alu){
	if(Pre_Alu->rear!=0){
		if(Pre_Alu->op_type[0][0]==1&&Pre_Alu->op_type[0][1]==0){//ADD
			//printf("ALU------RUN------ADD\n");
			int result=Pre_Alu->destinationandoperands[0][1]+Pre_Alu->destinationandoperands[0][2];
			Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
			//printf("ALU------RUN------ADD---destination:%d\n",Post_Alu->destinationandresult[0]);
			Post_Alu->destinationandresult[1]=result;
			Post_Alu->instructions=Pre_Alu->instructions[0];
			Post_Alu->is_empty=0;
			//Pre_Alu数据前移
			Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
			Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
			Pre_Alu->instructions[1]=-1;
			Pre_Alu_front->instructions[1]=-1;
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
				Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[1][i]=0;
				Pre_Alu_front->op_type[1][i]=0;
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
				Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[1][i]=0;
				Pre_Alu_front->destinationandoperands[1][i]=0;
			}
			Pre_Alu->rear--;
			Pre_Alu_front->rear--;
			//printf("ALU------RUN------ADD-----完成\n");
		}else if(Pre_Alu->op_type[0][0]==1&&Pre_Alu->op_type[0][1]==1){//SUB
			int result=Pre_Alu->destinationandoperands[0][1]-Pre_Alu->destinationandoperands[0][2];
			Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
			Post_Alu->destinationandresult[1]=result;
			Post_Alu->instructions=Pre_Alu->instructions[0];
			Post_Alu->is_empty=0;
			//Pre_Alu数据前移
			Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
			Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
			Pre_Alu->instructions[1]=-1;
			Pre_Alu_front->instructions[1]=-1;
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
				Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[1][i]=0;
				Pre_Alu_front->op_type[1][i]=0;
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
				Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[1][i]=0;
				Pre_Alu_front->destinationandoperands[1][i]=0;
			}
			Pre_Alu->rear--;
			Pre_Alu_front->rear--;
		}else if(Pre_Alu->op_type[0][0]==1&&Pre_Alu->op_type[0][1]==3){//AND
			int result=Pre_Alu->destinationandoperands[0][1]&Pre_Alu->destinationandoperands[0][2];
			Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
			Post_Alu->destinationandresult[1]=result;
			Post_Alu->instructions=Pre_Alu->instructions[0];
			Post_Alu->is_empty=0;
			//Pre_Alu数据前移
			Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
			Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
			Pre_Alu->instructions[1]=-1;
			Pre_Alu_front->instructions[1]=-1;
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
				Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[1][i]=0;
				Pre_Alu_front->op_type[1][i]=0;
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
				Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[1][i]=0;
				Pre_Alu_front->destinationandoperands[1][i]=0;
			}
			Pre_Alu->rear--;
			Pre_Alu_front->rear--;
		}else if(Pre_Alu->op_type[0][0]==1&&Pre_Alu->op_type[0][1]==4){//NOR
			int result=~(Pre_Alu->destinationandoperands[0][1]|Pre_Alu->destinationandoperands[0][2]);
			Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
			Post_Alu->destinationandresult[1]=result;
			Post_Alu->instructions=Pre_Alu->instructions[0];
			Post_Alu->is_empty=0;
			//Pre_Alu数据前移
			Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
			Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
			Pre_Alu->instructions[1]=-1;
			Pre_Alu_front->instructions[1]=-1;
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
				Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[1][i]=0;
				Pre_Alu_front->op_type[1][i]=0;
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
				Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[1][i]=0;
				Pre_Alu_front->destinationandoperands[1][i]=0;
			}
			Pre_Alu->rear--;
			Pre_Alu_front->rear--;
		}else if(Pre_Alu->op_type[0][0]==1&&Pre_Alu->op_type[0][1]==5){//SLT
			int result=Pre_Alu->destinationandoperands[0][1]<Pre_Alu->destinationandoperands[0][2];
			Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
			Post_Alu->destinationandresult[1]=result;
			Post_Alu->instructions=Pre_Alu->instructions[0];
			Post_Alu->is_empty=0;
			//Pre_Alu数据前移
			Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
			Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
			Pre_Alu->instructions[1]=-1;
			Pre_Alu_front->instructions[1]=-1;
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
				Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->op_type[1][i]=0;
				Pre_Alu_front->op_type[1][i]=0;
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
				Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
			}
			for(int i=0;i<3;i++){
				Pre_Alu->destinationandoperands[1][i]=0;
				Pre_Alu_front->destinationandoperands[1][i]=0;
			}
			Pre_Alu->rear--;
			Pre_Alu_front->rear--;
		}else{
			if(Pre_Alu->op_type[0][0]==0){
				if(Pre_Alu->op_type[0][1]==0&&Pre_Alu->op_type[0][2]==32){//ADD
					int result=Pre_Alu->destinationandoperands[0][1]+Pre_Alu->destinationandoperands[0][2];
					Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
					Post_Alu->destinationandresult[1]=result;
					Post_Alu->instructions=Pre_Alu->instructions[0];
					Post_Alu->is_empty=0;
					//Pre_Alu数据前移
					Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
					Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
					Pre_Alu->instructions[1]=-1;
					Pre_Alu_front->instructions[1]=-1;
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
						Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[1][i]=0;
						Pre_Alu_front->op_type[1][i]=0;
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
						Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[1][i]=0;
						Pre_Alu_front->destinationandoperands[1][i]=0;
					}
					Pre_Alu->rear--;
					Pre_Alu_front->rear--;
				}else if(Pre_Alu->op_type[0][1]==0&&Pre_Alu->op_type[0][2]==34){//SUB
					int result=Pre_Alu->destinationandoperands[0][1]-Pre_Alu->destinationandoperands[0][2];
					Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
					Post_Alu->destinationandresult[1]=result;
					Post_Alu->instructions=Pre_Alu->instructions[0];
					Post_Alu->is_empty=0;
					//Pre_Alu数据前移
					Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
					Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
					Pre_Alu->instructions[1]=-1;
					Pre_Alu_front->instructions[1]=-1;
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
						Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[1][i]=0;
						Pre_Alu_front->op_type[1][i]=0;
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
						Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[1][i]=0;
						Pre_Alu_front->destinationandoperands[1][i]=0;
					}
					Pre_Alu->rear--;
					Pre_Alu_front->rear--;
				}else if(Pre_Alu->op_type[0][1]==0&&Pre_Alu->op_type[0][2]==36){//AND
					int result=Pre_Alu->destinationandoperands[0][1]&Pre_Alu->destinationandoperands[0][2];
					Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
					Post_Alu->destinationandresult[1]=result;
					Post_Alu->instructions=Pre_Alu->instructions[0];
					Post_Alu->is_empty=0;
					//Pre_Alu数据前移
					Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
					Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
					Pre_Alu->instructions[1]=-1;
					Pre_Alu_front->instructions[1]=-1;
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
						Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[1][i]=0;
						Pre_Alu_front->op_type[1][i]=0;
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
						Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[1][i]=0;
						Pre_Alu_front->destinationandoperands[1][i]=0;
					}
					Pre_Alu->rear--;
					Pre_Alu_front->rear--;
				}else if(Pre_Alu->op_type[0][1]==0&&Pre_Alu->op_type[0][2]==37){//OR
					int result=Pre_Alu->destinationandoperands[0][1]|Pre_Alu->destinationandoperands[0][2];
					Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
					Post_Alu->destinationandresult[1]=result;
					Post_Alu->instructions=Pre_Alu->instructions[0];
					Post_Alu->is_empty=0;
					//Pre_Alu数据前移
					Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
					Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
					Pre_Alu->instructions[1]=-1;
					Pre_Alu_front->instructions[1]=-1;
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
						Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[1][i]=0;
						Pre_Alu_front->op_type[1][i]=0;
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
						Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[1][i]=0;
						Pre_Alu_front->destinationandoperands[1][i]=0;
					}
					Pre_Alu->rear--;
					Pre_Alu_front->rear--;
				}else if(Pre_Alu->op_type[0][1]==0&&Pre_Alu->op_type[0][2]==39){//NOR
					int result=~(Pre_Alu->destinationandoperands[0][1]|Pre_Alu->destinationandoperands[0][2]);
					Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
					Post_Alu->destinationandresult[1]=result;
					Post_Alu->instructions=Pre_Alu->instructions[0];
					Post_Alu->is_empty=0;
					//Pre_Alu数据前移
					Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
					Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
					Pre_Alu->instructions[1]=-1;
					Pre_Alu_front->instructions[1]=-1;
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
						Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[1][i]=0;
						Pre_Alu_front->op_type[1][i]=0;
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
						Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[1][i]=0;
						Pre_Alu_front->destinationandoperands[1][i]=0;
					}
					Pre_Alu->rear--;
					Pre_Alu_front->rear--;
				}else if(Pre_Alu->op_type[0][1]==0&&Pre_Alu->op_type[0][2]==42){//SLT
			int result=Pre_Alu->destinationandoperands[0][1]<Pre_Alu->destinationandoperands[0][2];
					Post_Alu->destinationandresult[0]=Pre_Alu->destinationandoperands[0][0];
					Post_Alu->destinationandresult[1]=result;
					Post_Alu->instructions=Pre_Alu->instructions[0];
					Post_Alu->is_empty=0;
					//Pre_Alu数据前移
					Pre_Alu->instructions[0]=Pre_Alu->instructions[1];
					Pre_Alu_front->instructions[0]=Pre_Alu_front->instructions[1];
					Pre_Alu->instructions[1]=-1;
					Pre_Alu_front->instructions[1]=-1;
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[0][i]=Pre_Alu->op_type[1][i];
						Pre_Alu_front->op_type[0][i]=Pre_Alu_front->op_type[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->op_type[1][i]=0;
						Pre_Alu_front->op_type[1][i]=0;
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[0][i]=Pre_Alu->destinationandoperands[1][i];
						Pre_Alu_front->destinationandoperands[0][i]=Pre_Alu_front->destinationandoperands[1][i];
					}
					for(int i=0;i<3;i++){
						Pre_Alu->destinationandoperands[1][i]=0;
						Pre_Alu_front->destinationandoperands[1][i]=0;
					}
					Pre_Alu->rear--;
					Pre_Alu_front->rear--;
				}
			}
		}
	}else{
		Post_Alu->is_empty=1;
		Post_Alu->instructions=-1;
		Post_Alu->destinationandresult[0]=-1;
		Post_Alu->destinationandresult[1]=-1;
	}
}