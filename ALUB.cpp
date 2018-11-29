#include "PipleLing.h"
#include "ALUB.h"
#include <stdio.h>
void ALUB::run(PRE_ALUB *Pre_Alub_front,PRE_ALUB *Pre_Alub,POST_ALUB *Post_Alub){
	if(Pre_Alub->rear!=0){
		if(cycle==1){
			cycle++;
			Post_Alub->is_empty=1;
			Post_Alub->instructions=-1;
			Post_Alub->destinationandresult[0]=-1;
			Post_Alub->destinationandresult[1]=-1;
		}else{
			if(Pre_Alub->op_type[0][0]==1&&Pre_Alub->op_type[0][1]==2){//MUL
				int result=Pre_Alub->destinationandoperands[0][1]*Pre_Alub->destinationandoperands[0][2];
				Post_Alub->destinationandresult[0]=Pre_Alub->destinationandoperands[0][0];
				Post_Alub->destinationandresult[1]=result;
				Post_Alub->instructions=Pre_Alub->instructions[0];
				Post_Alub->is_empty=0;
				//Pre_Alu数据前移
				Pre_Alub->instructions[0]=Pre_Alub->instructions[1];
				Pre_Alub_front->instructions[0]=Pre_Alub_front->instructions[1];
				Pre_Alub->instructions[1]=-1;
				Pre_Alub_front->instructions[1]=-1;
				for(int i=0;i<3;i++){
					Pre_Alub->op_type[0][i]=Pre_Alub->op_type[1][i];
					Pre_Alub_front->op_type[0][i]=Pre_Alub_front->op_type[1][i];
				}
				for(int i=0;i<3;i++){
					Pre_Alub->op_type[1][i]=0;
					Pre_Alub_front->op_type[1][i]=0;
				}
				for(int i=0;i<3;i++){
					Pre_Alub->destinationandoperands[0][i]=Pre_Alub->destinationandoperands[1][i];
					Pre_Alub_front->destinationandoperands[0][i]=Pre_Alub_front->destinationandoperands[1][i];
				}
				for(int i=0;i<3;i++){
					Pre_Alub->destinationandoperands[1][i]=0;
					Pre_Alub_front->destinationandoperands[1][i]=0;
				}
				Pre_Alub->rear--;
				Pre_Alub_front->rear--;
			}else{
				if(Pre_Alub->op_type[0][0]==0){
					if(Pre_Alub->op_type[0][1]==0&&Pre_Alub->op_type[0][2]==0){//SLL
						int result=Pre_Alub->destinationandoperands[0][1]<<Pre_Alub->destinationandoperands[0][2];
						Post_Alub->destinationandresult[0]=Pre_Alub->destinationandoperands[0][0];
						Post_Alub->destinationandresult[1]=result;
						Post_Alub->instructions=Pre_Alub->instructions[0];
						Post_Alub->is_empty=0;
						//Pre_Alub_front数据前移
						Pre_Alub->instructions[0]=Pre_Alub->instructions[1];
						Pre_Alub_front->instructions[0]=Pre_Alub_front->instructions[1];
						Pre_Alub->instructions[1]=-1;
						Pre_Alub_front->instructions[1]=-1;
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[0][i]=Pre_Alub->op_type[1][i];
							Pre_Alub_front->op_type[0][i]=Pre_Alub_front->op_type[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[1][i]=0;
							Pre_Alub_front->op_type[1][i]=0;
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[0][i]=Pre_Alub->destinationandoperands[1][i];
							Pre_Alub_front->destinationandoperands[0][i]=Pre_Alub_front->destinationandoperands[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[1][i]=0;
							Pre_Alub_front->destinationandoperands[1][i]=0;
						}
						Pre_Alub->rear--;
						Pre_Alub_front->rear--;
					}else if(Pre_Alub->op_type[0][1]==0&&Pre_Alub->op_type[0][2]==2){//SRL
						int result=Pre_Alub->destinationandoperands[0][1]>>Pre_Alub->destinationandoperands[0][2];
						//printf("sa：%d\n",Pre_Alub->destinationandoperands[0][2]);
						//printf("ALUB结果：%d\n",result);
						Post_Alub->destinationandresult[0]=Pre_Alub->destinationandoperands[0][0];
						Post_Alub->destinationandresult[1]=result;
						Post_Alub->instructions=Pre_Alub->instructions[0];
						Post_Alub->is_empty=0;
						//Pre_Alub_front数据前移
						Pre_Alub->instructions[0]=Pre_Alub->instructions[1];
						Pre_Alub_front->instructions[0]=Pre_Alub_front->instructions[1];
						Pre_Alub->instructions[1]=-1;
						Pre_Alub_front->instructions[1]=-1;
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[0][i]=Pre_Alub->op_type[1][i];
							Pre_Alub_front->op_type[0][i]=Pre_Alub_front->op_type[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[1][i]=0;
							Pre_Alub_front->op_type[1][i]=0;
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[0][i]=Pre_Alub->destinationandoperands[1][i];
							Pre_Alub_front->destinationandoperands[0][i]=Pre_Alub_front->destinationandoperands[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[1][i]=0;
							Pre_Alub_front->destinationandoperands[1][i]=0;
						}
						Pre_Alub->rear--;
						Pre_Alub_front->rear--;
					}else if(Pre_Alub->op_type[0][1]==0&&Pre_Alub->op_type[0][2]==3){//SRA
						int sa=Pre_Alub->destinationandoperands[0][2];
						char bit_num[32];
						decimal2binary(bit_num, Pre_Alub->destinationandoperands[0][1]);
						for(int i=31-sa;i>=0;i--){
							bit_num[i+sa]=bit_num[i];
						}
						for(int i=0;i<sa;i++){
							bit_num[i]=bit_num[sa];
						}
						int result=binary2decimal(bit_num);
						Post_Alub->destinationandresult[0]=Pre_Alub->destinationandoperands[0][0];
						Post_Alub->destinationandresult[1]=result;
						Post_Alub->instructions=Pre_Alub->instructions[0];
						Post_Alub->is_empty=0;
						//Pre_Alub_front数据前移
						Pre_Alub->instructions[0]=Pre_Alub->instructions[1];
						Pre_Alub_front->instructions[0]=Pre_Alub_front->instructions[1];
						Pre_Alub->instructions[1]=-1;
						Pre_Alub_front->instructions[1]=-1;
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[0][i]=Pre_Alub->op_type[1][i];
							Pre_Alub_front->op_type[0][i]=Pre_Alub_front->op_type[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[1][i]=0;
							Pre_Alub_front->op_type[1][i]=0;
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[0][i]=Pre_Alub->destinationandoperands[1][i];
							Pre_Alub_front->destinationandoperands[0][i]=Pre_Alub_front->destinationandoperands[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[1][i]=0;
							Pre_Alub_front->destinationandoperands[1][i]=0;
						}
						Pre_Alub->rear--;
						Pre_Alub_front->rear--;
					}else if(Pre_Alub->op_type[0][1]==28&&Pre_Alub->op_type[0][2]==2){//MUL
						int result=Pre_Alub->destinationandoperands[0][1]*Pre_Alub->destinationandoperands[0][2];
						Post_Alub->destinationandresult[0]=Pre_Alub->destinationandoperands[0][0];
						Post_Alub->destinationandresult[1]=result;
						Post_Alub->instructions=Pre_Alub->instructions[0];
						Post_Alub->is_empty=0;
						//Pre_Alub_front数据前移
						Pre_Alub->instructions[0]=Pre_Alub->instructions[1];
						Pre_Alub_front->instructions[0]=Pre_Alub_front->instructions[1];
						Pre_Alub->instructions[1]=-1;
						Pre_Alub_front->instructions[1]=-1;
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[0][i]=Pre_Alub->op_type[1][i];
							Pre_Alub_front->op_type[0][i]=Pre_Alub_front->op_type[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->op_type[1][i]=0;
							Pre_Alub_front->op_type[1][i]=0;
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[0][i]=Pre_Alub->destinationandoperands[1][i];
							Pre_Alub_front->destinationandoperands[0][i]=Pre_Alub_front->destinationandoperands[1][i];
						}
						for(int i=0;i<3;i++){
							Pre_Alub->destinationandoperands[1][i]=0;
							Pre_Alub_front->destinationandoperands[1][i]=0;
						}
						Pre_Alub->rear--;
						Pre_Alub_front->rear--;
					}
				}
			}
			cycle--;
		}
	}else{
		Post_Alub->is_empty=1;
		Post_Alub->instructions=-1;
		Post_Alub->destinationandresult[0]=-1;
		Post_Alub->destinationandresult[1]=-1;
	}
}

void ALUB::get_originalorcomplement(char * out,char *in){//得到原码或补码
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

void ALUB::decimal2binary(char *bi,int num){
	int number=num;
	int bit;
	int index=31;
	if(number>0){
		while(number>0){
			bit=number%2;
			bi[index]='0'+bit;
			number=number/2;
			index--;
		}
		bi[0]='0';
		for(int i=1;i<=index;i++){
			bi[i]='0';
		}
	}else{
		number=0-number;
		char temp[32];
		while(number>0){
			bit=number%2;
			temp[index]='0'+bit;
			number=number/2;
			index--;
		}
		temp[0]='1';
		for(int i=1;i<=index;i++){
			temp[i]='0';
		}
		get_originalorcomplement(bi, temp);
	}
}


int ALUB::binary2decimal(char *line)
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


