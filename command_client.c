#include "socket.h"
#include "rw.c"

int recv_file(int fd,char* buff,int size){
  FILE* fp = fopen(buff,"w+");
  if(!fp){
    printf("File could not be opened\n");
    return 0;
  }
  readn(fd,(void *)fp);
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
               // printf("%s %s\n",dir->d_name,buff);
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
    printf("%s\n",arr );
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

int getfile(int fd,char* buff, int size){
  char temp[2] = "2" ;
  if(check_file(buff)){
    char option;
    here :
    printf("Do You Wish To Overwrite %s Y/N :",buff);
    fflush(stdin);
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
  recv_cmd(fd,temp,sizeof(temp));

  send_cmd(fd,buff,size);

  if(recv_confirm(fd)){
    recv_file(fd,buff,size);
    recv_cmd(fd,temp,sizeof(temp));
  }
  else{
    printf("The requested file does not exist\n");
  }
  return 0;
}
