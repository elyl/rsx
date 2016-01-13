#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include "tcpchat.h"

static t_client_list	client_list;

int main(int argc, char **argv)
{
  int			port;

  client_list.nb_clients = 0;
  client_list.mutex = 0;
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
  unsigned int		size;
  struct sockaddr_in	sock_in;
  t_client		*client;
  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    printf("Erreur de generation de socket\n");
  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_family = AF_INET;
  sock_in.sin_port = htons(port);
  if ((bind(sock, (struct sockaddr*)&sock_in, sizeof(sock_in))) == -1)
    printf("Erreur de bind\n");
  if ((listen(sock, 5)) == -1)
    printf("Erreur de listen\n");
  size = sizeof(sock_in);
  while (1)
    {
      client = malloc(sizeof(t_client));
      if ((client->sock = accept(sock, (struct sockaddr*)&sock_in, &size)) == -1)
	printf("Erreur d'accept\n");
      while (client_list.mutex == 1);
      client_list.mutex = 1;
      add_client(client_list.list, client);
      pthread_create(&client_list.nb_clients, NULL, listen_client, client);
      ++(client_list.nb_clients);
      client_list.mutex = 0;
    }
}

void *listen_client(void *arg)
{
  t_client	*client;
  unsigned int	size;
  char		buffer[255];

  client = (t_client*)arg;
  while (1)
    {
      if ((recvfrom(client->sock, buffer, 255, 0, (struct sockaddr*)(&client->sock_in), &size)) == -1)
	printf("Erreur recv\n");
      send_all(buffer);
      printf("%s\n", buffer);
      if (strncmp("bye", buffer, 3) == 0)
	close_client(client);
    }
  return (NULL);
}

void close_client(t_client *client)
{
  while (client_list.mutex == 1);
  client_list.mutex = 1;
  close(client->sock);
  delete_client(client);
  client_list.mutex = 0;
  pthread_exit(NULL);
}

void send_all(char *buffer)
{
  t_client	*list;

  while (client_list.mutex == 1);
  client_list.mutex = 1;
  list = client_list.list;
  while (list != NULL)
    {

      list = list->next;
    }
  client_list.mutex = 0;
}

void delete_client(t_client *client)
{
  if (client->prev != NULL)
    client->prev->next = client->next;
  client->next->prev = client->prev;
  free(client);
}

t_client *add_client(t_client *list, t_client *client)
{
  t_client	*first;

  first = list;
  if (list == NULL)
    return (client);
  while (list->next != NULL)
    list = list->next;
  list->next = client;
  client->prev = list;
  return (first);
}
