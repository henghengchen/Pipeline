#ifndef Issue_H
#define Issue_H
#include "PipleLing.h"

class Issue
{
    private:
		
    public:
		int binary2decimal(char *line);
		void get_originalorcomplement(char * out,char *in);
        void run(ScoreBoardingTable *scoreboardingtable,char registers[][32],PRE_Issue *Pre_Issue_front,PRE_Issue *Pre_Issue,PRE_MEN *Pre_Men,PRE_ALU *Pre_Alu,PRE_ALUB *Pre_Alub);
};

#endif