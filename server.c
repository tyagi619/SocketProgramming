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
						break;
					case '2':
						recv_cmd(connfd,buff,sizeof(buff));
						printf("Requested File : %s\n",buff);

						if(check_file(buff)){

							send_confirm(connfd,true);
							printf("Confirmation sent\n" );

							recv_cmd(connfd,temp,sizeof(temp));
							printf("ACK recieved\n");

							send_file(connfd,buff);

							recv_cmd(connfd,temp,sizeof(temp));
							printf("ACK recieved\n");

							send_cmd(connfd,temp,sizeof(temp));
							printf("ACK sent\n");
						}
						else{

							send_confirm(connfd,false);
							printf("Confirmation sent\n" );
							printf("Requested File Not Found : %s\n",buff);
						}
						break;
					case '3':
						break;
					case '4':
						break;
					case '5':
						break;
					default:
						printf("If this Line works then i don't know how the code is running. i guess TCP has an error then.\n");
				}
		}
		close(connfd);
	}






}
