
#MakeFile

main: main.o board.o util.o miniMax.o evalFunc.o
	g++ -O3 -std=c++11 -o main main.o board.o util.o miniMax.o evalFunc.o

main.o: main.cpp board.h util.h 
	g++ -O3 -std=c++11 -c main.cpp 

board.o: board.cpp board.h
	g++ -O3 -std=c++11 -c board.cpp

util.o: util.cpp util.h miniMax.h
	g++ -O3 -std=c++11 -c util.cpp

miniMax.o: miniMax.cpp miniMax.h evalFunc.h
	g++ -O3 -std=c++11 -c miniMax.cpp

evalFunc.o: evalFunc.cpp evalFunc.h
	g++ -O3 -std=c++11 -c evalFunc.cpp

.PHONY: clean
clean: 
	$(RM) *.o *~
