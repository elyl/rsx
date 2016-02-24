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
#include <sys/timeb.h> 
#include <netdb.h>
#include <errno.h>
#include "client.h"

static char	gbuffer[GBUFFER_SIZE];
static int	gbuffer_len;
static int	r;
static int	s;

int main(int argc, char **argv)
{
  if (argc < 3)
    {
      printf("Usage : ./tcpclient <server> <port>\n");
      return (1);
    }
  launch_test(argv[1], atoi(argv[2]));
  return (0);
}

void launch_test(char *server, int port)
{
  struct timeb	t1;
  struct timeb	t2;
  int		buffer_size;

  buffer_size = 1;
  while (buffer_size <= 1 << 8)
    {
      ftime(&t1);
      s = 0;
      r = 0;
      communicate(server, port, buffer_size);
      ftime(&t2);
      printf("buffer_size : %d, time : %f\n", buffer_size, (double)(t2.millitm - t1.millitm));
      buffer_size <<= 1;
    }
}

void communicate(char *server, int port, int buffer_size)
{
  char			buffer_in[1024];
  char			*buffer;
  int			i;
  int			sock;
  int			n;
  struct sockaddr_in	sock_in;
  struct hostent	*host;

  buffer = malloc((buffer_size + 2) * sizeof(char));
  i = 0;
  memset(buffer, (int)(0x41 | 0x00), buffer_size);
  buffer[buffer_size] = '\n';
  while (i < 1 << 16)
    {
      // Connexion
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	perror("socket()");
      host = gethostbyname(server);
      sock_in.sin_addr = *(struct in_addr*)host->h_addr;
      sock_in.sin_port = htons(port);
      sock_in.sin_family = AF_INET;
      if (connect(sock, (struct sockaddr*)&sock_in, sizeof(struct sockaddr)) == -1)
	perror("connect()");

      printf("Emission %d\n", s + 1);
      if ((n = write(sock, buffer, buffer_size)) == -1)
	perror("write()");
      s += n;
      printf("Reception %d\n", r + 1);
      if ((n = read(sock, buffer_in, buffer_size)) == -1)
	  perror("read()");
      r += n;
      i += buffer_size;
      printf("received : %d; sent %d\n", r, s);
      if (close(sock) == -1)
	perror("close()");
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
