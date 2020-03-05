ssize_t readn(int fd,void *vptr){
  File *fp = (File *)vptr;
  size_t n;
  ssize_t nread;
  char ptr[1024];

  while(1){
      if((nread = read(fd,ptr,nleft)) < 0){
          if(errno == EINTR)
              nread=0;
          else
              return -1;
      }
      else if( nread==0 ){
          break;
      }

      fwrite(ptr,sizeof(char),sizeof(ptr)/sizeof(char),fp);
      n += nread;
  }

  return n;
}

ssize_t writen(int fd,void *vptr){
  size_t nleft,n = 0;
  ssize_t nwritten;
  FILE *fp = (FILE *)vptr;
  char ptr[1024];
  char *pt;
  while(1){
    int bytes_read = fread(ptr,sizeof(char),sizeof(ptr)/sizeof(char),fp);
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
    pt = ptr;
    nleft = bytes_read;
    while(nleft>0){
        if((nwritten = write(fd,pt,nleft)) < 0){
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        else if(nwritten == 0){
            break;
        }
        n += nwritten;
        nleft -= nwritten;
        pt += nwritten;
    }
  }
  return n;
}

ssize_t send_cmd(int fd,const void *vptr, size_t n){
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
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
  read_again :
  int bytes_read = read(fd,(char*)vptr,n);
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
