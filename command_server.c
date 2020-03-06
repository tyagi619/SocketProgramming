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

int send_file_size(int fd,long int file_size){
  char size_file[11];
  char temp[2];

  sprintf(size_file, "%ld", file_size);
  printf("\n%s\n", size_file);
  int len = strlen(size_file);
  int i=len-1;
  int j=9;
  size_file[10] = '\0';
  while(i>=0 && j>=0){
    size_file[j] = size_file[i];
    i--;
    j--;
  }
  i=0;
  for(;i<=j;i++){
    size_file[i] = '0';
  }
  printf("%s\n\n", size_file);

  send_cmd(fd,(void *)size_file,sizeof(size_file));
  printf("File size sent\n");

  recv_cmd(fd,temp,sizeof(temp));
  printf("ACK recieved\n");

  return 0;
}

int send_file(int fd,char* buff){
  FILE* fp = fopen(buff,"r");
  if(!fp){
    printf("%s could not be openend\n",buff);
    return 0;
  }

  fseek(fp,0L,SEEK_END);
  long int file_size = ftell(fp);
  if(file_size>1000000000){
    printf("File too Large. Aborting Sending File.\n");
    fclose(fp);
    return 0;
  }

  send_file_size(fd,file_size);

  fseek(fp,0L,SEEK_SET);
  printf("Sending File ...\n");
  writen(fd,(void* )fp);
  fclose(fp);
  printf("File Sent\n\n");
  return 0;
}
