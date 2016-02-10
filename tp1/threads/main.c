#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "tcpchat.h"

static int nbclient;
static int r;
static int s;

int main(int argc, char **argv)
{
  if (argc < 2)
    {
      printf("Usage : ./tcpchat <port>\n");
      return (1);
    }
  nbclient = 0;
  r = 0;
  s = 0;
  open_connection(atoi(argv[1]));
  return (0);
}

void open_connection(int port)
{
  int			sock;
  struct sockaddr_in	sock_in;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror("socket()");
      exit(errno);
    }
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_family = AF_INET;
  sock_in.sin_port = htons(port);
  if (bind(sock, (struct sockaddr*)&sock_in, sizeof(sock_in)) == -1)
    {
      perror("bind()");
      exit(errno);
    }
  if (listen(sock, 5) == -1)
    {
      perror("listen()");
      exit(errno);
    }
  wait_client(sock, &sock_in);
}

void wait_client(int sock, struct sockaddr_in *sock_in)
{
  int			csock;
  struct sockaddr_in	*csin;
  unsigned int		size;
  struct moche		m;
  
  while (1)
    {
      csin = malloc(sizeof(csin));
      size = sizeof(csin);
      if ((csock = accept(sock, (struct sockaddr*)csin, &size)) == -1)
	{
	  perror("accept()");
	  exit(errno);
	}
      m.csock = csock;
      m.csin = csin;
      pthread_create((pthread_t*)&nbclient, NULL, thread_entry, &m);
      ++nbclient;
    }
}

void *thread_entry(void *arg)
{
  int			csock;
  struct sockaddr_in	*csin;
  struct moche		*m;

  printf("Nouveau client\n");
  m = (struct moche*)arg;
  csock = m->csock;
  csin = m->csin;
  listen_client(csock, csin);
  return (NULL);
}

void listen_client(int sock, struct sockaddr_in *sin)
{
  char	buffer[BUFFER_SIZE + 1];
  int	n;
  
  while ((n = read(sock, buffer, BUFFER_SIZE)) > 0)
    {
      r += n;
      //printf("reception...\n");
      /*if ((n = read(sock, buffer, BUFFER_SIZE)) < 0)
	perror("read()");*/
      buffer[n] = '\0';
      printf("%d %d %d\n", n, r, s);
      // printf("%d: %s\n", n, buffer);
      //printf("envoie...\n");
      /*if (write(sock, buffer, n + 1) == -1)
	perror("write()");*/
      if ((n = write(sock, buffer, n)) == -1)
	perror("send");
      s += n;
      /*      if (n == 0)
	      return;*/
    }
  //perror("recv()");
}
