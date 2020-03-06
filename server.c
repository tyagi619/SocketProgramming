#include "socket.h"
#include <time.h>
#include "command_server.c"

int Socket(int family, int type,int protocol){
	int n;
	if((n=socket(family,type,protocol))<0){
		printf("error\n");
		exit(0);
	}
	return (n);
}


int main(int argc, char **argv){
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT); /* daytime server */

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);
	char temp[2] = "0";

	printf("Waiting for Connection\n");

	for(;;){

		connfd = accept(listenfd, (SA *) NULL, NULL);

		printf("Connection Successful\n");

		while(1){

				recv_cmd(connfd,temp,sizeof(temp));
				printf("Option recieved\n");
				printf("Option chosen : %s\n",temp);

				send_cmd(connfd,temp,sizeof(temp));
				printf("ACK sent\n");

				switch(temp[0]){
					case '1':
						put(connfd);
						break;
					case '2':
						get(connfd);
						break;
					case '3':
						mput(connfd);
						break;
					case '4':
						mget(connfd);
						break;
					case '5':
						goto out;
						break;
					default:
					return 0;
						printf("If this Line works then i don't know how the code is running. i guess TCP has an error then.\n");
				}
		}
		out:
		close(connfd);
	}
}
