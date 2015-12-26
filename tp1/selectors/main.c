#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include "tcpchat.h"

extern fd_set	readfds;
extern fd_set	writefds;
extern int	fdmax;

static t_client_list	client_list;
static t_command cmd[] = {
  {"ack",	ack},
  {"echo",	echo},
  {"compute",	compute}
};

int main(int argc, char **argv)
{
  int			port;
  
  if (argc < 2)
    {
      printf("Usage : ./tcpchat <port>\n");
      exit (1);
    }
  port = atoi(argv[1]);
  start_listening(port);
  return (0);
}

void start_listening(int port)
{
  int			sock;
  struct sockaddr_in	sock_in;
  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    printf("Erreur de generation de socket\n");
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_family = AF_INET;
  sock_in.sin_port = htons(port);
  if ((bind(sock, (struct sockaddr*)&sock_in, sizeof(sock_in))) == -1)
    printf("Erreur de bind\n");
  if ((listen(sock, 5)) == -1)
    printf("Erreur de listen\n");
  fdmax = sock;
  FD_ZERO(&readfds);
  FD_SET(sock, &readfds);
  do_listen(sock, &sock_in);
}

void do_listen(int sock, struct sockaddr_in *sock_in)
{
  t_client	*client;
  unsigned int	size;
  int		n;

  while (1)
    {
      size = sizeof(*sock_in);
      printf("Mise en attente...\n");
      n = select(fdmax + 1, &readfds, NULL, NULL, NULL);
      printf("Activité detectee %d\n", n);
      client = malloc(sizeof(t_client));
      if (FD_ISSET(sock, &readfds))
	{
	  --n;
	  printf("Ajout d'un client\n");
	  if ((client->sock = accept(sock, (struct sockaddr*)sock_in, &size)) == -1)
	    printf("Erreur d'accept\n");
	  client_list.list = add_client(client_list.list, client);
	  if (client->sock > fdmax)
	    fdmax = client->sock;
	}
      if (n != 0)
	do_recv();
      printf("Ajout principal\n");
      add_all(&readfds);
      FD_SET(sock, &readfds);
    }
}

void do_recv()
{
  int		len;
  t_client	*client;
  char		buffer[BUFFER_SIZE];
  int		i;
  int		n;
  unsigned int	size;

  size = 0;
  add_all(&readfds);
  printf("Ecoute d'un client\n");
  n = select(fdmax + 1, &readfds, NULL, NULL, NULL);
  client = client_list.list;
  while (client != NULL && n != 0)
    {
      if (FD_ISSET(client->sock, &readfds))	
	{
	  printf("En attente de reception\n");
	  if ((len = recvfrom(client->sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)(&client->sock_in), &size)) == -1)
	    printf("Erreur recv %d\n", errno);
	  printf("%d %s\n", len, buffer);
	  if (len > 1 && buffer[0] == '/')
	    {
	      i = 0;
	      while (i < 3)
		{
		  if (strncmp(cmd[i].text, &buffer[1], strlen(cmd[i].text)) == 0)
		    cmd[i].fn(buffer, client);
		  ++i;
		}
	    }
	  else
	    send_all(buffer, len - 1);
	  if (len > 2 && strncmp("bye", buffer, 3) == 0)
	    close_client(client);
	  --n;
	  client = client->next;
	}
    }
}

void close_client(t_client *client)
{
  close(client->sock);
  delete_client(client);
}

void send_all(char *buffer, size_t len)
{
  t_client	*list;
  int		n;

  add_all(&writefds);
  printf("Attente d'une dispo en écriture\n");
  n = select(fdmax + 1, NULL, &writefds, NULL, NULL);
  printf("Dispo détectée !\n");
  list = client_list.list;
  while (list != NULL && n != 0)
    {
      if (FD_ISSET(list->sock, &writefds))
	{
	  printf("%d\n", (int)send(list->sock, buffer, len + 1, 0));
	  --n;
	}
      // Ajouter dans un buffer si pas ok
      list = list->next;
    }
}

void delete_client(t_client *client)
{
  if (client->prev != NULL)
    client->prev->next = client->next;
  if (client->next != NULL)
    client->next->prev = client->prev;
  free(client);
}

t_client *add_client(t_client *list, t_client *client)
{
  t_client	*first;

  first = list;
  client->next = NULL;
  if (list == NULL)
    {
      client->prev = NULL;
      return (client);
    }
  while (list->next != NULL)
    list = list->next;
  list->next = client;
  client->prev = list;
  return (first);
}

void add_all(fd_set *set)
{
  t_client	*ptr;

  FD_ZERO(set);
  ptr = client_list.list;
  while (ptr != NULL)
    {
      printf("Ajout : %d\n", ptr->sock);
      FD_SET(ptr->sock, set);
      ptr = ptr->next;
    }
}
