#ifndef ALUB_H
#define ALUB_H
#include "PipleLing.h"
class ALUB
{
    private:
		int cycle=1;
    public:
		void get_originalorcomplement(char * out,char *in);//得到原码或补码
		void decimal2binary(char *bi,int num);
		int binary2decimal(char *line);
        void run(PRE_ALUB *Pre_Alub_front,PRE_ALUB *Pre_Alub,POST_ALUB *Post_Alub);
};

#endif

