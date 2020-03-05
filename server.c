#include "socket.h"
#include "rw.c"
#include <time.h>
#include <dirent.h>

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

	for(;;){
		connfd = accept(listenfd, (SA *) NULL, NULL);
		DIR *d;
		struct dirent *dir;
		d = opendir("/home/anu/Downloads");
		if (d){
				 while ((dir = readdir(d)) != NULL)
				 {
						 if(dir->d_type==DT_REG){
							 	 snprintf(buff, sizeof(buff), "%s\n",dir->d_name);
								 write(connfd,buff,strlen(buff));
						 }
				 }
		 }
		 closedir(d);
		// ticks = time(NULL);
		// snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		// write(connfd, buff, strlen(buff));
		close(connfd);
	}
}
