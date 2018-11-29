#ifndef PIPLELING_H
#define PIPLELING_H
#define MAX 20

#define MAXINSTRUCTIONNUMBER 49 //指令和数据的总数
#define MAXLINE 33  //读取指令字符串长度
#define FIRSTINSTRUCTLOCATION 64  //指令首地址

using namespace std;
struct PRE_Issue
{
    int maxsize=4;
    int rear=0;
    int op_type[4][3]={{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}};
    int instruction_content[4][4]={-1};//目的寄存器，源寄存器，源寄存器,立即数
    int instructions[4]={-1,-1,-1,-1};
};

struct PRE_ALU
{
    int maxsize=2;
    int rear=0;
    int op_type[2][3]={{-1,-1,-1},{-1,-1,-1}};
	int destinationandoperands[2][3];//目的寄存器，源操作数1，源操作数2
    int instructions[2]={-1,-1};
};
struct PRE_ALUB
{
    int maxsize=2;
    int rear=0;
    int op_type[2][3]={{-1,-1,-1},{-1,-1,-1}};
	int destinationandoperands[2][3];//目的寄存器，源操作数1，源操作数2
    int instructions[2]={-1,-1};
};
struct PRE_MEN
{
    int maxsize=2;
    int rear=0;
    int op_type[2][3]={{-1,-1,-1},{-1,-1,-1}};
	int destinationandoperands[2][4];//目的寄存器，源操作数1，源操作数2，立即数
    int instructions[2]={-1,-1};
};

struct POST_MEN
{
	int is_empty=1;
	int destinationandresult[2];
    int instructions={-1};
};
struct POST_ALU
{
	int is_empty=1;
	int destinationandresult[2];
    int instructions={-1};
};
struct POST_ALUB
{
	int is_empty=1;
	int destinationandresult[2];
    int instructions={-1};
};
struct ScoreBoardingTable //ScoreBoarding表
{
    //第0行---MEN单元，第一行---ALU单元，第二行---ALUB单元
    //第0列---Busy，第一列---略，第二列---Ri（目的寄存器），第三列---Rj（原寄存器1），第四列---Rk（原寄存器2）
    //第五列---Qj（依赖单元1），第六列---Qk（依赖单元2），第七列---Rj（是否准备），第八列---Rk（是否准备）
    //int function_unit_status[3][9]={-1};
    //寄存器被哪个单元作为目的寄存器使用
    int register_result_status[32]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
};

#endif