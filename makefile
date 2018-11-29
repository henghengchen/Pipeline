MIPSsim:PipleLing.o IF.o Issue.o MEN.o ALU.o ALUB.o WB.o
	g++ PipleLing.o IF.o Issue.o MEN.o ALU.o ALUB.o WB.o -g -o MIPSsim
PipleLing.o:PipleLing.cpp PipleLing.h IF.h Issue.h ALU.h ALUB.h MEN.h WB.h
	g++ -c PipleLing.cpp
IF.o:IF.cpp IF.h PipleLing.h
	g++ -c IF.cpp
Issue.o:Issue.cpp Issue.h PipleLing.h
	g++ -c Issue.cpp
MEN.o:MEN.cpp MEN.h PipleLing.h
	g++ -c MEN.cpp
ALU.o:ALU.cpp ALU.h PipleLing.h
	g++ -c ALU.cpp
ALUB.o:ALUB.cpp ALUB.h PipleLing.h
	g++ -c ALUB.cpp
WB.o:WB.cpp WB.h PipleLing.h
	g++ -c WB.cpp