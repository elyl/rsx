#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

void main(int argc, char **argv)
{
  int			sock;
  struct sockaddr_in	sock_in;
  struct hostent	*host;
  char			buffer[2];

  sock = socket(AF_INET, SOCK_STREAM, 0);
  host = gethostbyname(argv[1]);
  sock_in.sin_addr = *(struct in_addr*)host->h_addr;
  sock_in.sin_port = htons(atoi(argv[2]));
  sock_in.sin_family = AF_INET;
  connect(sock, (struct sockaddr*)&sock_in, sizeof(struct sockaddr));

  write(sock, "A\n", 2);
  read(sock, buffer, 1);
  close(sock);
  buffer[1] = '\0';
  printf("%s\n", buffer);
}
