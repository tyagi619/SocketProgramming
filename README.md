# SocketProgramming

struct sockaddr {
  uint8_t sa_len;
  sa_family_t sa_family;
  char sa_data[14];
};

struct in_addr {
in_addr_t s_addr;
};

struct sockaddr_in {
  uint8_t sin_len;
  sa_family_t sin_family;
  in_port_t sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};

/* converts string IP address to 32 bit address * /
int inet_pton (int family, const char * strptr, void * addrptr);

/* converts 32 bit address to string IP address with dot * /
const char * inet_ntop (int family, const void * addrptr, char * strptr, size_t len);


Page 89 readn, writen and readline implementations
