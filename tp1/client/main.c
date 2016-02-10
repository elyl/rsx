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
#include "client.h"

static char	gbuffer[GBUFFER_SIZE];
static int	gbuffer_len;

int main(int argc, char **argv)
{
  if (argc < 3)
    {
      printf("Usage : ./tcpclient <server> <port>\n");
      return (1);
    }
  read_file();
  launch_test(argv[1], atoi(argv[2]));
  return (0);
}

void launch_test(char *server, int port)
{
  time_t	t1;
  time_t	t2;
  int		buffer_size;

  buffer_size = 1;
  while (buffer_size <= 1 << 16)
    {
      t1 = 0;
      time(&t1);
      communicate(server, port, buffer_size);
      t2 = 0;
      time(&t2);
      printf("buffer_size : %d, time : %f\n", buffer_size, (double)(t2 - t1));
      buffer_size <<= 1;
    }
}

void communicate(char *server, int port, int buffer_size)
{
  char			*buffer;
  int			i;
  int			sock;
  struct sockaddr_in	sock_in;
  struct hostent	*host;

  buffer = malloc((buffer_size + 1) * sizeof(char));
  //Connexion
    i = 0;
  while (i < gbuffer_len)
    {
      sock = socket(AF_INET, SOCK_STREAM, 0);
      host = gethostbyname(server);
      sock_in.sin_addr = *(struct in_addr*)host->h_addr;
      sock_in.sin_port = htons(port);
      sock_in.sin_family = AF_INET;
      connect(sock, (struct sockaddr*)&sock_in, sizeof(struct sockaddr));
      buffer_size = (gbuffer_len - i < buffer_size) ? gbuffer_len - i : buffer_size;
      memcpy(buffer, &gbuffer[i], buffer_size);
      printf("envoie...\n");
      write(sock, buffer, buffer_size);
      printf("reception...\n");
      read(sock, buffer, buffer_size + 1);
      i += buffer_size;
      close(sock);
    }
  free(buffer);
}

void read_file()
{
  char	buffer[BUFFER_SIZE + 1];
  int	fd;
  int	n;

  gbuffer_len = 0;
  fd = open("file", O_RDONLY);
  while ((n = read(fd, buffer, BUFFER_SIZE)) != 0)
    {
      buffer[n] = '\0';
      if (n == 0)
	strcpy(gbuffer, buffer);
      else
	strcat(gbuffer, buffer);
      gbuffer_len += n;
    }
  close(fd);
}
