run:main.o MessageTable.o MessageAnal.o MessagePackage.o
	g++ -o run main.o MessageTable.o MessageAnal.o MessagePackage.o -lpthread -g
main.o:main.cpp
	g++ -c main.cpp -g
MessageTable.o:MessageTable.cpp
	g++ -c MessageTable.cpp -g
MessageAnal.o:MessageAnal.cpp
	g++ -c MessageAnal.cpp -g
MessagePackage.o:MessagePackage.cpp
	g++ -c MessagePackage.cpp -g
clean:
	rm main.o MessageTable.o MessageAnal.o MessagePackage.o

