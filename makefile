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
	rm *.o P2.out *.gch

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamLetterP2
	mkdir TeamLetterP2
	cp Project2.cpp TeamLetterP2
	cp LexicalAnalyzer.h TeamLetterP2
	cp LexicalAnalyzer.cpp TeamLetterP2
	cp SyntacticalAnalyzer.h TeamLetterP2
	cp SyntacticalAnalyzer.cpp TeamLetterP2
	cp makefile TeamLetterP2
	cp README.txt TeamLetterP2
	tar cfvz TeamLetterP2.tgz TeamLetterP2
	cp TeamLetterP2.tgz ~tiawatts/cs460drop
