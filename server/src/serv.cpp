/* servTCP.c   riceve stringa, trasla i caratteri di 2, e 
   rispedisce al mittente
   su SunOS compilare con gcc -o servTCP.c -lsocket -lnsl servTCP.c
   su linux               gcc -o servTCP servTCP.c                   

   eseguire ad esempio su 130.136.2.7 lanciando la seguente riga di comandi:
   servTCP 5001 
 */

//#define _GNU_SOURCE



#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
//#include "read.cpp"
#include <iostream>
//#include "write.cpp"
#include "constant.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>          /* Definition of AT_* constants */
#include <dirent.h>
#include <errno.h>
#include <pthread.h> 
#define DEVICE "/dev/loop1"
#define SOCKET_ERROR   ((int)-1)
#define SIZEBUF 1000000

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
typedef uint32_t sect_type;

using namespace std;
int write_sect(void * cont, int sect_num,int size) {

    //cout<<"Implementation of the File Handler Read Method..."<<endl;
	int l;
    int f = open(DEVICE, O_RDWR);
    if(f < 0)
    {
        cout<<"Error In Opening the HardDisk File Retuning Error..."<<endl;
        return -1;
    }
	cout << (char*)cont;
    //Read One Block of Data to Buffer
	std::cout << "writing sector" << sect_num << std::endl;
    if(lseek(f, sect_num*SECTOR_SIZE, SEEK_SET) != sect_num*SECTOR_SIZE)
    {
          perror("Couldn't seek");
          close(f);
          return -1;
    }
    if ((l = write(f, cont, size)) <= 0) {
		cout << "error";
		return -1;
	}
    close(f);
	std::cout << "sector_written" << std::endl;
    /* Do something with the data */
    /*cout<<"Length : "<<length<<endl;
	cout << block;*/
    return 1;
}




void * read_sect(int sect_num) {

    void *block = malloc(SECTOR_SIZE);
    //cout<<"Implementation of the File Handler Read Method..."<<endl;
	int l;
    int f = open(DEVICE, O_RDWR);
    if(f < 0)
    {
        cout<<"Error In Opening the HardDisk File Retuning Error..."<<endl;
        return NULL;
    }
	
    //Read One Block of Data to Buffer
    if(lseek(f, sect_num*SECTOR_SIZE, SEEK_SET) != sect_num*SECTOR_SIZE)
    {
          perror("Couldn't seek");
          close(f);
          return NULL;
    }
	std::cout << "reading sector " << sect_num <<std::endl;
    if ((l = read(f, block, SECTOR_SIZE)) <= 0) {
		cout << "error";
		return NULL;
	}
    close(f);
    /* Do something with the data */
    /*cout<<"Length : "<<length<<endl;
	cout << block;*/

    return block;
}



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

void usage(void)
{  printf ("usage: servTCP LOCAL_PORT_NUMBER\n"); exit(1); }


void *thread_cli(void *sock) {
	int socket = *(int *) sock, wr_succ;
	uint32_t len, len_net, sect, sect_net, size, size_net, aa;
	char op[3];
	void* ris;
	while (1) {
		if (readn(socket, (char*)&op, 2) <= 0) {
			cout << "errore";
			cout << op;
			pthread_exit(NULL);
		}
		op[2] = '\0';
//		std::cout << op[0] << op[1] << std::endl;
//		std::cout << "sadfas" << std::endl;
		if(strcmp(op, "re") == 0) {
			if(readn(socket, (char *) &sect_net, 4)<=0){
				pthread_exit(NULL);
			}
			sect = ntohl(sect_net);	
			pthread_mutex_lock(&mut);
			if ((ris = read_sect(sect)) == NULL) {	
				pthread_mutex_unlock(&mut);
				pthread_exit(NULL);
			}
			pthread_mutex_unlock(&mut);
			if (writen(socket, (char*)ris, SECTOR_SIZE)<=0) {
				pthread_exit(NULL);
			}
			free(ris);
		}
		else if (strcmp(op, "wr") == 0){
			std::cout << "reading from socket" << std::endl;
			if(readn(socket, (char *) &sect_net, 4)<=0){
				pthread_exit(NULL);
			}
			std::cout << sect_net << " sector " <<sect << std::endl;
			sect = ntohl(sect_net);
			if(readn(socket, (char *) &size_net, 4)<=0){
				pthread_exit(NULL);
			}
			size = ntohl(size_net);
			cout << size << "\n";
			ris = malloc(size);
			if (readn(socket, (char*)ris, size)<=0) {
				pthread_exit(NULL);
			}
			pthread_mutex_lock(&mut);
			std::cout << "writing " << ris << " " << sect << " " << size << std::endl;
			
			if(write_sect(ris, sect, size) < 0) {
				pthread_mutex_unlock(&mut);
				pthread_exit(NULL);
			}
			pthread_mutex_unlock(&mut);
			printf("sector_written\n");
			if ((wr_succ = writen(socket, (char*)&sect_net, 4))<=0) {
				printf("wr_%d\n", wr_succ);
				pthread_exit(NULL);
			}
		}
		else if (strcmp(op,"xx") == 0){
			if (writen(socket, (char*)"xx", 3)<=0) {
				pthread_exit(NULL);
			}
			cout << "closing";
			break;
		}
	}
	close(socket);
	pthread_exit(NULL);
}

int server_init(char *port) {
	struct sockaddr_in Local, Cli;
	short int local_port_number;
	int socketfd, *newsocketfd, ris;
	unsigned int len;
	pthread_t id;
/*
	if(argc!=2) { printf ("necessario 1 parametri\n"); usage(); exit(1);  }
	else {
		
	}
*/
	local_port_number = atoi(port);
	/* get a stream socket */
	printf ("socket()\n");
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == SOCKET_ERROR) {
		printf ("socket() failed, Err: %d \"%s\"\n", errno,strerror(errno));
		exit(1);
	}

	/* name the socket */
	memset ( &Local, 0, sizeof(Local) );
	Local.sin_family		=	AF_INET;
	/* indicando INADDR_ANY viene collegato il socket all'indirizzo locale IP     */
	/* dell'interaccia di rete che verrà utilizzata per inoltrare il datagram IP  */
	Local.sin_addr.s_addr	=	htonl(INADDR_ANY);         /* wildcard */
	Local.sin_port		=	htons(local_port_number);
	printf ("bind()\n");
	ris = bind(socketfd, (struct sockaddr*) &Local, sizeof(Local));
	if (ris == SOCKET_ERROR)  {
		printf ("bind() failed, Err: %d \"%s\"\n",errno,strerror(errno));
		exit(1);
	}

	printf ("listen()\n");
	ris = listen(socketfd, 10 );
	if (ris == SOCKET_ERROR)  {
		printf ("listen() failed, Err: %d \"%s\"\n",errno,strerror(errno));
		exit(1);
	}
	while(1) {
		newsocketfd = (int *)malloc(sizeof(int));
		do {
			memset ( &Cli, 0, sizeof(Cli) );
			len=sizeof(Cli);
			printf ("accept()\n");
			*newsocketfd = accept(socketfd, (struct sockaddr*) &Cli, &len);
		} while( (*newsocketfd<0)&&(errno==EINTR) );
		if (*newsocketfd == SOCKET_ERROR)  {
			printf ("accept() failed, Err: %d \"%s\"\n",errno,strerror(errno));
			free(newsocketfd);
			exit(1);
		}
		printf("connection from %s : %d\n", inet_ntoa(Cli.sin_addr), ntohs(Cli.sin_port));
		if(pthread_create(&id, NULL, &thread_cli, (void*)newsocketfd)) {
			free(newsocketfd);
			exit(1);
		}
	}
}


int main(int argc, char * argv[]) {
	server_init(argv[1]);
	return 0;
}
	/*	len = ntohl(len);
		buf = malloc(len);
		if (readn(*socket, buf, len) <= 0) {
			close(*socket);
			free(socket);
			free(buf);
			pthread_exit(NULL);
		}
		buf[len-1] = '\0';
		printf("e' arrivato %s\n", buf);
		if (strcmp(buf, "exit") == 0)
			break;
		if(stat(buf, &info) == 0) {
			printf("la stat ha avuto successo\n");
			if(S_ISREG(info.st_mode)){
				printf("file regolare\n");
				if((fd = open(buf, O_RDONLY))< 0) {
					free(socket);
					free(buf);
					pthread_exit(NULL);
					close(*socket);
				}
				while((len = read(fd, buf2, 1024)) > 0) {
					len2 = htonl(len);
					if(writen(*socket, (char*)&len2, 4) <= 0){
						close(*socket);
						close(fd);
						free(socket);
						free(buf);
						pthread_exit(NULL);
					}
					if(writen(*socket, buf2, len) <= 0){
						close(*socket);
						close(fd);
						free(socket);
						free(buf);
						pthread_exit(NULL);
					}
				}
				close(fd);
			}
			else if(S_ISDIR(info.st_mode)) {
				printf("cartella\n");
				dir = opendir(buf);
				while((ent = readdir(dir))!= NULL) {
					asprintf(&str, "%s/%s", buf, ent->d_name);
					if(stat(str, &info) == 0) {
						if(S_ISREG(info.st_mode)) {
							len2 = htonl(strlen(ent->d_name));
							if(writen(*socket, (char*)&len2, 4) <= 0){
								close(*socket);
								free(socket);
								free(buf);
								pthread_exit(NULL);
							}
							len2 = ntohl(len2);
							if(writen(*socket, ent->d_name, len2) <= 0){
								close(*socket);
								free(socket);
								free(buf);
								pthread_exit(NULL);
							}
						}
					}
				}
				closedir(dir);
			}
			else {
				cond = 1;
			}
		}
		else {
			cond = 1;
		}
		
		if(cond == 1) {
			printf("ls\n");
			dir = opendir(".");
			while((ent = readdir(dir))!= NULL) {
				len2 = htonl(strlen(ent->d_name));
				if(writen(*socket, (char*)&len2, 4) <= 0){
					close(*socket);
					free(socket);
					free(buf);
					pthread_exit(NULL);
				}
				len2 = ntohl(len2);
				if(writen(*socket, ent->d_name, len2) <= 0){
					close(*socket);
					free(socket);
					free(buf);
					pthread_exit(NULL);
				}
			}
			closedir(dir);
		}
		
		len = 0;
		if(writen(*socket, (char*)&len, 4) <= 0){
			close(*socket);
			free(socket);
			free(buf);
			pthread_exit(NULL);
		}
		cond = 0;*/
