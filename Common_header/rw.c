ssize_t readn(int fd,int vptr,long int size){
  printf("Recieving file in progress ....\n");
  int fp = vptr;
  long int nleft = size;
  size_t n;
  ssize_t nread;
  char ptr[1024];
  while(nleft>0){
      if((nread = read(fd,ptr,sizeof(ptr))) < 0){
          if(errno == EINTR)
              nread=0;
          else
              return -1;
      }
      else if( nread==0 ){
          break;
      }
      n += nread;
      nleft -= nread;
      while(nread>0){
        ssize_t bytes_written = write(fp,ptr,nread);
        nread -= bytes_written;
      }
  }
  printf("Recieving file finished\n");
  return n;
}

ssize_t writen(int fd,int vptr){
  size_t nleft,n = 0;
  ssize_t nwritten;
  int fp = vptr;
  char ptr[1024];
  char *pt;
  while(1){
    memset(ptr, 0x00, 1024);
    int bytes_read = read(fp,ptr,sizeof(ptr));
    if(bytes_read == 0){
      break;
    }
    if(bytes_read < 0){
      if(errno == EINTR){
        bytes_read = 0;
      }
      else{
        return -1;
      }
    }
    send(fd, ptr, bytes_read, 0);
  }
  return n;
}

ssize_t send_cmd(int fd,const void *vptr, size_t n){
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = (char *)vptr;
  nleft = n;
  while(nleft>0){
      if((nwritten = write(fd,ptr,nleft)) < 0){
          if(errno == EINTR)
              nwritten = 0;
          else
              return -1;
      }
      else if(nwritten == 0){
          break;
      }
      nleft -= nwritten;
      ptr += nwritten;
  }
  return (n-nleft);
}

ssize_t recv_cmd(int fd,void *vptr, size_t n){
  int bytes_read;
  read_again :
  bytes_read = read(fd,(char*)vptr,n);
  if(bytes_read == 0){
    return 0;
  }
  if(bytes_read < 0){
    if(errno == EINTR){
      bytes_read = 0;
      goto read_again;
    }
    else{
      return -1;
    }
  }
  return (ssize_t) bytes_read;
}
