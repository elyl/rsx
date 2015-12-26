#include <stdio.h>
#include "tcpchat.h"

int receive(t_client *client, char *buffer, int buffer_size)
{
  int		n;
  unsigned int	size;

  if ((n = recvfrom(client->sock, buffer, buffer_size, 0, (struct sockaddr*)(&client->sock_in), &size)) == -1)
    printf("Reception error\n");
  return (n);
}

int send_buffer(int socket, char *buffer, int len)
{
  return (send(socket, buffer, len, 0));
}
