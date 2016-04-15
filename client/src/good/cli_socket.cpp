#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <errno.h>
#include <iostream>
#include "header_cli.h"



//write len bytes on the given socket
int writen(int sock, char *buf, int len) {
	int nsent = 0, ris;
	while (nsent < len) {
		ris = send(sock, buf+nsent, len-nsent, MSG_NOSIGNAL);
		if (ris == -1 && errno == EINTR)
			continue;
		else if (ris <= 0)
			return ris;
		nsent += ris;
	}
	return nsent;
}

//here I can improve by sending few packages
//read len bytes from the given socket
int readn(int sock, char *buf, int len) {
	int nread = 0, ris;
	while (nread < len) {
		ris = read(sock, buf+nread, len-nread);
		if (ris == -1 && errno == EINTR)
			continue;
		else if (ris <= 0)
			return ris;
		nread += ris;
	}
	return nread;
}

int sendData(int sock, void * data, sect_type sect, int size){
	uint32_t l, s, ack, ack_net, size_net;
	if (writen(sock, (char*)"wr", 2)<=0) {
		perror("command");
		return -1;
	}
	s = htonl(sect);
	//std::cout << s << " sector " << sect << std::endl;
	if (writen(sock, (char*)&s, 4)<=0) {
		perror("number of the sector");
		return -1;
	}
	size_net = htonl(size);
	//std::cout << size_net << " " << size << std::endl;
	if (writen(sock, (char*)&size_net, 4)<=0) {
		perror("number of the sector");
		return -1;
	}
	if (writen(sock, (char*)data, size)<=0) {
		perror("data of the sector");
		return -1;
	}
	if (readn(sock,(char*)&ack_net, 4) <=0){
		perror("ack");
		return -1;
	}
	ack = ntohl(ack_net);
	if (ack != sect) {
		perror ("write failed");
		return -1;
	}
	//std::cout << "ack ok\n";
	return 0;
}

int retrieveData(int sock, sect_type sector, void ** ris) {
	uint32_t l, l_comm, ack, sect;
	int len, ack_int;
	l = htonl(sizeof(uint32_t));
	l_comm = htonl(2);
	sect =  htonl(sector);
	//here I send that I want to read
	if (writen(sock, (char*)"re", 2)<=0) {
		perror("command");
		return 0;
	}
	//I have to send the number of the sector of the data to be retrieved
	if (writen(sock, (char*)&sect, 4)<=0) {
		perror("number of the sector");
		return 0;
	}
	//here I receive the length of te record
	if (readn(sock, (char*)&l, 4)<=0) {
		perror("receiving length");
		return 0;
	}
	len = ntohl(l);
	//I suppose that all the records are 512 byte
	std::cout << "cli_sock " << len << std::endl;
	*ris = new char[len];
	/* here I receive the answer */
	
	if (readn(sock, (char *)*ris, len)<=0) {
		perror("record not retrieved");
		free(*ris);
		return 0;
	}
	std::cout << "retrieved all" << std::endl;
	BIO_dump_fp (stdout, (const char *)ris, len);
	std::cout << *ris << std::endl;
/*	//this is the acknowledgment for the server
	if (writen(sock, (char*)sect, 4)<=0) {
		perror("ack client send");
		return NULL;
	}
	//this is the acknowledgment of the server
	if (readn(sock, (char*)&ack, 4)<=0) {
		perror("receiving length");
		return NULL;
	}
	if (ntohl(ack) != sector){
		return NULL;
	}*/
	return len;
}

int serverConnectionFinish(int socketfd) {
	char ack[2];
	if (writen(socketfd, (char*)"xx", 2)<=0) {
		perror("command");
		return -1;
	}
	if (readn(socketfd, (char*)&ack, 2)<=0) {
		perror("receiving ack");
		return -1;
	}
	if (strcmp(ack, "xx")!= 0) {
		return -1;
	}
	close(socketfd);
	return 0;
}

//here I need the socket file descriptor only
int serverConnectionInit(char *ip_addr,char *port, int *socket_main) {
	struct sockaddr_in Local, Serv;
	char *string_remote_ip_address;
	short int remote_port_number;
	int ris, socketfd;
	char inp[INPSIZE];
	string_remote_ip_address = (char *)ip_addr;
	remote_port_number = atoi(port);

	/* get a datagram socket */
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == SOCKET_ERROR) {
		printf ("socket() failed, Err: %d \"%s\"\n", errno,strerror(errno));
		return 1;
	}

	/* name the socket */
	memset ( &Local, 0, sizeof(Local) );
	Local.sin_family		=	AF_INET;
	Local.sin_addr.s_addr	=	htonl(INADDR_ANY);         /* wildcard */
	Local.sin_port	=	htons(0);
	ris = bind(socketfd, (struct sockaddr*) &Local, sizeof(Local));
	if (ris == SOCKET_ERROR)  {
		printf ("bind() failed, Err: %d \"%s\"\n",errno,strerror(errno));
		return 1;
	}

	/* assign our destination address */
	memset ( &Serv, 0, sizeof(Serv) );
	Serv.sin_family	 =	AF_INET;
	Serv.sin_addr.s_addr  =	inet_addr(string_remote_ip_address);
	Serv.sin_port		 =	htons(remote_port_number);

	/* connection request */
	//std::cout << ip_addr << " " << port << "\n";
	ris = connect(socketfd, (struct sockaddr*) &Serv, sizeof(Serv));
	if (ris == SOCKET_ERROR)  {
		printf ("connect() failed, Err: %d \"%s\"\n",errno,strerror(errno));
		//in case of error must handle it and send an error message
		return 1;
	}
	*socket_main = socketfd;
	return 0;
}
/*
int main(int argc, char * argv[]) {
	int sock, l, ri;
	char * read_res;
	char sent[512] = "llslsllslllslllslllsllslslls";
	serverConnectionInit("127.0.0.1", "3333", &sock);
	ri = sendData(sock, sent, 44);
	printf("%d\n",ri );
	read_res = (char *)retrieveData(sock, 44);
	printf("%s\n", read_res);
	l = serverConnectionFinish(sock);
	std::cout << l << "\n";
	return 0;
}
*/
