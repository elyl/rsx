#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tcpchat.h"

void echo(char *args, t_client *client)
{
  do_echo(args, client, 1);
}

void do_echo(char *args, t_client *client, int display)
{
  char	buffer[BUFFER_SIZE + 1];
  char	*ptr;
  int	n;
  int	n2;
  int	c;

  strtok(args, " ");
  ptr = strtok(NULL, " ");
  if (ptr == NULL)
    {
      printf("Missing argument\n");
      return;
    }
  n = atoi(ptr);
  ptr = strtok(NULL, " ");
  c = 0;
  if (ptr != NULL)
    {
      c = strlen(ptr) - 2; //strlen renvoie 1 trop grand; -1 pour le \n
      ptr[c] = '\0'; // Sert à rien, strtok le fait deja
      if (display == 1)
	send(client->sock, ptr, n, 0);
    }
  printf("%d %d %s\n", n, c, ptr);
  while (c < n)
    {
      n2 = receive(client, buffer, BUFFER_SIZE);
      if (display == 1)
	send(client->sock, buffer, n2, 0);
      c += n2;
    }
  strcpy(buffer, "[echo ");
  ptr = itoa(n);
  strcat(buffer, ptr);
  strcat(buffer, "] OK\n");
  send(client->sock, buffer, 11 + strlen(ptr), 0);
  free(ptr);
}

void ack(char *args, t_client *client)
{
  do_echo(args, client, 0);
}

void compute(char *agrs, t_client *client)
{
  return;
}

