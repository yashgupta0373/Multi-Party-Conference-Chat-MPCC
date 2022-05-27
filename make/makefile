#variables
CC=g++
RM=rm
TFLAG=-pthread
DBFLAG=-lsqlite3
INCLUDEPATH= ../include
SRCPATH= ../src
BINPATH= ../bin
OBJPATH= ../obj

PHONY: all clean

all: $(BINPATH)/server.exe $(BINPATH)/client.exe


$(BINPATH)/server.exe: $(OBJPATH)/servermain.o $(OBJPATH)/server.o

	$(CC) -g -o server.exe $(OBJPATH)/servermain.o $(OBJPATH)/server.o $(TFLAG)
	mv ./server.exe $(BINPATH)/


$(BINPATH)/client.exe: $(OBJPATH)/clientmain.o $(OBJPATH)/client.o

	$(CC) -g -o client.exe $(OBJPATH)/clientmain.o $(OBJPATH)/client.o $(TFLAG) $(DBFLAG)
	mv ./client.exe $(BINPATH)/


$(OBJPATH)/servermain.o: $(SRCPATH)/servermain.cpp $(INCLUDEPATH)/header.h $(INCLUDEPATH)/server.h $(INCLUDEPATH)/global.h 

	$(CC) -c -Wall -g $(SRCPATH)/servermain.cpp -I $(INCLUDEPATH)/
	mv ./servermain.o $(OBJPATH)/


$(OBJPATH)/server.o: $(SRCPATH)/server.cpp $(INCLUDEPATH)/header.h $(INCLUDEPATH)/server.h $(INCLUDEPATH)/global.h 

	$(CC) -c -Wall -g $(SRCPATH)/server.cpp -I $(INCLUDEPATH)/
	mv ./server.o $(OBJPATH)/


$(OBJPATH)/clientmain.o: $(SRCPATH)/clientmain.cpp $(INCLUDEPATH)/header.h $(INCLUDEPATH)/client.h $(INCLUDEPATH)/global.h $(INCLUDEPATH)/database.h

	$(CC) -c -Wall -g $(DBFLAG) $(SRCPATH)/clientmain.cpp -I $(INCLUDEPATH)/
	mv ./clientmain.o $(OBJPATH)/


$(OBJPATH)/client.o: $(SRCPATH)/client.cpp $(INCLUDEPATH)/header.h $(INCLUDEPATH)/client.h $(INCLUDEPATH)/global.h $(INCLUDEPATH)/database.h

	$(CC) -c -Wall -g $(SRCPATH)/client.cpp -I $(INCLUDEPATH)/
	mv ./client.o $(OBJPATH)/


clean:

	$(RM) -f $(OBJPATH)/*.o
	$(RM) -f $(BINPATH)/client.exe $(BINPATH)/server.exe