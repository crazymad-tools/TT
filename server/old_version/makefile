TT-server:main.o MessageTable.o MessageAnal.o MessagePackage.o
	g++ -std=c++11 -o TT-server main.o MessageTable.o MessageAnal.o MessagePackage.o -lpthread -g 
main.o:main.cpp
	g++ -std=c++11 -c main.cpp -g
MessageTable.o:MessageTable.cpp
	g++ -std=c++11 -c MessageTable.cpp -g
MessageAnal.o:MessageAnal.cpp
	g++ -std=c++11 -c MessageAnal.cpp -g
MessagePackage.o:MessagePackage.cpp
	g++ -std=c++11 -c MessagePackage.cpp -g
clean:
	rm main.o MessageTable.o MessageAnal.o MessagePackage.o

