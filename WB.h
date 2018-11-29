#ifndef WB_H
#define WB_H
#include "PipleLing.h"
class WB
{
    private:

    public:
		void get_originalorcomplement(char * out,char *in);//得到原码或补码
		void decimal2binary(char *bi,int num);
		int binary2decimal(char *line);
        void run(POST_MEN *Post_Men_front,POST_MEN *Post_Men,POST_ALU *Post_Alu_front,POST_ALU *Post_Alu,POST_ALUB *Post_Alub_front,POST_ALUB *Post_Alub,char registers[][32],ScoreBoardingTable *scoreboardingtable);
};

#endif