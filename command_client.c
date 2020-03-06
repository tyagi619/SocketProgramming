#include "socket.h"
#include "common_function.c"


int getfile(int fd,char* buff, int size){
  char temp[2] = "2" ;
  if(check_file(buff)){
    char option;

    here :
    printf("Do You Wish To Overwrite %s Y/N :",buff);
    scanf("\n%c", &option);

    if(option == 'N' || option == 'n'){
      return 0;
    }
    else if(option != 'Y' && option != 'y'){
      printf("Option not recognized %c\n",option);
      goto here;
    }
  }

  send_cmd(fd,temp,sizeof(temp));
  printf("Sending Option...\n");

  recv_cmd(fd,temp,sizeof(temp));
  printf("Recieve ACK from Server %s\n",temp);

  send_cmd(fd,buff,sizeof(buff));
  printf("Sending File name : %s\n",buff);

  if(recv_confirm(fd)){
    printf("File found on server. Confirm Recieved\n");

    send_cmd(fd,temp,sizeof(temp));
    printf("ACK sent\n" );

    long int file_size = recv_file_size(fd);

    send_cmd(fd,temp,sizeof(temp));
    printf("ACK sent\n");

    recv_file(fd,buff,file_size);

    send_cmd(fd,temp,sizeof(temp));
    printf("ACK sent\n");

    recv_cmd(fd,temp,sizeof(temp));
    printf("ACK recieved\n");
  }
  else{
    printf("The requested file does not exist\n");
  }
  return 0;
}

int putfile(int fd,char* buff, int size){
  char temp[2] = "1";

  send_cmd(fd,temp,sizeof(temp));
  recv_cmd(fd,temp,sizeof(temp));

  send_cmd(fd,buff,sizeof(buff));
  if(recv_confirm(fd)){
    char option;
    recv_confirmation:
    printf("The file already exists on remote host. Do you wish to overwrite : \n");
    scanf("\n%s",&option);

    if(option == 'N' || option == 'n'){
      send_confirm(fd,false);
      recv_cmd(fd,temp,sizeof(temp));
      return 0;
    }
    else if(option == 'Y' || option == 'y'){
      send_confirm(fd,true);
      recv_cmd(fd,temp,sizeof(temp));
    }
    else if(option != 'Y' && option != 'y'){
      printf("Option not recognized %c\n",option);
      goto recv_confirmation;
    }
  }
  else{
    send_confirm(fd,true);
    recv_cmd(fd,temp,sizeof(temp));
  }

  send_file(fd,buff);
  recv_cmd(fd,temp,sizeof(temp));

  return 0;
}

int mgetfile(int fd,char *buff){
  char temp[2] = "4";

  send_cmd(fd,temp,sizeof(temp));
  recv_cmd(fd,temp,sizeof(temp));

  send_cmd(fd,buff,sizeof(buff));
  while(recv_confirm(fd)){
    send_cmd(fd,temp,sizeof(temp));
    recv_one_file(fd);
  }

  send_cmd(fd,temp,sizeof(temp));
  recv_cmd(fd,temp,sizeof(temp));
}

int mputfile(int fd,char *buff){
  char temp[3] = "3";

  send_cmd(fd,temp,sizeof(temp));
  recv_cmd(fd,temp,sizeof(temp));

  DIR *d = opendir(".");
  struct dirent *dir;
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
  return 0;
}

int closeConnection(int fd){
  char temp[2] = "5";
  send_cmd(fd,temp,sizeof(temp));
  recv_cmd(fd,temp,sizeof(temp));
  return 0;
}
