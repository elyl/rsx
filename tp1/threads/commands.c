#include <stdlib.h>
#include <string.h>
#include "tcpchat.h"

void echo(char *args, t_client *client)
{
  char	buffer[BUFFER_SIZE];
  char	*ptr;
  int	n;
  int	n2;
  int	c;

  strtok(args, " ");
  ptr = strtok(NULL, " ");
  n = atoi(ptr);
  ptr = strtok(NULL, " ");
  c = strlen(ptr) - 1; //strlen renvoie 1 trop grand
  ptr[c] = '\0';
  printf("%d %d %s\n", n, c, ptr);
  send(client->sock, ptr, n, 0);
  while (c < n)
    {
      n2 = receive(client, buffer, BUFFER_SIZE);
      send(client->sock, buffer, n, 0);
      c += n2;
    }
  strcpy(buffer, "[echo ");
  ptr = itoa(n);
  strcat(buffer, ptr);
  strcat(buffer, "] OK\n");
  send(client->sock, buffer, 11 + strlen(ptr), 0);
  free(ptr);
  return;
}

void ack(char *args, t_client *client)
{
  return;
}

void compute(char *agrs, t_client *client)
{
  return;
}

