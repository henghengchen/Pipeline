#include "PipleLing.h"
#include "IF.h"
#include <stdio.h>

void IF::run(ScoreBoardingTable *scoreboardingtable,char InstructionStorage[][MAXLINE],char executeInstructionStorage[][MAXLINE],char registers[][32],PRE_Issue *Pre_Issue,int *is_break){
	printf("IF---run---start\n");
	printf("PC=%d\n",pc);
	//printf("%d %d %d %d %d %d\n",op_table[0][0],op_table[0][1],op_table[0][2],op_table[0][3],op_table[0][4],op_table[0][5]);
	if(Waiting_Instruction!=-1){
		if(op_type[0]==0&&op_type[1]==4){//分支指令BEQ
		printf("IF-----run-------Wait-------BEQ\n");
			//查表
			if(scoreboardingtable->register_result_status[instruction_content[2]]==-1&&scoreboardingtable->register_result_status[instruction_content[1]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[0][0]!=instruction_content[2]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[2]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[2]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[2]){
				Executed_Instruction=Waiting_Instruction;
				Waiting_Instruction=-1;
				if(binary2decimal(registers[instruction_content[2]])==binary2decimal(registers[instruction_content[1]])){
					pc=pc+(instruction_content[3]*4);
				}
			}
		}else if(op_type[0]==0&&op_type[1]==7){//分支指令BGTZ
		printf("IF-----run-------Wait-------BGTZ\n");
			//查表
			if(scoreboardingtable->register_result_status[instruction_content[1]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
				//printf("BGTZ可以进入Executed_Instruction，原寄存器R%d=%d\n",instruction_content[1],binary2decimal(registers[instruction_content[1]]));
				Executed_Instruction=Waiting_Instruction;
				Waiting_Instruction=-1;
				if(binary2decimal(registers[instruction_content[1]])>0){
					pc=pc+(instruction_content[3]*4);
					//printf("BGTZ更改pc\n");
				}
			}
		}else if(op_type[0]==0&&op_type[1]==0&&op_type[2]==8){//分支指令JR
		printf("IF-----run-------Wait-------JR\n");
			//查表
			if(scoreboardingtable->register_result_status[instruction_content[1]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
				Executed_Instruction=Waiting_Instruction;
				Waiting_Instruction=-1;
				pc=binary2decimal(registers[instruction_content[1]]);
			}
		}else if(op_type[0]==0&&op_type[1]==1&&op_type[2]==0){//分支指令BLTZ
		printf("IF-----run-------Wait-------BLTZ\n");
			//查表
			if(scoreboardingtable->register_result_status[instruction_content[1]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
				Executed_Instruction=Waiting_Instruction;
				Waiting_Instruction=-1;
				if(binary2decimal(registers[instruction_content[1]])<0){
					pc=pc+(instruction_content[3]*4);
				}
			}
		}
	}else{
		Executed_Instruction=-1;
	    if(Pre_Issue->rear<Pre_Issue->maxsize){//队列不满
	        if(Pre_Issue->rear==Pre_Issue->maxsize-1){//--------------Pre_issue只有一个空位
	        printf("只有一个空位\n");
	            decode_instruction(InstructionStorage[(pc-FIRSTINSTRUCTLOCATION)/4]);
	            int pc_temp=pc;
	            pc=pc+4;
	            if(op_type[0]==0&&op_type[1]==2){//分支指令J---立即
	            	printf("IF-----run-----1空---------J\n");
	                Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
	                pc=instruction_content[3];
					printf("J指令跳转目的地址：%d\n",pc);
	            }else if(op_type[0]==0&&op_type[1]==4){//分支指令BEQ
	            printf("IF-----run-------1空-------BEQ\n");
	                //查表
	                if(scoreboardingtable->register_result_status[instruction_content[2]]!=-1||scoreboardingtable->register_result_status[instruction_content[1]]!=-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[0][0]!=instruction_content[2]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[2]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[2]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[2]){
	                    Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
	                }else{
	                    Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						if(binary2decimal(registers[instruction_content[2]])==binary2decimal(registers[instruction_content[1]])){
	                    	pc=pc+(instruction_content[3]*4);
						}
	                }
	            }else if(op_type[0]==0&&op_type[1]==7){//分支指令BGTZ
	            printf("IF-----run-------1空-------BGTZ\n");
	                //查表
	                if(scoreboardingtable->register_result_status[instruction_content[1]]!=-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
	                    Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
	                }else{
	                    Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						if(binary2decimal(registers[instruction_content[1]])>0){
	                    	pc=pc+(instruction_content[3]*4);
						}
	                }
	            }else if(op_type[0]==0&&op_type[1]==0&&op_type[2]==8){//分支指令JR
	            printf("IF-----run---------1空-----JR\n");
	                //查表
	                if(scoreboardingtable->register_result_status[instruction_content[1]]!=-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
	                    Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
	                }else{
	                    Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
	                    pc=binary2decimal(registers[instruction_content[1]]);
	                }
	            }else if(op_type[0]==0&&op_type[1]==1&&op_type[2]==0){//分支指令BLTZ
	            printf("IF-----run--------1空------BLTZ\n");
	                //查表
	                if(scoreboardingtable->register_result_status[instruction_content[1]]!=-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
	                    Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
	                }else{
	                    Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						if(binary2decimal(registers[instruction_content[1]])<0){
	                    	pc=pc+(instruction_content[3]*4);
						}
	                }
	            }else if(op_type[0]==0&&op_type[1]==0&&op_type[2]==13){//BREAK
	            	Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
					*is_break=1;
	            }else{
					printf("IF-----run-----1空---------取\n");
	                //取指令放入Pre_Issue
	                Pre_Issue->instructions[Pre_Issue->rear]=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
					//printf("IF-放入指令%d\n",Pre_Issue->instructions[Pre_Issue->rear]);
					//printf("%d %d %d %d\n",Pre_Issue->instructions[0],Pre_Issue->instructions[1],Pre_Issue->instructions[2],Pre_Issue->instructions[3]);
	                for(int i=0;i<3;i++){
	                    Pre_Issue->op_type[Pre_Issue->rear][i]=op_type[i];
	                }
	                for(int i=0;i<4;i++){
	                    Pre_Issue->instruction_content[Pre_Issue->rear][i]=instruction_content[i];
	                }
	                Pre_Issue->rear=Pre_Issue->rear+1;
					//printf("--%d %d %d %d\n",Pre_Issue->instructions[0],Pre_Issue->instructions[1],Pre_Issue->instructions[2],Pre_Issue->instructions[3]);
	            }
	        }else if(Pre_Issue->rear<Pre_Issue->maxsize-1){//--------------Pre_issue多于一个空位
				for(int i=0;i<2;i++){
					printf("循环取两条指令\n");
	                decode_instruction(InstructionStorage[(pc-FIRSTINSTRUCTLOCATION)/4]);
	                int pc_temp=pc;
	                pc=pc+4;
					//printf("指令解码完成\n");
	                if(op_type[0]==0&&op_type[1]==2){//分支指令J---立即
	                	printf("IF-----run-----2空---------J\n");
						Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						pc=instruction_content[3];
						printf("J指令跳转目的地址：%d\n",pc);
	                    break;
	                }else if(op_type[0]==0&&op_type[1]==4){//分支指令BEQ
	                	printf("IF-----run-----2空---------BEQ\n");
						//查表
						if(scoreboardingtable->register_result_status[instruction_content[2]]==-1&&scoreboardingtable->register_result_status[instruction_content[1]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[0][0]!=instruction_content[2]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[2]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[2]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[2]){
							Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
							if(binary2decimal(registers[instruction_content[2]])==binary2decimal(registers[instruction_content[1]])){
								pc=pc+(instruction_content[3]*4);
							}
						}else{
							Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						}
	                    break;
	                }else if(op_type[0]==0&&op_type[1]==7){//分支指令BGTZ
	                printf("IF-----run-----2空---------BGTZ\n");
						//查表
						if(scoreboardingtable->register_result_status[instruction_content[2]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
							Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
							if(binary2decimal(registers[instruction_content[1]])>0){
								pc=pc+(instruction_content[3]*4);
							}
						}else{
							Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						}
	                    break;
	                }else if(op_type[0]==0&&op_type[1]==0&&op_type[2]==8){//分支指令JR
	                printf("IF-----run-----2空---------JR\n");
		                //查表
		                if(scoreboardingtable->register_result_status[instruction_content[0]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
		                    Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
		                    pc=binary2decimal(registers[instruction_content[1]]);
		                }else{
		                    Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						}
	                    break;
	                }else if(op_type[0]==0&&op_type[1]==1&&op_type[2]==0){//分支指令BLTZ
	                printf("IF-----run-----2空---------BLTZ\n");
						//查表
						if(scoreboardingtable->register_result_status[instruction_content[1]]==-1&&Pre_Issue->instruction_content[0][0]!=instruction_content[1]&&Pre_Issue->instruction_content[1][0]!=instruction_content[1]&&Pre_Issue->instruction_content[2][0]!=instruction_content[1]&&Pre_Issue->instruction_content[3][0]!=instruction_content[1]){
							Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
							if(binary2decimal(registers[instruction_content[1]])<0){
								pc=pc+(instruction_content[3]*4);
							}
						}else{
							Waiting_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						}
	                    break;
	                }else if(op_type[0]==0&&op_type[1]==0&&op_type[2]==13){//BREAK
	                printf("IF-----run-----2空---------BREAK\n");
						Executed_Instruction=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						*is_break=1;
						break;
	            	}else{
	                    //将指令放入Pre_Issue，继续下一个循环
	                    printf("IF-----run-----2空---------取\n");
						//printf("下一条指令的地址：%d\n",pc);
						//printf("%s\n",InstructionStorage[(pc-FIRSTINSTRUCTLOCATION)/4]);
	                    Pre_Issue->instructions[Pre_Issue->rear]=(pc_temp-FIRSTINSTRUCTLOCATION)/4;
						//printf("汇编指令地址：%d\n",(pc_temp-FIRSTINSTRUCTLOCATION)/4);
	                    for(int i=0;i<3;i++){
	                        Pre_Issue->op_type[Pre_Issue->rear][i]=op_type[i];
	                    }
						//printf("写入指令类型\n");
	                    for(int i=0;i<4;i++){
	                        Pre_Issue->instruction_content[Pre_Issue->rear][i]=instruction_content[i];
	                    }
						//printf("写入指令内容型\n");
	                    Pre_Issue->rear=Pre_Issue->rear+1;
	                }
	            }
	        }else{//--------------Pre_issue没有一个空位
	        //printf("没有空位\n");
	            printf("No space in Pre_issue------------stall\n");
	        }
	    }
	}
}

void IF::decode_instruction(char *instruction){
    int *opcode_xy;
	int *special_xy;
	int *special2_xy;
	int *special3_xy;
	int *regimm_xy;
    char *line=instruction;
    //先作为第二类指令分析
    if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='0'){//ADD
    //printf("判断为第二类ADD指令\n");
        int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
        int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
        int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
        op_type[0]=1;
        op_type[1]=0;
        instruction_content[0]=rt;
        instruction_content[1]=rs;
        instruction_content[3]=imm;
    }else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){//SUB
        int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
        int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
        int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
        op_type[0]=1;
        op_type[1]=1;
        instruction_content[0]=rt;
        instruction_content[1]=rs;
        instruction_content[3]=imm;
    }else if(line[0]=='1'&&line[1]=='0'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){//MUL
        int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
        int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
        int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
        op_type[0]=1;
        op_type[1]=2;
        instruction_content[0]=rt;
        instruction_content[1]=rs;
        instruction_content[3]=imm;
    }else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='0'){//AND
        int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
        int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
        int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
        op_type[0]=1;
        op_type[1]=3;
        instruction_content[0]=rs;
        instruction_content[1]=rt;
        instruction_content[3]=imm;
    }else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='1'){//NOR
        int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
        int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
        int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
        op_type[0]=1;
        op_type[1]=4;
        instruction_content[0]=rt;
        instruction_content[1]=rs;
        instruction_content[3]=imm;
    }else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='1'&&line[4]=='0'&&line[5]=='1'){//SLT
        int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
        int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
        int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
        op_type[0]=1;
        op_type[1]=5;
        instruction_content[0]=rt;
        instruction_content[1]=rs;
        instruction_content[3]=imm;
    }else{ //作为第一类指令分析
        opcode_xy=get_opcode(line);
        int x=opcode_xy[0];
        int y=opcode_xy[1];
        int opcode=op_table[x][y];
        if(opcode==0){
            special_xy=get_special(line);
            int xx=special_xy[0];
            int yy=special_xy[1];
            int special=special_table[xx][yy];
            if(special==0){//SLL
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=0;
                instruction_content[0]=rd;
                instruction_content[1]=rt;
                instruction_content[3]=sa;
            }else if(special==2){//SRL
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=2;
                instruction_content[0]=rd;
                instruction_content[1]=rt;
                instruction_content[3]=sa;
            }else if(special==3){//SRA
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=3;
                instruction_content[0]=rd;
                instruction_content[1]=rt;
                instruction_content[3]=sa;
            }else if(special==8){//JR
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=8;
                instruction_content[1]=rs;
            }else if(special==13){//BREAK
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=13;
            }else if(special==32){//ADD
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=32;
                instruction_content[0]=rd;
                instruction_content[1]=rs;
                instruction_content[2]=rt;
            }else if(special==34){//SUB
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=34;
                instruction_content[0]=rd;
                instruction_content[1]=rs;
                instruction_content[2]=rt;
            }else if(special==36){//AND
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=36;
                instruction_content[0]=rd;
                instruction_content[1]=rs;
                instruction_content[2]=rt;
            }else if(special==37){//OR
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=37;
                instruction_content[0]=rd;
                instruction_content[1]=rs;
                instruction_content[2]=rt;
            }else if(special==39){//NOR
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=39;
                instruction_content[0]=rd;
                instruction_content[1]=rs;
                instruction_content[2]=rt;
            }else if(special==42){//SLT
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                op_type[0]=0;
                op_type[1]=0;
                op_type[2]=42;
                instruction_content[0]=rd;
                instruction_content[1]=rs;
                instruction_content[2]=rt;
            }else{
            }
        }else if(opcode==1){
        //printf("opcode==1\n");
            regimm_xy=get_regimm(line);
            int xx=regimm_xy[0];
            int yy=regimm_xy[1];
            int regimm=regimm_table[xx][yy];
            if(regimm==0){//BLTZ
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
                +(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
                +(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
                +(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
                op_type[0]=0;
                op_type[1]=1;
                op_type[2]=0;
                instruction_content[1]=rs;
                instruction_content[3]=offset;
            }else{

            }
        }else if(opcode==2){//J
            int target=(line[6]-'0')*16777216+(line[7]-'0')*8388608+(line[8]-'0')*4194304+(line[9]-'0')*2097152+(line[10]-'0')*1048576
                +(line[11]-'0')*524288+(line[12]-'0')*262144+(line[13]-'0')*131072+(line[14]-'0')*65536+(line[15]-'0')*32768
                +(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
                +(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
                +(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
                +(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
                op_type[0]=0;
                op_type[1]=2;
				unsigned int temp=pc;
				temp=temp/268435465;
				temp=temp*268435465;
				temp=temp+target*4;
				instruction_content[3]=temp;
        }else if(opcode==4){//BEQ
            int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
            int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
            int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
            +(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
            +(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
            +(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
            op_type[0]=0;
            op_type[1]=4;
            instruction_content[1]=rs;
            instruction_content[2]=rt;
            instruction_content[3]=offset;
        }else if(opcode==7){//BGTZ
            int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
            int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
            +(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
            +(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
            +(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
            op_type[0]=0;
            op_type[1]=7;
            instruction_content[1]=rs;
            instruction_content[3]=offset;
        }else if(opcode==28){
            special2_xy=get_special2(line);
            int xx=special2_xy[0];
            int yy=special2_xy[1];
            int special2=special2_table[xx][yy];
            if(special2==0){

            }else if(special2==2){//MUL
                int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
                int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
                int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
                op_type[0]=0;
                op_type[1]=28;
                op_type[2]=2;
                instruction_content[0]=rd;
                instruction_content[1]=rs;
                instruction_content[2]=rt;
            }else{
                
            }
        }else if(opcode==31){
            special3_xy=get_special3(line);
            int xx=special3_xy[0];
            int yy=special3_xy[1];
            int special3=special3_table[xx][yy];
            if(special3==0){

            }else{
                
            }
        }else if(opcode==35){//LW
            int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
            int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
                    +(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
                    +(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
                    +(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
            int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
            op_type[0]=0;
            op_type[1]=35;
            instruction_content[0]=rt;
            instruction_content[3]=offset;
            instruction_content[1]=base;
        }else if(opcode==43){//SW
            int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
            int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
                    +(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
                    +(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
                    +(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
            int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
            op_type[0]=0;
            op_type[1]=43;
            instruction_content[1]=rt;
            instruction_content[2]=base;
            instruction_content[3]=offset;
        }else{
            
        }
    }
}

void IF::translate_instruction(char instruction[][MAXLINE],char disassemblyinstructions[][MAXLINE],int memory[],int *firstmemory){
    int *opcode_xy;
	int *special_xy;
	int *special2_xy;
	int *special3_xy;
	int *regimm_xy;
    char *line=0;
	char disassembly[20];
	int current_index=0;
	for(current_index=0;current_index<MAXINSTRUCTIONNUMBER;current_index++){
		line=instruction[current_index];
		//先作为第二类指令分析
		if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='0'){//ADD
		//printf("判断为第二类ADD指令\n");
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			sprintf(disassembly,"%s	R%d, R%d, #%d","ADD",rt,rs,imm);
			for(int i=0;i<20;i++){
				disassemblyinstructions[current_index][i]=disassembly[i];
			}
		}else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){//SUB
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			sprintf(disassembly,"%s	R%d, R%d, #%d","SUB",rt,rs,imm);
			for(int i=0;i<20;i++){
				disassemblyinstructions[current_index][i]=disassembly[i];
			}
		}else if(line[0]=='1'&&line[1]=='0'&&line[2]=='0'&&line[3]=='0'&&line[4]=='0'&&line[5]=='1'){//MUL
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			sprintf(disassembly,"%s	R%d, R%d, #%d","MUL",rt,rs,imm);
			for(int i=0;i<20;i++){
				disassemblyinstructions[current_index][i]=disassembly[i];
			}
		}else if(line[0]=='1'&&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='0'){//AND
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			sprintf(disassembly,"%s	R%d, R%d, #%d","AND",rt,rs,imm);
			for(int i=0;i<20;i++){
				disassemblyinstructions[current_index][i]=disassembly[i];
			}
		}else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='0'&&line[4]=='1'&&line[5]=='1'){//NOR
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			sprintf(disassembly,"%s	R%d, R%d, #%d","NOR",rt,rs,imm);
			for(int i=0;i<20;i++){
				disassemblyinstructions[current_index][i]=disassembly[i];
			}
		}else if(line[0]=='1'&line[1]=='1'&&line[2]=='0'&&line[3]=='1'&&line[4]=='0'&&line[5]=='1'){//SLT
			int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
			int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
			int imm=(line[16]-'0')*16384+(line[17]-'0')*8192+(line[18]-'0')*4096+(line[19]-'0')*(2^12)+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
			sprintf(disassembly,"%s	R%d, R%d, #%d","SLT",rt,rs,imm);
			for(int i=0;i<20;i++){
				disassemblyinstructions[current_index][i]=disassembly[i];
			}
		}else{ //作为第一类指令分析
			opcode_xy=get_opcode(line);
			int x=opcode_xy[0];
			int y=opcode_xy[1];
			int opcode=op_table[x][y];
			if(opcode==0){
				special_xy=get_special(line);
				int xx=special_xy[0];
				int yy=special_xy[1];
				int special=special_table[xx][yy];
				if(special==0){//SLL
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					sprintf(disassembly,"%s	R%d, R%d, #%d","SLL",rd,rt,sa);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==2){//SRL
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					sprintf(disassembly,"%s	R%d, R%d, #%d","SRL",rd,rt,sa);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==3){//SRA
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					int sa=(line[21]-'0')*16+(line[22]-'0')*8+(line[23]-'0')*4+(line[24]-'0')*2+(line[25]-'0');
					sprintf(disassembly,"%s	R%d, R%d, #%d","SRA",rd,rt,sa);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==8){//JR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					sprintf(disassembly,"%s	R%d","JR",rs);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==13){//BREAK
					sprintf(disassembly,"%s","BREAK");
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
					break;
				}else if(special==32){//ADD
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					sprintf(disassembly,"%s	R%d, R%d, R%d","ADD",rd,rs,rt);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==34){//SUB
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					sprintf(disassembly,"%s	R%d, R%d, R%d","SUB",rd,rs,rt);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==36){//AND
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					sprintf(disassembly,"%s	R%d, R%d, R%d","AND",rd,rs,rt);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==37){//OR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					sprintf(disassembly,"%s	R%d, R%d, R%d","OR",rd,rs,rt);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==39){//NOR
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					sprintf(disassembly,"%s	R%d, R%d, R%d","NOR",rd,rs,rt);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else if(special==42){//SLT
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					sprintf(disassembly,"%s	R%d, R%d, R%d","SLT",rd,rs,rt);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else{
				}
			}else if(opcode==1){
			//printf("opcode==1\n");
				regimm_xy=get_regimm(line);
				int xx=regimm_xy[0];
				int yy=regimm_xy[1];
				int regimm=regimm_table[xx][yy];
				if(regimm==0){//BLTZ
				//printf("解析BLTZ\n");
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
					+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
					+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
					+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
					sprintf(disassembly,"%s	R%d, #%d","BLTZ",rs,offset*4);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else{
					
				}
			}else if(opcode==2){//J
			//printf("opcode==2\n");
			//printf("解析J\n");
				int target=(line[6]-'0')*16777216+(line[7]-'0')*8388608+(line[8]-'0')*4194304+(line[9]-'0')*2097152+(line[10]-'0')*1048576
					+(line[11]-'0')*524288+(line[12]-'0')*262144+(line[13]-'0')*131072+(line[14]-'0')*65536+(line[15]-'0')*32768
					+(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
					+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
					+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
					+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
					unsigned int temp=pc;
					temp=temp/268435465;
					temp=temp*268435465;
					temp=temp+target*4;
					sprintf(disassembly,"%s	#%d","J",temp);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
			}else if(opcode==4){//BEQ
				int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
				+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
				+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
				+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				sprintf(disassembly,"%s	R%d, R%d, #%d","BEQ",rs,rt,offset*4);
				for(int i=0;i<20;i++){
					disassemblyinstructions[current_index][i]=disassembly[i];
				}
			}else if(opcode==7){//BGTZ
				int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
				+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
				+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
				+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				sprintf(disassembly,"%s	R%d, #%d","BGTZ",rs,offset*4);
				for(int i=0;i<20;i++){
					disassemblyinstructions[current_index][i]=disassembly[i];
				}
			}else if(opcode==28){
				special2_xy=get_special2(line);
				int xx=special2_xy[0];
				int yy=special2_xy[1];
				int special2=special2_table[xx][yy];
				if(special2==0){
		
				}else if(special2==2){//MUL
					int rs=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
					int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
					int rd=(line[16]-'0')*16+(line[17]-'0')*8+(line[18]-'0')*4+(line[19]-'0')*2+(line[20]-'0');
					sprintf(disassembly,"%s	R%d, R%d, R%d","MUL",rd,rs,rt);
					for(int i=0;i<20;i++){
						disassemblyinstructions[current_index][i]=disassembly[i];
					}
				}else{
					
				}
			}else if(opcode==31){
				special3_xy=get_special3(line);
				int xx=special3_xy[0];
				int yy=special3_xy[1];
				int special3=special3_table[xx][yy];
				if(special3==0){
		
				}else{
					
				}
			}else if(opcode==35){//LW
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
						+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
						+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
						+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				sprintf(disassembly,"%s	R%d, %d(R%d)","LW",rt,offset,base);
				for(int i=0;i<20;i++){
					disassemblyinstructions[current_index][i]=disassembly[i];
				}
			}else if(opcode==43){//SW
				int rt=(line[11]-'0')*16+(line[12]-'0')*8+(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
				int offset=(line[16]-'0')*4096*8+(line[17]-'0')*4096*4+(line[18]-'0')*4096*2
						+(line[19]-'0')*4096+(line[20]-'0')*2048+(line[21]-'0')*1024+(line[22]-'0')*512
						+(line[23]-'0')*256+(line[24]-'0')*128+(line[25]-'0')*64+(line[26]-'0')*32
						+(line[27]-'0')*16+(line[28]-'0')*8+(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
				int base=(line[6]-'0')*16+(line[7]-'0')*8+(line[8]-'0')*4+(line[9]-'0')*2+(line[10]-'0');
				sprintf(disassembly,"%s	R%d, %d(R%d)","SW",rt,offset,base);
				for(int i=0;i<20;i++){
					disassemblyinstructions[current_index][i]=disassembly[i];
				}
			}else{
				
			}
		}
	}
	current_index++;
	*firstmemory=current_index;
	for(current_index;current_index<MAXINSTRUCTIONNUMBER;current_index++){
		memory[current_index]=binary2decimal(instruction[current_index]);
	}
}


int* IF::get_opcode(char *line)
{
	int opcode[2];
	opcode[0]=(line[0]-'0')*4+(line[1]-'0')*2+(line[2]-'0');
	opcode[1]=(line[3]-'0')*4+(line[4]-'0')*2+(line[5]-'0');
	//printf("x=%d,y=%d\n",opcode[0],opcode[1]);
	int *p=opcode;
	return p;
}

int* IF::get_special(char *line)
{
	int special[2];
	special[0]=(line[26]-'0')*4+(line[27]-'0')*2+(line[28]-'0');
	special[1]=(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
	int *p=special;
	return p;
}

int* IF::get_special2(char *line)
{
	int special[2];
	special[0]=(line[26]-'0')*4+(line[27]-'0')*2+(line[28]-'0');
	special[1]=(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
	int *p=special;
	return p;
}
int* IF::get_special3(char *line)
{
	int special[2];
	special[0]=(line[26]-'0')*4+(line[27]-'0')*2+(line[28]-'0');
	special[1]=(line[29]-'0')*4+(line[30]-'0')*2+(line[31]-'0');
	int *p=special;
	return p;
}

int* IF::get_regimm(char * line)
{
	int regimm[2];
	regimm[0]=(line[11]-'0')*2+(line[12]-'0');
	regimm[1]=(line[13]-'0')*4+(line[14]-'0')*2+(line[15]-'0');
	int *p=regimm;
	return p;
}

int IF::binary2decimal(char *line)
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

void IF::get_originalorcomplement(char * out,char *in){//得到原码或补码
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

int IF::getWaitingInstruction(){
	return Waiting_Instruction;
}

int IF::getExecutedInstruction(){
	return Executed_Instruction;
}

