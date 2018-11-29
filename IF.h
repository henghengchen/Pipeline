#ifndef IF_H
#define IF_H
#include "PipleLing.h"
class IF
{
    private:
        int pc=FIRSTINSTRUCTLOCATION;//根据存储器地址确定
        int Waiting_Instruction=-1;
        int Executed_Instruction=-1;
        int op_type[3]={-1,-1,-1};
        int instruction_content[4]={-1};//目的寄存器，源寄存器，源寄存器,立即数
        int op_table[8][8]={
            {0,1,2,3,4,5,6,7},
            {8,9,10,11,12,13,14,15},
            {16,17,18,19,20,21,22,23},
            {24,25,26,27,28,29,30,31},
            {32,33,34,35,36,37,38,39},
            {40,41,42,43,44,45,46,47},
            {48,49,50,51,52,53,54,55},
            {56,57,58,59,60,61,62,63}
        };
        int special_table[8][8]={
            {0,1,2,3,4,5,6,7},
            {8,9,10,11,12,13,14,15},
            {16,17,18,19,20,21,22,23},
            {24,25,26,27,28,29,30,31},
            {32,33,34,35,36,37,38,39},
            {40,41,42,43,44,45,46,47},
            {48,49,50,51,52,53,54,55},
            {56,57,58,59,60,61,62,63}
        };
        int special2_table[8][8]={
            {0,1,2,3,4,5,6,7},
            {8,9,10,11,12,13,14,15},
            {16,17,18,19,20,21,22,23},
            {24,25,26,27,28,29,30,31},
            {32,33,34,35,36,37,38,39},
            {40,41,42,43,44,45,46,47},
            {48,49,50,51,52,53,54,55},
            {56,57,58,59,60,61,62,63}
        };
        int special3_table[8][8]={
            {0,1,2,3,4,5,6,7},
            {8,9,10,11,12,13,14,15},
            {16,17,18,19,20,21,22,23},
            {24,25,26,27,28,29,30,31},
            {32,33,34,35,36,37,38,39},
            {40,41,42,43,44,45,46,47},
            {48,49,50,51,52,53,54,55},
            {56,57,58,59,60,61,62,63}
        };
        int regimm_table[4][8]={
            {0,1,2,3,4,5,6,7},
            {8,9,10,11,12,13,14,15},
            {16,17,18,19,20,21,22,23},
            {24,25,26,27,28,29,30,31},
        };
    public:
        int* get_opcode(char *line);
        int* get_special(char *line);
        int* get_special2(char *line);
        int* get_special3(char *line);
        int* get_regimm(char * line);
		int binary2decimal(char *line);
		void get_originalorcomplement(char * out,char *in);
		int getWaitingInstruction();
		int getExecutedInstruction();
        void decode_instruction(char *instruction);//判断指令是否为brunch
        void translate_instruction(char instruction[][MAXLINE],char disassemblyinstructions[][MAXLINE],int memory[],int *firstmemory);
        void run(ScoreBoardingTable *scoreboardingtable,char InstructionStorage[][MAXLINE],char executeInstructionStorage[][MAXLINE],char registers[][32],PRE_Issue *Pre_Issue,int *is_break);
};

#endif