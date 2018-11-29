#include <stdio.h>
#include <iostream>
#include "PipleLing.h"
#include "ALU.h"
#include "ALUB.h"
#include "IF.h"
#include "Issue.h"
#include "MEN.h"
#include "WB.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int main(int argc,char *argv[])
{
	int cycle=0;
	int is_break=0;
	char registers[32][32];//寄存器0-31
	for(int i=0;i<32;i++)
	{
		for(int j=0;j<32;j++){
			registers[i][j]='0';
		}
	}
	char instructions[MAXINSTRUCTIONNUMBER][MAXLINE]={0};
	char disassemblyinstructions[MAXINSTRUCTIONNUMBER][MAXLINE]={0};
	int memory[MAXINSTRUCTIONNUMBER];
	int first_memory;
    FILE *fp_r=fopen(argv[1],"r");
	FILE *fp_w_simulation=fopen(argv[2],"w");
	if(fp_r==NULL)
		printf("open file failed\n");
	int j=0;
	char inst[MAXLINE];
	while(fgets(inst,MAXLINE,fp_r)!=NULL){
		if(inst[0]<'0'||inst[0]>'1')
			continue;
		for(int i=0;i<MAXLINE;i++){
			instructions[j][i]=inst[i];
		}
        j++;
	}
	
	if(fclose(fp_r)!=0){
		printf("Error in closing file \n");
	}
	printf("close input file sucessfully\n");

    //
    IF If;
    Issue issue;
    MEN Men;
    ALU Alu;
    ALUB Alub;
    WB Wb;
	//
    ScoreBoardingTable scoreboardingtable;
    PRE_Issue Pre_Issue1;
	PRE_Issue Pre_Issue2;
    PRE_MEN Pre_Men1;
	PRE_MEN Pre_Men2;
    PRE_ALU Pre_Alu1;
	PRE_ALU Pre_Alu2;
    PRE_ALUB Pre_Alub1;
	PRE_ALUB Pre_Alub2;
	POST_MEN Post_Men1;
	POST_MEN Post_Men2;
	POST_ALU Post_Alu1;
	POST_ALU Post_Alu2;
	POST_ALUB Post_Alub1;
	POST_ALUB Post_Alub2;
	If.translate_instruction(instructions,disassemblyinstructions,memory,&first_memory);
    while(!is_break)
    {
    	cycle++;
		printf("--------------------cycle:%d\n",cycle);
        If.run(&scoreboardingtable,instructions,disassemblyinstructions,registers,&Pre_Issue1,&is_break);
        issue.run(&scoreboardingtable,registers,&Pre_Issue1,&Pre_Issue2,&Pre_Men1,&Pre_Alu1,&Pre_Alub1);
        Men.run(memory,&Pre_Men1,&Pre_Men2,&Post_Men1);
        Alu.run(&Pre_Alu1,&Pre_Alu2,&Post_Alu1);
        Alub.run(&Pre_Alub1,&Pre_Alub2,&Post_Alub1);
        Wb.run(&Post_Men1,&Post_Men2,&Post_Alu1,&Post_Alu2,&Post_Alub1,&Post_Alub2,registers,&scoreboardingtable);
		//将1写入2
		//Pre_Issue
		Pre_Issue2.rear=Pre_Issue1.rear;
		for(int i=0;i<4;i++){
			Pre_Issue2.instructions[i]=Pre_Issue1.instructions[i];
			for(int j=0;j<4;j++){
				Pre_Issue2.instruction_content[i][j]=Pre_Issue1.instruction_content[i][j];
			}
			for(int k=0;k<3;k++){
				Pre_Issue2.op_type[i][k]=Pre_Issue1.op_type[i][k];
			}
		}
		//Pre_MEN
		Pre_Men2.rear=Pre_Men1.rear;
		for(int i=0;i<2;i++){
			Pre_Men2.instructions[i]=Pre_Men1.instructions[i];
			for(int j=0;j<4;j++){
				Pre_Men2.destinationandoperands[i][j]=Pre_Men1.destinationandoperands[i][j];
			}
			for(int k=0;k<3;k++){
				Pre_Men2.op_type[i][k]=Pre_Men1.op_type[i][k];
			}
		}
		//Pre_ALU
		Pre_Alu2.rear=Pre_Alu1.rear;
		for(int i=0;i<2;i++){
			Pre_Alu2.instructions[i]=Pre_Alu1.instructions[i];
			for(int j=0;j<3;j++){
				Pre_Alu2.destinationandoperands[i][j]=Pre_Alu1.destinationandoperands[i][j];
				Pre_Alu2.op_type[i][j]=Pre_Alu1.op_type[i][j];
			}
		}
		//Pre_ALUB
		Pre_Alub2.rear=Pre_Alub1.rear;
		for(int i=0;i<2;i++){
			Pre_Alub2.instructions[i]=Pre_Alub1.instructions[i];
			for(int j=0;j<3;j++){
				Pre_Alub2.destinationandoperands[i][j]=Pre_Alub1.destinationandoperands[i][j];
				Pre_Alub2.op_type[i][j]=Pre_Alub1.op_type[i][j];
			}
		}
		//Post_MEN
		Post_Men2.is_empty=Post_Men1.is_empty;
		Post_Men2.instructions=Post_Men1.instructions;
		Post_Men2.destinationandresult[0]=Post_Men1.destinationandresult[0];
		Post_Men2.destinationandresult[1]=Post_Men1.destinationandresult[1];
		//Post_ALU
		Post_Alu2.is_empty=Post_Alu1.is_empty;
		Post_Alu2.instructions=Post_Alu1.instructions;
		Post_Alu2.destinationandresult[0]=Post_Alu1.destinationandresult[0];
		Post_Alu2.destinationandresult[1]=Post_Alu1.destinationandresult[1];
		//Post_ALUB
		Post_Alub2.is_empty=Post_Alub1.is_empty;
		Post_Alub2.instructions=Post_Alub1.instructions;
		Post_Alub2.destinationandresult[0]=Post_Alub1.destinationandresult[0];
		Post_Alub2.destinationandresult[1]=Post_Alub1.destinationandresult[1];
		//写文件
		fputs("--------------------\n",fp_w_simulation);
		//周期
		char print_cycle[10];
		sprintf(print_cycle,"Cycle:%d\n",cycle);
		fputs(print_cycle,fp_w_simulation);
		fputs("\n",fp_w_simulation);
		//IF
		fputs("IF Unit:\n",fp_w_simulation);
		char print_Waiting[55];
		if(If.getWaitingInstruction()==-1){
			sprintf(print_Waiting,"	Waiting Instruction: \n");
		}else{
			sprintf(print_Waiting,"	Waiting Instruction: %s\n",disassemblyinstructions[If.getWaitingInstruction()]);
		}
		fputs(print_Waiting,fp_w_simulation);
		char print_Execute[55];
		if(If.getExecutedInstruction()==-1){
			sprintf(print_Execute,"	Executed Instruction: \n");
		}else{
			sprintf(print_Execute,"	Executed Instruction: %s\n",disassemblyinstructions[If.getExecutedInstruction()]);
		}
		fputs(print_Execute,fp_w_simulation);
		//Pre_Issue
		fputs("Pre-Issue Buffer:\n",fp_w_simulation);
		char entry[45];
		if(Pre_Issue1.instructions[0]==-1){
			sprintf(entry,"	Entry 0:\n");
		}else{
			sprintf(entry,"	Entry 0:[%s]\n",disassemblyinstructions[Pre_Issue1.instructions[0]]);
		}
		fputs(entry,fp_w_simulation);
		if(Pre_Issue1.instructions[1]==-1){
			sprintf(entry,"	Entry 1:\n");
		}else{
			sprintf(entry,"	Entry 1:[%s]\n",disassemblyinstructions[Pre_Issue1.instructions[1]]);
		}
		fputs(entry,fp_w_simulation);
		if(Pre_Issue1.instructions[2]==-1){
			sprintf(entry,"	Entry 2:\n");
		}else{
			sprintf(entry,"	Entry 2:[%s]\n",disassemblyinstructions[Pre_Issue1.instructions[2]]);
		}
		fputs(entry,fp_w_simulation);
		if(Pre_Issue1.instructions[3]==-1){
			sprintf(entry,"	Entry 3:\n");
		}else{
			sprintf(entry,"	Entry 3:[%s]\n",disassemblyinstructions[Pre_Issue1.instructions[3]]);
		}
		fputs(entry,fp_w_simulation);
		//Pre_ALU
		fputs("Pre-ALU Queue:\n",fp_w_simulation);
		if(Pre_Alu1.instructions[0]==-1){
			sprintf(entry,"	Entry 0:\n");
		}else{
			sprintf(entry,"	Entry 0:[%s]\n",disassemblyinstructions[Pre_Alu1.instructions[0]]);
		}
		fputs(entry,fp_w_simulation);
		if(Pre_Alu1.instructions[1]==-1){
			sprintf(entry,"	Entry 1:\n");
		}else{
			sprintf(entry,"	Entry 1:[%s]\n",disassemblyinstructions[Pre_Alu1.instructions[1]]);
		}
		fputs(entry,fp_w_simulation);
		//Post_ALU
		char singleentry[50];
		if(Post_Alu1.instructions==-1){
			sprintf(singleentry,"Post-ALU Buffer:\n");
		}else{
			sprintf(singleentry,"Post-ALU Buffer:[%s]\n",disassemblyinstructions[Post_Alu1.instructions]);
		}
		fputs(singleentry,fp_w_simulation);
		//Pre_ALUB
		fputs("Pre-ALUB Queue:\n",fp_w_simulation);
		if(Pre_Alub1.instructions[0]==-1){
			sprintf(entry,"	Entry 0:\n");
		}else{
			sprintf(entry,"	Entry 0:[%s]\n",disassemblyinstructions[Pre_Alub1.instructions[0]]);
		}
		fputs(entry,fp_w_simulation);
		if(Pre_Alub1.instructions[1]==-1){
			sprintf(entry,"	Entry 1:\n");
		}else{
			sprintf(entry,"	Entry 1:[%s]\n",disassemblyinstructions[Pre_Alub1.instructions[1]]);
		}
		fputs(entry,fp_w_simulation);
		//Post_ALUB
		if(Post_Alub1.instructions==-1){
			sprintf(singleentry,"Post-ALUB Buffer:\n");
		}else{
			sprintf(singleentry,"Post-ALUB Buffer:[%s]\n",disassemblyinstructions[Post_Alub1.instructions]);
		}
		fputs(singleentry,fp_w_simulation);
		//Pre_MEM
		fputs("Pre-MEM Queue:\n",fp_w_simulation);
		if(Pre_Men1.instructions[0]==-1){
			sprintf(entry,"	Entry 0:\n");
		}else{
			sprintf(entry,"	Entry 0:[%s]\n",disassemblyinstructions[Pre_Men1.instructions[0]]);
		}
		fputs(entry,fp_w_simulation);
		if(Pre_Men1.instructions[1]==-1){
			sprintf(entry,"	Entry 1:\n");
		}else{
			sprintf(entry,"	Entry 1:[%s]\n",disassemblyinstructions[Pre_Men1.instructions[1]]);
		}
		fputs(entry,fp_w_simulation);
		//Post_MEM
		if(Post_Men1.instructions==-1){
			sprintf(singleentry,"Post-MEM Buffer:\n");
		}else{
			sprintf(singleentry,"Post-MEM Buffer:[%s]\n",disassemblyinstructions[Post_Men1.instructions]);
		}
		fputs(singleentry,fp_w_simulation);
		fputs("\n",fp_w_simulation);
		//Registers
		fputs("Registers\n",fp_w_simulation);
		char registersline[25];
		sprintf(registersline,"R00:	%d	%d	%d	%d	%d	%d	%d	%d\n",If.binary2decimal(registers[0]),If.binary2decimal(registers[1]),If.binary2decimal(registers[2]),If.binary2decimal(registers[3]),If.binary2decimal(registers[4]),If.binary2decimal(registers[5]),If.binary2decimal(registers[6]),If.binary2decimal(registers[7]));
		fputs(registersline,fp_w_simulation);
		sprintf(registersline,"R08:	%d	%d	%d	%d	%d	%d	%d	%d\n",If.binary2decimal(registers[8]),If.binary2decimal(registers[9]),If.binary2decimal(registers[10]),If.binary2decimal(registers[11]),If.binary2decimal(registers[12]),If.binary2decimal(registers[13]),If.binary2decimal(registers[14]),If.binary2decimal(registers[15]));
		fputs(registersline,fp_w_simulation);
		sprintf(registersline,"R16:	%d	%d	%d	%d	%d	%d	%d	%d\n",If.binary2decimal(registers[16]),If.binary2decimal(registers[17]),If.binary2decimal(registers[18]),If.binary2decimal(registers[19]),If.binary2decimal(registers[20]),If.binary2decimal(registers[21]),If.binary2decimal(registers[22]),If.binary2decimal(registers[23]));
		fputs(registersline,fp_w_simulation);
		sprintf(registersline,"R24:	%d	%d	%d	%d	%d	%d	%d	%d\n",If.binary2decimal(registers[24]),If.binary2decimal(registers[25]),If.binary2decimal(registers[26]),If.binary2decimal(registers[27]),If.binary2decimal(registers[28]),If.binary2decimal(registers[29]),If.binary2decimal(registers[30]),If.binary2decimal(registers[31]));
		fputs(registersline,fp_w_simulation);
		//Memory
		fputs("\n",fp_w_simulation);
		fputs("Data\n",fp_w_simulation);
		char memoryline[25];
		sprintf(memoryline,"%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",FIRSTINSTRUCTLOCATION+first_memory*4,memory[first_memory],memory[first_memory+1],memory[first_memory+2],memory[first_memory+3],memory[first_memory+4],memory[first_memory+5],memory[first_memory+6],memory[first_memory+7]);
		fputs(memoryline,fp_w_simulation);
		sprintf(memoryline,"%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",FIRSTINSTRUCTLOCATION+(first_memory+8)*4,memory[first_memory+8],memory[first_memory+9],memory[first_memory+10],memory[first_memory+11],memory[first_memory+12],memory[first_memory+13],memory[first_memory+14],memory[first_memory+15]);
		fputs(memoryline,fp_w_simulation);
		sprintf(memoryline,"%d:	%d	%d	%d	%d	%d	%d	%d	%d\n",FIRSTINSTRUCTLOCATION+(first_memory+16)*4,memory[first_memory+16],memory[first_memory+17],memory[first_memory+18],memory[first_memory+19],memory[first_memory+20],memory[first_memory+21],memory[first_memory+22],memory[first_memory+23]);
		fputs(memoryline,fp_w_simulation);

		//printf("||||||||||||||||||||||||||||||||||||||||||||||||\n");
    }
	if(fclose(fp_w_simulation)!=0){
		printf("Error in closing file \n");
	}
}
