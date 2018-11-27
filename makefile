P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P2.out *.gch *.Zlst *.dbg *.p1 *.Zp2

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamZP2
	mkdir TeamZP2
	cp Project2.cpp TeamZP2
	cp LexicalAnalyzer.h TeamZP2
	cp LexicalAnalyzer.cpp TeamZP2
	cp SyntacticalAnalyzer.h TeamZP2
	cp SyntacticalAnalyzer.cpp TeamZP2
	cp makefile TeamZP2
	cp README.txt TeamZP2
	tar cfvz TeamZP2.tgz TeamZP2
	cp TeamZP2.tgz ~tiawatts/cs460drop
