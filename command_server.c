#include "socket.h"
#include "common_function.c"

int get(int connfd){
  char buff[1024];
  char temp[2] = "2";

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

  return 0;
}

int put(int fd){
  char buff[1024];
  char temp[2] = "1";

  recv_cmd(fd,buff,sizeof(buff));

  if(check_file(buff)){

    send_confirm(fd,true);
    if(recv_confirm(fd)){
      send_cmd(fd,temp,sizeof(temp));
    }
    else{
      send_cmd(fd,temp,sizeof(temp));
      return 0;
    }
  }
  else{

    send_confirm(fd,false);
    recv_confirm(fd);
    send_cmd(fd,temp,sizeof(temp));
  }

  long int file_size = recv_file_size(fd);
  send_cmd(fd,temp,sizeof(temp));

  recv_file(fd,buff,file_size);
  send_cmd(fd,temp,sizeof(temp));

  return 0;
}

int mput(int fd){
  char buff[1024];
  char temp[2] = "3";
  while(recv_confirm(fd)){
    send_cmd(fd,temp,sizeof(temp));
    recv_put_one_file(fd);
  }
  send_cmd(fd,temp,sizeof(temp));
  return 0;
}

int mget(int fd){
  char buff[1024];
  char temp[2] = "4";

  recv_cmd(fd,buff,sizeof(buff));

  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if(d){
    while((dir = readdir(d))){
      if(dir->d_type==DT_REG){
        if(strstr(dir->d_name,buff)){
            send_confirm(fd,true);
            recv_cmd(fd,temp,sizeof(temp));
            send_one_file(fd,dir->d_name);
        }
      }
    }
  }

  send_confirm(fd,false);
  recv_cmd(fd,temp,sizeof(temp));
  send_cmd(fd,temp,sizeof(temp));

  return 0;
}
