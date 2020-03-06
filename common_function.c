#include "socket.h"
#include "rw.c"

bool check_file(char* buff){
  printf("Checking %s on local host\n",buff );
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d){
       while ((dir = readdir(d)) != NULL)
       {
           if(dir->d_type==DT_REG){
               if(strcmp(dir->d_name,buff) == 0){
                 closedir(d);
                 printf("%s found\n",buff );
                 return true;
               }
           }
       }
  }
  closedir(d);
  printf("%s not found\n",buff );
  return false;
}

long int recv_file_size(int fd){
  char size_file[11];
  recv_cmd(fd,size_file,sizeof(size_file));
  printf("Recieved file size\n");
  long int file_size = atol(size_file);
  printf("Size of requested file is: %ld bytes\n\n", file_size);
  return file_size;
}

int send_file_size(int fd,long int file_size){
  char size_file[11];

  sprintf(size_file, "%010ld", file_size);
  printf("\n%s\n", size_file);

  send_cmd(fd,size_file,sizeof(size_file));
  printf("File size sent\n");

  return 0;
}

int send_confirm(int fd,bool option){
  char arr[2] = "0";
  if(option){
      arr[0] = '1';
  }
  int bytes_written;
  send_confirm_again :
  bytes_written = write(fd,arr,sizeof(arr));
  if(bytes_written < 0){
    if(errno == EINTR){
      goto send_confirm_again;
    }
  }
  return 0;
}

bool recv_confirm(int fd){
    char arr[2];
    int bytes_read;

    recv_again :
    bytes_read = read(fd,arr,sizeof(arr));
    printf("Server returned file status : %s\n",arr);
    if(bytes_read == 0){
      return 0;
    }
    if(bytes_read < 0){
      if(errno == EINTR){
        bytes_read = 0;
        goto recv_again;
      }
      else{
        return -1;
      }
    }
    return (arr[0] == '1');
}

int recv_file(int fd,char* buff,long int size){
  FILE* fp = fopen(buff,"w+");
  if(!fp){
    printf("File could not be opened\n");
    return 0;
  }
  readn(fd,(void *)fp,size);
  fclose(fp);
  return 0;
}

int send_file(int fd,char* buff){
  char temp[2] = "1";
  FILE* fp = fopen(buff,"r");
  if(!fp){
    printf("%s could not be openend\n",buff);
    return 0;
  }

  fseek(fp,0L,SEEK_END);
  long int file_size = ftell(fp);
  fseek(fp,0L,SEEK_SET);
  if(file_size>1000000000){
    printf("File too Large. Aborting Sending File.\n");
    fclose(fp);
    return 0;
  }

  send_file_size(fd,file_size);
  recv_cmd(fd,temp,sizeof(temp));
  printf("ACK recieved\n");

  printf("Sending File ...\n");
  writen(fd,(void* )fp);
  fclose(fp);
  printf("File Sent\n\n");
  return 0;
}

int send_one_file(int fd, char *name){
  char buff[1024];
  char temp[2] = "1";
  strcpy(buff,name);

  send_cmd(fd,buff,sizeof(buff));
  printf("%s\n",buff);
  if(!recv_confirm(fd)){
    return 0;
  }

  send_file(fd,buff);
  recv_cmd(fd,temp,sizeof(temp));

  return 0;
}

int recv_one_file(int fd){
  char buff[1024];
  char temp[2] = "4";

  recv_cmd(fd,buff,sizeof(buff));
  if(check_file(buff)){
    char option;

    confirm_recv:
    printf("The file %s already exists. Do you wish to overwrite? : ",buff);
    scanf("\n%c",&option);

    if(option == 'N' || option == 'n'){
      send_confirm(fd,false);
      return 0;
    }
    else if(option == 'Y' || option == 'y'){
      send_confirm(fd,true);
    }
    else{
      printf("Option not recognized.\n");
      goto confirm_recv;
    }
  }
  else{
    send_confirm(fd,true);
  }
  long int file_size = recv_file_size(fd);
  send_cmd(fd,temp,sizeof(temp));

  recv_file(fd,buff,file_size);
  send_cmd(fd,temp,sizeof(temp));
  return 0;
}

int put_one_file(int fd, char *name){
  char buff[1024];
  char temp[2] = "1";
  strcpy(buff,name);

  send_cmd(fd,buff,sizeof(buff));
  printf("%s\n",buff);
  if(recv_confirm(fd)){
    char option;

    put_file_confirm:
    printf("The file %s already exists. Do you want to overwrite? : ", buff);
    scanf("\n%c",&option);
    if(option == 'N' || option == 'n'){
      send_confirm(fd,false);
      recv_cmd(fd,temp,sizeof(temp));
      return 0;
    }
    else if(option == 'Y' || option == 'y'){
      send_confirm(fd,true);
      recv_cmd(fd,temp,sizeof(temp));
    }
    else{
      printf("Option not recognized.\n");
      goto put_file_confirm;
    }
  }

  send_file(fd,buff);
  recv_cmd(fd,temp,sizeof(temp));
  return 0;
}

int recv_put_one_file(int fd){
  char buff[1024];
  char temp[2] = "4";

  recv_cmd(fd,buff,sizeof(buff));
  if(check_file(buff)){
    send_confirm(fd,true);
  }
  else{
    send_confirm(fd,false);
  }

  if(recv_confirm(fd)){
    send_cmd(fd,temp,sizeof(temp));
  }
  else{
    send_cmd(fd,temp,sizeof(temp));
    return 0;
  }

  long int file_size = recv_file_size(fd);
  send_cmd(fd,temp,sizeof(temp));

  recv_file(fd,buff,file_size);
  send_cmd(fd,temp,sizeof(temp));
  return 0;
}
