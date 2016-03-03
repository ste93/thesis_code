CC=g++
DIRSRC=src/
DIRBIN=bin/

OBJECTS=$(DIRBIN)serv.o


all: server

server: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -lpthread

$(DIRBIN)serv.o: $(DIRSRC)serv.cpp
	$(CC) -c $< -o $@

clean:
	rm -rf $(DIRBIN)*.o server 
