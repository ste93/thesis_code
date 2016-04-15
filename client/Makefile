CC=g++
DIRSRC=src/good/
DIRBIN=bin/

OBJECTS=$(DIRBIN)utils_tree.o $(DIRBIN)cli_socket.o $(DIRBIN)cli_server.o $(DIRBIN)creation.o $(DIRBIN)find_and_print.o $(DIRBIN)insertion.o $(DIRBIN)lic.o $(DIRBIN)aes.o $(DIRBIN)main.o -lssl -lcrypto -lgmpxx -lgmp


all: client

client: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(DIRBIN)main.o: $(DIRSRC)main.cpp -lgmpxx -lgmp
	$(CC) -c $< -o $@

$(DIRBIN)insertion.o: $(DIRSRC)insertion.cpp
	$(CC) -c $< -o $@

$(DIRBIN)creation.o: $(DIRSRC)creation.cpp
	$(CC) -c $< -o $@

$(DIRBIN)find_and_print.o: $(DIRSRC)find_and_print.cpp
	$(CC) -c $< -o $@

$(DIRBIN)cli_server.o: $(DIRSRC)cli_server.cpp
	$(CC) -c $< -o $@

$(DIRBIN)cli_socket.o: $(DIRSRC)cli_socket.cpp
	$(CC) -c $< -o $@

$(DIRBIN)utils_tree.o: $(DIRSRC)utils_tree.cpp
	$(CC) -c $< -o $@ -fpermissive -lssl -lcrypto

$(DIRBIN)lic.o: $(DIRSRC)lic.cpp
	$(CC) -c $< -o $@

$(DIRBIN)aes.o: $(DIRSRC)aes.cpp
	$(CC) -c $< -o $@ -lssl -lcrypto
clean:
	rm -rf $(DIRBIN)*.o client 
