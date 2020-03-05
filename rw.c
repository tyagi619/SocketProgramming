ssize_t readn(int fd,void *vptr, size_t n){
  size_t nleft;
  ssize_t nread;
  char *ptr;

  ptr = vptr;
  nleft = n;
  while(nleft > 0){
      if((nread = read(fd,ptr,nleft)) < 0){
          if(errno == EINTR)
              nread=0;
          else
              return -1;
      }
      else if( nread==0 ){
          break;
      }
      nleft -= nread;
      ptr += nread;
  }

  return (n-nleft);
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
