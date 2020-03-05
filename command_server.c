#include "socket.h"

bool check_file(char* buff){
  Dir *d;
  d = opendir(".");
  if (d){
       while ((dir = readdir(d)) != NULL)
       {
           if(dir->d_type==DT_REG){
               if(strcmp(dir->dname,buff) == 0){
                 closedir(d);
                 return true;
               }
           }
       }
  }
  closedir(d);
  return false;
}

int send_confirm(int fd,bool option){
  char arr[2] = "0";
  if(option){
      arr[0] = '1';
  }
  send_confirm_again :
  int bytes_written = write(fd,arr,sizeof(char));
  if(byte_written < 0){
    if(errno == EINTR){
      goto send_confirm_again;
    }
  }
  return 0;
}

int send_file(int fd,char* buff){
  File* fp = fopen(buff,"r");
  writen(fd,(void* )fp);
  fclose(fp);
  return 0;
}
