#include "socket.h"
#include "rw.c"

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

long int recv_file_size(int fd){
  char size_file[11];
  recv_cmd(fd,size_file,sizeof(size_file));
  printf("Recieved file size\n");
  long int file_size = atol(size_file);
  printf("Size of requested file is: %ld bytes\n\n", file_size);
  return file_size;
}

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
