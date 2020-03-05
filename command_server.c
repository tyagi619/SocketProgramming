#include "socket.h"

bool check_file(char* buff){
  printf("Searching File\n");
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d){
       while ((dir = readdir(d)) != NULL)
       {
           if(dir->d_type==DT_REG){
               if(strcmp(dir->d_name,buff) == 0){
                 closedir(d);
                 printf("File found\n");
                 return true;
               }
           }
       }
  }
  closedir(d);
  printf("File Not found\n");
  return false;
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

int send_file(int fd,char* buff){
  FILE* fp = fopen(buff,"r");
  if(!fp){
    printf("%s could not be openend\n",buff);
    return 0;
  }
  writen(fd,(void* )fp);
  fclose(fp);
  return 0;
}
