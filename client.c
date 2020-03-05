#include "socket.h"
#include "command_client.c"

int Socket(int family, int type,int protocol){
	int n;
	if((n=socket(family,type,protocol))<0){
		printf("error\n");
		exit(0);
	}
	return (n);
}

int main(int argc, char **argv){
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	if (argc != 2){
		printf("usage: a.out <IPaddress>");
		return -1;
	}
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT); /* daytime server */
	printf("Testing1\n");
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
		printf("inet_pton error for %s", argv[1]);
		return -1;
	}
	printf("Testing\n");
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
		printf("connect error");
		return -1;
	}
	printf("Testing Succes\n" );
	char buff[1024];
	int option;
	while(1){
		printf("Enter Key For The Coomand 1. Put \n 2. Get \n 3. Mput \n 4. Mget \n 5. Close Connection :\n");
		scanf("%d", &option);
		switch(option){
			case 1 :
				break;
			case 2 :
				printf("Enter Filename : ");
				scanf("%s", buff);
				getfile(sockfd,buff,sizeof(1024));
				break;
			case 3 :
			case 4 :
			case 5 :
				close(sockfd);
				goto out;
			default :
				printf("Command not recognized\n");
		}
	}
	out:
	return 0;
}
