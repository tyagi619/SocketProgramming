#include "socket.h"
#include "rw.c"
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
	printf("Ravi\n");
	for(;;){
		printf("Anubhav\n");
		connfd = accept(listenfd, (SA *) NULL, NULL);
		printf("Anubhav\n");
		recv_cmd(connfd,buff,sizeof(char));
		printf("%c\n",buff[0]);
		switch(buff[0]){
			case '1':
				break;
			case '2':
				recv_cmd(connfd,buff,5);
				printf("%s\n",buff);
				if(check_file(buff)){
					send_confirm(connfd,true);
					send_file(connfd,buff);
				}
				else{
					send_confirm(connfd,false);
				}

				break;
			case '3':
				break;
			case '4':
				break;
			default:
				printf("If this Line works then i don't know how the code is running. i guess TCP has an error then.\n");
		}

		close(connfd);
	}






}
