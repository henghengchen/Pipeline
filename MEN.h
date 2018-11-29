#ifndef MEN_H
#define MEN_H
#include "PipleLing.h"

class MEN
{
    private:

    public:
		void get_originalorcomplement(char * out,char *in);//得到原码或补码
		void decimal2binary(char *bi,int num);
		int binary2decimal(char *line);
        void run(int *memory,PRE_MEN *Pre_Men_front,PRE_MEN *Pre_Men,POST_MEN *Post_Men);
};

#endif