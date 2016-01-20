#include "tcpchat.h"

int receive(t_client *client, char *buffer, int buffer_size)
{
  int		n;
  unsigned int	size;

  if ((n = recvfrom(client->sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)(&client->sock_in), &size)) == -1)
    printf("Reception error\n");
  return (n);
}
