#include <stdlib.h>
#include "tcpchat.h"

void echo(char *args, t_client *client)
{
  char	buffer[BUFFER_SIZE];
  char	*ptr;
  int	n;
  int	c;

  strtok(args, " ");
  ptr = strtok(NULL, " ");
  c = strlen(ptr);
  n = atoi(ptr);
  ptr = strtok(NULL, " ");
  c = BUFFER_SIZE - c - 4 - 2 - 1;
  while (c < n)
    c += receive(client, buffer, BUFFER_SIZE);
  send();
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

